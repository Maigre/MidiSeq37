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
import time

import liblo
import mutex

receivedMatrix = None
lastProcessedMatrix = None
myLock = mutex.mutex()

# SEVER Thread
class MyServer(liblo.ServerThread):
    def __init__(self, port):
        liblo.ServerThread.__init__(self, port)

    @liblo.make_method('/set', 'iiii')
    def setXY(self, path, args):
		print ('set :', args)
		x, y, r, g = args
		matrixIN[x][y] = (r,g)

    @liblo.make_method('/matrix', 's')
    def setMatrix(self, path, args):
		dataInput = args[0]
		myLock.lock(copyRecv, dataInput)
		myLock.unlock()

def copyRecv(data):
	global receivedMatrix
	receivedMatrix = data

# create server, listening on port 9000
try:
    server = MyServer(9000)
except ServerError as err:
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

#print (json.dumps(matrixIN))


# create an instance
lp = launchpad.Launchpad();
lp.Open()
print("Launchpad Mk1/S/Mini")

# Clear the buffer because the Launchpad remembers everything :-)
lp.ButtonFlush()
lp.Reset()
butHit = 0

def parseMatrix(args):
	global receivedMatrix
	global lastProcessedMatrix
	if receivedMatrix != lastProcessedMatrix:
		layout = receivedMatrix
		for x in range(0, 8):
			for y in range(0,8):
				if len(layout) > 1:
					data = layout[:2]
					layout = layout[2:]
				matrixIN[x][y] = (data[:1], data[1:])
		lastProcessedMatrix = receivedMatrix

server.start()
while 1:

	myLock.lock(parseMatrix, None)
	myLock.unlock()

	# Main Matrix update
	for x in range(0,8):
		for y in range(0,8):
			if matrixIN[x][y] != matrixOUT[x][y]:
				lp.LedCtrlXY(x, y+1, matrixIN[x][y][0], matrixIN[x][y][1])
				matrixOUT[x][y] = matrixIN[x][y]

	time.sleep(0.03)
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
time.sleep( 2 )
lp.Close() # close the Launchpad (will quit with an error due to a PyGame bug)
