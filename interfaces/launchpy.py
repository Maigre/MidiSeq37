#!/usr/bin/python
#
# Quick usage of "launchpad.py", LEDs and buttons.
# Works with all Launchpads: Mk1, Mk2, S/Mini and Pro.
#
#
# ASkr 7/2013..1/2017
# www.askrprojects.net
#

import sys

try:
	import launchpad_py as launchpad
except ImportError:
	try:
		import launchpad
	except ImportError:
		sys.exit("error loading launchpad.py")

import random, copy
from pygame import time

import liblo
import json

# create server, listening on port 9000
try:
    server = liblo.Server(9000)
except liblo.ServerError as err:
    print(err)
    sys.exit()

# send all messages to port 9001 on the local machine
try:
    target = liblo.Address(9001)
except liblo.AddressError as err:
    print(err)
    sys.exit()

# Internal Layout MATRIX
matrixOUT = []
for x in range(0, 8):
	matrixOUT.append([])
	for y in range(0,8):
		matrixOUT[x].append((0,0))

matrixIN = []
for x in range(0, 8):
	matrixIN.append([])
	for y in range(0,8):
		matrixIN[x].append((1,0))

print (json.dumps(matrixIN))

def setXY(path, args):
	print ('set :', args)
	x, y, r, g = args
	matrixIN[x][y] = (r,g)

def setMatrix(path, args):
	layout = json.loads(args[0])
	print (layout)
	for x in range(0, 8):
		for y in range(0,8):
			if len(layout[x][y]) == 2:
				matrixIN[x][y] = (layout[x][y][0], layout[x][y][1])

def ok(path, args):
	print('yo')

server.add_method("/set", 'iiii', setXY)
server.add_method("/matrix", 's', setMatrix)

# create an instance
lp = launchpad.Launchpad();
lp.Open()
print("Launchpad Mk1/S/Mini")

# Clear the buffer because the Launchpad remembers everything :-)
lp.ButtonFlush()
lp.Reset()
butHit = 0
while 1:
	server.recv(100)

	# Main Matrix update
	for x in range(0,8):
		for y in range(0,8):
			if matrixIN[x][y] != matrixOUT[x][y]:
				lp.LedCtrlXY(x, y+1, matrixIN[x][y][0], matrixIN[x][y][1])
				matrixOUT[x][y] = matrixIN[x][y]
				#time.wait( 1 )

	but = lp.ButtonStateXY()
	if but != []:
		if but[2]:
			liblo.send(target, '/push', but[0], but[1])
		else:
			liblo.send(target, '/release', but[0], but[1])
		# print( but )
		# if butHit > 8:
		# 	break

lp.Reset() # turn all LEDs off
time.wait( 5 )
lp.Close() # close the Launchpad (will quit with an error due to a PyGame bug)
