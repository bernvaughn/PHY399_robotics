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

def likeness(waldo,maybeWaldo):
    '''In: waldo as 2-dimensional list
    In: maybeWaldo as 2-dimensional list to compare to
    Out: likeness as float. 1 is 100% alike, .5 is 50% alike, etc.
    '''
    likeness,count = 0,0
    for yi in range(len(waldo)):
        for xi in range(len(waldo[0])):
            count+=1
            if waldo[xi][yi] == maybeWaldo[xi][yi]: likeness += 1
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
                result.append([xi+round(len(waldo)/2),yi+round(len(waldo[0])/2)])
##              #debug
##              print("-"*79)
##              for y in thisSearch:
##                  print(y)
##              print("-"*79)
##              for y in waldo:
##                  print(y)
##              print("-"*79)
##              print(xi,yi,like)
##              print("-"*79)
##          print("Waldo at: x =",xi,"y =",yi)
    return result
        
def main():
    waldo = (pngToList(gResourceDir+gWaldoPNG))
    toSearch = (pngToList(gResourceDir+gImageToSearch))
    
    waldo = badList(waldo)
    toSearch = badList(toSearch)
    bigWaldo = growWaldo(waldo)

    print("Running program...")
    print("Waldos:",find(waldo,toSearch,0.8));
    print("Big Waldos:",find(bigWaldo,toSearch,0.8));

if __name__ == "__main__":
    main()
