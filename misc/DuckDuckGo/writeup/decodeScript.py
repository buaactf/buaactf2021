#!/usr/bin/env python2.7
import base64
import argparse
from PIL import Image
import math

def decode(filenameOut, table, l):
	im = Image.open(filenameOut)
	pixels = list(im.getdata())
	dimension = int(math.sqrt(len(pixels)))
	d1 = dimension
	d2 = dimension - 1
	d3 = dimension + 1

	colours = [
		(255, 0, 0),
		(0, 0, 255),
		(0, 128, 0),
		(255, 255, 0)
	]

	x = 0
	y = 0
	i = 0
	res = ''
	while i < dimension * dimension:
		p = []
		i1 = pixels[i]
		i2 = pixels[i+1]
		i3 = pixels[i+2]
		i4 = pixels[i+3]
		try:
			p.append(colours.index(i1))
			p.append(colours.index(i2))
			p.append(colours.index(i3))
			p.append(colours.index(i4))
		except:
			pass

		try:
			charI = table.index(p)
			res = res + l[charI]
		except:
			pass

		i = i+4
	return res



def arrayBuild():
	charList = ['b', 'a', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z','A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '/', '+', '=']
	table = []
	colours = [
		(255, 0, 0),
		(0, 0, 255),
		(0, 128, 0),
		(255, 255, 0)
	]
	x,y = 0,0
	for i in charList:
		part = []
		part.append(int(ord(i)/64)&3)
		part.append(int(ord(i)/16)&3)
		part.append(int(ord(i)/4)&3)
		part.append(int(ord(i)/1)&3)
		table.append(part)
	return table, charList

if __name__ == '__main__':
	table, l = arrayBuild()
	res = decode('pixelduck.png', table, l)
	image_64_decode = base64.decodestring(res) 
	image_result = open('res', 'wb')
	image_result.write(image_64_decode)

# run with python2.7