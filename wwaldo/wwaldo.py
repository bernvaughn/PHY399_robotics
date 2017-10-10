#! /usr/bin/env python3
#
# Vaughn J. Bernard
# 10/8/17
# This program hopefully finds waldo.
#
# -----------------------------------------------
import sys
from PIL import Image

gResourceDir = "./resources/"
gWaldoPNG = "waldo.png"
gImageToSearch = "new_waldoSearch02.png"

def pngToList(filename):
    file = Image.open(filename)
    pixels = file.load()
    w,h = file.size
    result = []
    for y in range(h):
        thisLine = []
        for x in range(w):
            try:
                thisLine.append(max(pixels[x,y]))
            except TypeError:
                thisLine.append(pixels[x,y])
        result.append(thisLine)
        #print("This line(",len(result),": ",thisLine)
    #print("png to list success.")
    return list(result)

def printBadMap(aList,f):
    file = open(f,"w")
    for y in aList:
        #print("This y: ",y)
        for x in y:
            #print("This x: ",x)
            if x < 150: print("X",end="",file=file)
            else: print("_",end="",file=file)
        print("",file=file)
    file.close()

# I call it bad because it produces a list with only two values.
def badList(aList):
    result = []
    for y in aList:
        thisLine = []
        for x in y:
            if x < 150: thisLine.append("X")
            else: thisLine.append("_")
        result.append(thisLine)
    return result

def growWaldo(waldo):
    result = []
    for y in waldo:
        thisLine = []
        for x in y:
            thisLine+=x+x
        result.append(thisLine)
        result.append(thisLine)
    return result

def likeness(waldo,maybeWaldo):
    likeness,count = 0,0
    for yi in range(len(waldo)):
        for xi in range(len(waldo[0])):
            count+=1
            if waldo[xi][yi] == maybeWaldo[xi][yi]: likeness += 1
    return likeness/count
        
def main():
    waldo = (pngToList(gResourceDir+gWaldoPNG))
    toSearch = (pngToList(gResourceDir+gImageToSearch))
    waldo = badList(waldo)
    toSearch = badList(toSearch)
    bigWaldo = growWaldo(waldo)
    for item in waldo:
        print(item)
    print("-"*49)
    for item in bigWaldo:
        print(item)
    print("-"*49)
    print(likeness(waldo,waldo))
    print(likeness(waldo,toSearch))

if __name__ == "__main__":
    main()
