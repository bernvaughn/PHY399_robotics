#! /usr/bin/env python3
#
# Vaughn J. Bernard
# 10/8/17
# This program hopefully finds waldo.
# NOTE: EDIT "gImageToSearch" to change which image is searched.
# -----------------------------------------------
import sys
from PIL import Image

gResourceDir = "./resources/"
gWaldoPNG = "waldo.png"
gImageToSearch = "new_waldoSearch01.png"
gDebugMode = 0 # set to 1 to enable verbose outputs

def pngToList(filename):
    '''In: filename as string that is the path and name of
        the png file to load
    Out: 2-dimensional list of numbers based on maximum
        value of each pixel
    '''
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
    '''In: aList as a 2-dimensional list of numbers
    In: f as string that is the output file's path and name
    '''
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
    '''In: aList as 2-dimensional list of numbers
    Out: a list of X and _
    '''
    result = []
    for y in aList:
        thisLine = []
        for x in y:
            if x < 150: thisLine.append("X")
            else: thisLine.append("_")
        result.append(thisLine)
    return result

def growWaldo(waldo):
    '''In: waldo as 2-dimensional list
    Out: 2-dimensional list that is twice the size of waldo, scaled.
    '''
    result = []
    for y in waldo:
        thisLine = []
        for x in y:
            thisLine+=x+x
        result.append(thisLine)
        result.append(thisLine)
    return result

def rotateWaldo(waldo):
    '''In: waldo as 2-dimensional list
    Out: 2-dimensional list that is rotated 90 degrees clockwise.
    '''
    result = []
    for yi in range(len(waldo)):
        thisLine = []
        for xi in range(len(waldo[0])):
            thisLine.append(waldo[len(waldo)-1-xi][yi])
        result.append(thisLine)
    return result

def likeness(waldo,maybeWaldo):
    '''In: waldo as 2-dimensional list
    In: maybeWaldo as 2-dimensional list to compare to
    Out: likeness as float, scale of 0-1. 1 is 100% alike, .5 is 50% alike, etc.
    '''
    likeness,count = 0,0
    for yi in range(len(waldo)):
        for xi in range(len(waldo[0])):
            count+=1
            if waldo[yi][xi] == maybeWaldo[yi][xi]: likeness += 1
    return likeness/count

def find(waldo,toSearch,tolerance):
    '''In: waldo as 2-dimensional array to find instances of
    In: toSearch as 2-dimensional array to find waldo within
    In: tolerance as float between 0 and 1
        0 returns everything, 1 returns only exact matches
    Out: list of tuples of x,y coordinates of occurances
    '''
    result = []
    for yi in range(len(toSearch)-len(waldo)):
        for xi in range(len(toSearch[0])-len(waldo[0])):
            #get a chunk of toSearch as big as waldo and offset by xi,yi
            thisSearch = []
            for syi in range(yi,yi+len(waldo)):
                thisSearch.append(toSearch[syi][xi:xi+len(waldo[0])])
                
            like = likeness(waldo,thisSearch)
            if like >=tolerance:
                result.append(str(xi+round(len(waldo)/2))+" "+str(yi+round(len(waldo[0])/2)))
    return result

def printBadMap(badMap):
    '''In: badMap as 2-dimensional list
    Prints a badMap in human-readable form. Mostly for debugging.
    '''
    for y in badMap:
        for x in y:
            print(x,end="")
        print("")
        
def main():
    waldo = (pngToList(gResourceDir+gWaldoPNG))
    toSearch = (pngToList(gResourceDir+gImageToSearch))
    
    waldo = badList(waldo)
    toSearch = badList(toSearch)
    bigWaldo = growWaldo(waldo)

    waldo90 = rotateWaldo(waldo)
    waldo180 = rotateWaldo(waldo90)
    waldo270 = rotateWaldo(waldo180)

    if gDebugMode == 1:
        print("FILE TO SEARCH:",gResourceDir+gWaldoPNG)
        print("USING MASK:",gResourceDir+gImageToSearch)

        print("Running program...\n")

        a = find(waldo,toSearch,0.8)
        b = find(bigWaldo,toSearch,0.8)
        c = find(waldo90,toSearch,0.73)
        d = find(waldo180,toSearch,0.73)
        e = find(waldo270,toSearch,0.73)

        a.sort()
        b.sort()
        f = c+d+e
        f.sort()
        
        print(len(a),"Waldos:\n-------")
        for item in a:
            print(item)
        print("-"*7)
        print(len(b),"Big Waldos:\n-------")
        for item in b:
            print(item)
        print("-"*7)
        print(len(f),"Rotated Waldos:\n-------")
        for item in f:
            print(item)
        print("-"*7)
    else:
        a = find(waldo,toSearch,0.8)
        b = find(bigWaldo,toSearch,0.8)
        c = find(waldo90,toSearch,0.73)
        d = find(waldo180,toSearch,0.73)
        e = find(waldo270,toSearch,0.73)
        waldos = sorted(a+b+c+d+e)
        for item in waldos:
            print(item)
    

if __name__ == "__main__":
    main()
