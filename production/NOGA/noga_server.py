#!/usr/bin/env python
# -*- coding: UTF-8 -*-

import serial
import string
import os
import time
import sys, traceback
import shutil
from platform import system
from datetime import date, datetime, timedelta

class todo():
    ''' TODO
    1.
    2.
    3.
    4.
    5.
    6.
    '''
    def __init__(self):
        print ("Not implemented!")

class serial_controller():
    def __init__(self, _name = ""):
        ''' init'''
        self.TIMEOUT = 1
        self.set_port_name(_name)
        self.set_port_baudrate(9600)
        self.ser = False
        # self.open_port()

    def open_port(self, _port_name = False):
        '''open port: default is already given in constructor, otherwise new one. '''
        if _port_name:
            self.set_port_name(_port_name)

        try:
            self.ser = serial.Serial(self.port_name, baudrate = self.port_baudrate, timeout = self.TIMEOUT)
            self.ser.flushInput()
            self.ser.flushOutput()
            # self.ser.writeTimeout = 0
            # self.ser.xonxoff=1
            self.ser.open()
            print ("Serial port", self.port_name, "is open")
        except serial.SerialException:
            b = 0
            # print ("SerialException. ERROR in open port", self.port_name)
            # sys.exit(0)
        except:
            print ("Couldn't read port %i. Traceback:" % _port_name)
            traceback.print_exc()
        #finally:
        #    print("Finally: try except block successfully executed")

    def isOpen(self):
        ''' return status of the current port'''
        if not self.ser:
            return False

        try:
            retVal = self.ser.isOpen()
        except:
            print ("ERROR in isOpen()", self.port_name)
            retVal = False

        return retVal

    def set_port_baudrate(self, _bdr):
        self.port_baudrate = _bdr

    def set_port_name(self, _port_name):
        if len(_port_name) == 0:
            _port_name = '/dev/ttyACM0'
        self.port_name = _port_name

    def ping_device(self):
        ''' send ping to currently open port, if any'''
        self.write_to_port("PING\r\n")

    def write_to_port(self, _to_port = 'A'):
        # print ("Enter into write_to_port...")
        # if not self.isOpen():
        #     print ("PORT NOT open! Return.")
        #     return False
        try:
            # print ("Before write to port...")
            a = self.ser.write(_to_port)
            a = self.ser.write('\r\n')
            # self.ser.flush()
            # print (_to_port, " - sent!")
        except:
            a = 1
            # self.ser.flush()
            # print ("ERROR in write to port", self.port_name)
            # self.close_port()

    def readline(self):
        ''' if port open read one line '''
        if not self.isOpen():
            return 'Nan'

        #return self.ser.readline()
        try:
            return self.ser.readline()
        except:
            print ("ERROR in readline, port", self.port_name)
            self.close_port()
            return 'Nan'


    def read(self):
        if not self.isOpen():
            return 0

        return self.ser.read()

    def close_port(self):
        if self.isOpen():
            try:
                self.ser.close()
                print ("Port", self.port_name, "is closed.")
            except:
                print ("ERROR in attempt to close port", self.port_name)

    def recv(self):
        return self.port.readline()


# Function to read from the serial port in a separate thread
def read_serial():
    while True:
        if ser.in_waiting:
            received_data = ser.readline().decode().strip()
            if received_data:
                print("Received data:", received_data)

def getOSname():
    current_os = system()
    if current_os == 'Linux':
        print ("OS - Linux")
    elif current_os == 'Windows':
        print ("OS - Windows")

    # main entrance point:
if __name__ == '__main__' or __name__ == sys.argv[0]:
    print ("Main program start.")
    _port_to_open = 'COM23'
    # _port_to_open = 'COM33'
    data_raw = "None"
    StopLoop = False

    isConEstablished = False

    getOSname()

    TIME_TO_RECONNECT = 5

    time_received = time.time()
    time_cur = time.time()
    time_ser = time.time()
    time_ping = time.time()
    time_check_critical = time.time()
    time_check_done = time.time()
    start = time.time()

    ping_sent_counter = 0

    # need to use follow command:
    # >python noga_server.py COMxx
    if len(sys.argv) >= 2:
       _port_to_open = str(sys.argv[1])
       # print _port_to_open
    else:
        print ("Default serial port to use: ", _port_to_open)

    serial_port = serial_controller(_port_to_open)

    print("NOGA controller started", time.strftime("%Y-%m-%d %H:%M:%S") )

    while not StopLoop:
        try:
            # get time:
            time_cur = time.time()

            # print("Check if port is open")
            if (serial_port.isOpen()):
                # print("OK - port is open")
                data_raw = serial_port.readline()
                if data_raw:
                    print ("Client response: ")
                    print (data_raw)
            else:
                # print("ERR - port NOT open")
                # check time delay and try to open port again:
                serial_port.close_port()
                # print ("Port not open... Try to open it") # TODO: port name to print?
                serial_port.open_port()
                # isConEstablished = False
            #end if

            user_input = input("Enter command to send (or ':q' to quit): ")
            if user_input.lower() == ':q':
                break
            # Send the user input data to the serial port
            serial_port.write_to_port(user_input.encode())

            time.sleep(0.1) # delay to receive response from client.

            # if "(1)" in data_raw:
            #    elapsed = (time.time() - start)
            #    time_received = time.time()
            #    isConEstablished = True
            #    ping_received_counter = ping_received_counter + 1
            # elif "(2)" in data_raw:
            #    print ("---------------------\n")
            #    time_received = time.time()
            # elif "PING" in data_raw:
            #    print "-INFO: Ping received-\n"
            # elif "INFO" in data_raw:
            #     print data_raw


            # status = r"rec: %3.2f sent: %3.2f  Total: [%3.2f MB]" % (self.bytes_rec, self.bytes_sen, self.total)
            #status = status + chr(8)*(len(status)+1)

            # connection control:
            if (time_cur - time_received > 600):
                #
                print ("Too long time no info!", time_cur - time_received)
                time_received = time.time() + 3600 # add 60 minutes to wait?
                serial_port.close_port()
                _msg_to_print = "Noga disconnected at: " + str(datetime.now())
            # end_if

        except KeyboardInterrupt:
            print ("\nCtrl+C detected! Abort the mission.")
            StopLoop = True
    #end_while

    serial_port.close_port()
    #endif
    print ("Main program end.")


