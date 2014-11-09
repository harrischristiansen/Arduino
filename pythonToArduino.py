import serial
import time

ser = serial.Serial('/dev/tty.usbmodem1421', 57600)
time.sleep(3)
ser.write("100 400 700 950")