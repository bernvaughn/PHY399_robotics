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

        
def main():
    waldo = (pngToList(gResourceDir+gWaldoPNG))
    printBadMap(waldo,"walOut.txt")
    toSearch = (pngToList(gResourceDir+gImageToSearch))
    print("");
    printBadMap(toSearch,"mapOut.txt")

if __name__ == "__main__":
    main()
