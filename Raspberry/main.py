import serial
import datetime
from collections import OrderedDict
from firebase import firebase
from uuid import getnode as get_mac

# Connecting to firebase
firebase = firebase.FirebaseApplication('https://brilliant-heat-7159.firebaseio.com', None)

while True:

	try:
		
		# If arduino is undefined
		try:
			arduino
		except NameError:
			arduino = serial.Serial('/dev/ttyUSB0', 9600)
		
		if 'SETUP' in arduino.readline():
			
			# Getting the HOSTNAME 
			hostName = arduino.readline()[:-2].replace('HOSTNAME ', '')
			
			# If HOSTNAME not referenced, it will be the mac address of the py
			if not hostName:
				hostName = ':'.join(("%012X" % get_mac())[i:i+2] for i in range(0, 12, 2))
				
			print 'Acquisition started'

		datasToSend = []
		
		while arduino.isOpen():
			
			# Reading input buffer of Serial Port
			dataRead = arduino.readline()
			
			# Beggining of transmission : Clear the datasToSend list
			if "BEGIN" in dataRead:
				datasToSend[:] = []
				
			# End of transmission : Format the list in a Dictionnary and send it to the firebase
			elif "END" in dataRead:

				dataDico = {}
				dataDico["_TIMESTAMP"] = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
				
				for data in datasToSend:
					dataName = data.split(' ')[0]
					dataValue = data.split(' ')[1]
					
					dataDico[dataName] = dataValue
				
				# Ordering dictionnary
				dataDico = OrderedDict(sorted(dataDico.items()))
				
				print dataDico
				firebase.post('/'+ hostName, dataDico)
				
			# If it's not a signal of Start/Beggining, it's a data.
			# Adding it to the list
			else:
				datasToSend.append(dataRead[:-2])
			
	except serial.SerialException:
		
		# If we unplug/replug the arduino, ttyUSB0 will not appear anymore in TTY devices but the USB manager
		# will address ttyUSB1 on replug, thinking ttyUSB0 is still used
		try:
			arduino = serial.Serial('/dev/ttyUSB0', 9600)
		except serial.SerialException:
			try:
				arduino = serial.Serial('/dev/ttyUSB1', 9600)
			except serial.SerialException:
				print("Arduino not connected !")