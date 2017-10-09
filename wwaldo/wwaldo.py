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
            thisLine.append(pixels[x,y])
        result.append(thisLine)
    return list(result)

def printBadMap(aList):
    for y in aList:
        for x in y:
            if max(x) < 150: print("X",end="")
            else: print("_",end="")
        print("")

        
def main():
    waldo = (pngToList(gResourceDir+gWaldoPNG))
    printBadMap(waldo)
    toSearch = (pngToList(gResourceDir+gImageToSearch))
    print("");
    printBadMap(toSearch)

if __name__ == "__main__":
    main()
