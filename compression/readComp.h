#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int getSizeArrayRegular(FILE *f);
int getSizeElementArrayRegular(FILE *f);
void getElementArrayRegular(int i,int sizeElem,void *s,FILE *f);

int getSizeList(FILE *f);
int getSizeElementList(FILE *f);
void getElementList(FILE *f,int size,void *s);
char** getList(FILE *f,int *size);

int32_t getSizeArray(FILE *f);
int getSizeElementArray(int i,int sizeArray,FILE *f);
void getElementArray(void *s,int size,FILE *f);

int getParamsListArrays(int *sizeList,int *sizeArray,int *sizeElem,FILE *f);
void getElementRun(int i,int sizeArray,int sizeElem,void *s,FILE *f);
void getListElementListArrays(int i,int sizeList,int sizeArray,int sizeElem,void *s,FILE *f);

int getParamsArrayArrays(int *sizeMainArray,int *sizeArrays,int *sizeElem,FILE *f);
void getElementArrayArrays(int i,int j,int sizeMainArray,int sizeArrays,int sizeElem,void *s,FILE *f);

void test();
