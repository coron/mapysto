# Copyright (c) 2014 Jean-Sebastien Coron <jscoron@gmail.com>
#
# This program is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License version 2 as published
# by the Free Software Foundation.

import struct

# stores an array of strings of equal length
def storeArrayRegular(data):
  return struct.pack("II",len(data),len(data[0]))+"".join(data)

# stores a list of strings of arbitrary length
def storeList(data):
  return struct.pack("I",len(data))+"".join([struct.pack("I",len(dat))+dat for dat in data])

# stores an array of strings of arbitrary length
def storeArray(a):
  li,li2,m=[],[],0
  for x in a:
    li.append(struct.pack("I",m))
    li2.append(struct.pack("I",len(x))+x)
    m+=len(x)+4
  return struct.pack("I",len(a))+"".join(li)+"".join(li2)

def runs(li):
  lim=255
  lir=[]
  x,n=li[0],1
  for y in li[1:]:
    if y!=x or n==lim:
      lir.append((n,x))
      x,n=y,1
    else:
      n+=1
  lir.append((n,x))
  return lir

# given a list of string elements of equal length, stores with compressed runs
def storeRuns(li):
  return struct.pack("I",len(li))+"".join([struct.pack("B",n)+x for n,x in runs(li)])

# store a list of equal-length arrays of equal-length strings, with the arrays in compressed runs
def storeListArrays(data):
  sizeList=len(data)
  sizeArray=len(data[0])
  sizeElem=len(data[0][0])
  return struct.pack("III",sizeList,sizeArray,sizeElem)+"".join([storeRuns(dat) for dat in data])

# store an array of equal-length arrays of equal-length strings, with the arrays in compressed runs
def storeArrayArrays(data):
  sizeMainArray=len(data)
  sizeArrays=len(data[0])
  sizeElem=len(data[0][0])
  return struct.pack("III",sizeMainArray,sizeArrays,sizeElem)+storeArray([storeRuns(dat) for dat in data])

def tests():
  from subprocess import check_output
  open("testArrayRegular.dat","w").write(storeArrayRegular(["toto","tutu"]))
  open("testList.dat","w").write(storeList(["tata","tu"]))
  open("testArray.dat","w").write(storeArray(["toto","te","tutu"]))
  open("testListArrays.dat","w").write(storeListArrays([["toto","toto","tutu"],["tata","tete","tete"]]))
  open("testArrayArrays.dat","w").write(storeArrayArrays([["toto","toto","tutu"],["tata","tete","tete"]]))
  assert(check_output(["./testComp.out"])=="sizeArray=2\nsizeElementArray=4\ntototutu\ntata\ntu\nte\nsizeList=2,sizeArray=3,sizeElem=4\ntototete\nsizeMainArray=2,sizeArrays=3,sizeElem=4\ntete\n")
  import os
  os.system("rm *.dat")

if __name__=="__main__":
  tests()
  
  
  
  
