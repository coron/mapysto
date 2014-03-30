// Copyright (c) 2014 Jean-Sebastien Coron <jscoron@gmail.com>
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License version 2 as published
// by the Free Software Foundation.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

int32_t read_int32(FILE *f)
{
  int32_t a;
  size_t nr=fread(&a,sizeof(int32_t),1,f);
  if (nr!=1)
  {
    fprintf(stderr,"Error in read_int32\n");
    exit(1);
  }
  return a;
}

//////////////////////////////////////////////////////////////
// Rountines for reading an array of strings of constant length
//////////////////////////////////////////////////////////////

int getSizeArrayRegular(FILE *f)
{
    return read_int32(f);
}

int getSizeElementArrayRegular(FILE *f)
{
    return read_int32(f);
}

void getElementArrayRegular(int i,int sizeElem,void *s,FILE *f)
{
    fseek(f,i*sizeElem,SEEK_CUR);
    fread(s,sizeElem,1,f);
}

/////////////////////////////////////////////////////////////
// Routines for reading a list of strings of arbitrary length
/////////////////////////////////////////////////////////////

int getSizeList(FILE *f)
{
  return read_int32(f);
}

int getSizeElementList(FILE *f)
{
  return read_int32(f);
}
  
void getElementList(FILE *f,int size,void *s)
{
  size_t nr=fread(s,1,size,f);
  if(nr!=size)
  {
    fprintf(stderr,"Error in getElementList\n");
    exit(1);
  }
}  

char** getList(FILE *f,int *size)
{
  *size=getSizeList(f);
  char **data;
  data=malloc(*size*sizeof(char*));
  int i;
  for(i=0;i<*size;i++)
  {
    int l=getSizeElementList(f);
    data[i]=malloc(l+1);
    getElementList(f,l,data[i]);
    data[i][l]='\0';
  }
  return data;
}
    

//////////////////////////////////////////////////////////////////
// Routines for reading an array of strings of arbitrary lengths
//////////////////////////////////////////////////////////////////

int32_t getSizeArray(FILE *f)
{
    return read_int32(f);
}

int getSizeElementArray(int i,int sizeArray,FILE *f)
{
    if (i>=sizeArray)
    {
        fprintf(stderr,"Index error in getSizeElementArray\n");
        exit(1);
    }
    int32_t pos;
    fseek(f,i*sizeof(int32_t),SEEK_CUR);
    fread(&pos,sizeof(int32_t),1,f);
    fseek(f,(sizeArray-i-1)*sizeof(int32_t),SEEK_CUR);
    
    fseek(f,pos,SEEK_CUR);
    int32_t size;
    fread(&size,sizeof(int32_t),1,f);
    return size;
}

void getElementArray(void *s,int size,FILE *f)
{
    fread(s,1,size,f);
}

void testArray()
{
    FILE *f=fopen("testArray.dat","r");
    int sizeArray=getSizeArray(f);
    int i=1;
    int sizeElem=getSizeElementArray(i,sizeArray,f);
    char *s=malloc(sizeElem+1);
    getElementArray(s,sizeElem,f);
    s[sizeElem]='\0';
    printf("%s\n",s);
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Routines for reading a list of equal-length arrays of equal-length strings in compressed form
////////////////////////////////////////////////////////////////////////////////////////////////

int getParamsListArrays(int *sizeList,int *sizeArray,int *sizeElem,FILE *f)
{
    *sizeList=read_int32(f);
    *sizeArray=read_int32(f);
    *sizeElem=read_int32(f);
    return 3*sizeof(int32_t);
}

// Recovers the i-th element in an array in compressed form
void getElementRun(int i,int sizeArray,int sizeElem,void *s,FILE *f)
{
  assert(i>=0);
  if(i>=sizeArray)
  {
    fprintf(stderr,"Index error in getElementRun:i=%d sizeArray=%d\n",i,sizeArray);
    exit(1);
  }
  int32_t size;
  fread(&size,sizeof(int32_t),1,f);
  int ind=0;
  while (ind<sizeArray)
  {
    unsigned char n;
    fread(&n,1,1,f);
    int n2=n;
    if ((i>=ind) && (i<(ind+n2)))
      fread(s,sizeElem,1,f);
    else
      fseek(f,sizeElem,SEEK_CUR);
    ind+=n2;
  }
}

// Recovers the list of the i-th element in the arrays in compressed form
void getListElementListArrays(int i,int sizeList,int sizeArray,int sizeElem,void *s,FILE *f)
{
  int j;
  for(j=0;j<sizeList;j++)
  {
    getElementRun(i,sizeArray,sizeElem,s+sizeElem*j,f);
  }
}



//////////////////////////////////////////////////////////////////////////////////////
// Routines for reading an array of equal-length arrays of equal-length strings in compressed form
//////////////////////////////////////////////////////////////////////////////////////

int getParamsArrayArrays(int *sizeMainArray,int *sizeArrays,int *sizeElem,FILE *f)
{
    *sizeMainArray=read_int32(f);
    *sizeArrays=read_int32(f);
    *sizeElem=read_int32(f);
    return 3*sizeof(int32_t);
}

void getElementArrayArrays(int i,int j,int sizeMainArray,int sizeArrays,int sizeElem,void *s,FILE *f)
{
    int si=getSizeArray(f);
    assert(sizeMainArray==si);
    getSizeElementArray(i,sizeMainArray,f);
    getElementRun(j,sizeArrays,sizeElem,s,f);
}



//////////////////////////////////////////////////////////
void testArrayRegular()
{
  FILE *f=fopen("testArrayRegular.dat","r");
  unsigned int size=getSizeArrayRegular(f),
               sizeElem=getSizeElementArrayRegular(f);
  printf("sizeArray=%d\nsizeElementArray=%d\n",size,sizeElem);
  char buf[255];
  assert(sizeElem<=255);
  long pos=ftell(f);
  unsigned int i,j;
  for(i=0;i<size;i++)
  {
    fseek(f,pos,SEEK_SET);
    getElementArrayRegular(i,sizeElem,buf,f);
    for(j=0;j<sizeElem;j++)
      printf("%c",buf[j]);
  }
  printf("\n");
}

void testList()
{
  FILE *f=fopen("testList.dat","r");
  int size;
  char **data=getList(f,&size);
  int i;
  for(i=0;i<size;i++)
  {
      printf("%s\n",data[i]);
  }
}

void testListArrays()
{
    FILE *f=fopen("testListArrays.dat","r");
    int sizeList,sizeArray,sizeElem;
    getParamsListArrays(&sizeList,&sizeArray,&sizeElem,f);
    printf("sizeList=%d,sizeArray=%d,sizeElem=%d\n",sizeList,sizeArray,sizeElem);
    char *s=malloc(sizeList*sizeElem+1);
    getListElementListArrays(1,sizeList,sizeArray,sizeElem,s,f);
    s[sizeList*sizeElem]='\0';
    printf("%s\n",s);
}

void testArrayArrays()
{
    FILE *f=fopen("testArrayArrays.dat","r");
    int sizeMainArray,sizeArrays,sizeElem;
    getParamsArrayArrays(&sizeMainArray,&sizeArrays,&sizeElem,f);
    printf("sizeMainArray=%d,sizeArrays=%d,sizeElem=%d\n",sizeMainArray,sizeArrays,sizeElem);
    char *s=malloc(sizeElem+1);
    getElementArrayArrays(1,2,sizeMainArray,sizeArrays,sizeElem,s,f);
    s[sizeElem]='\0';
    printf("%s\n",s);
}

void test()
{
  testArrayRegular();
  testList();
  testArray();
  testListArrays();
  testArrayArrays();
}
