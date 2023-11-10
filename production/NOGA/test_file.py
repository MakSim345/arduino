#!/usr/bin/env python
# -*- coding: UTF-8 -*-

import serial
import string
import os
import time
from sms_rus import sendSMS
from do_web_page import web_page_generator
import sys, traceback
import shutil
from platform import system
from datetime import date, datetime, timedelta

sys.path.insert(0, '/home/ys/Python/telega_bot')
from send_one_message import *

class todo():
    ''' TODO
    1. Check minimum allowed temperature for Frizer and for Basement
    2. Add mail feature.
    3. Add monitoring feature: use narod.ru
    4. Add monitoring feature by demand: Dropbox or mail request.
    5. Add default COM port name for Win32 or POSIX (os_selection)
    6. Add light sensor, to see if lights on or off in basement"
    '''
    def __init__(self):
        print "Not implemented!"

class temperature_obj():
    '''a structure for keep a name and current temperature of an object'''
    def __init__(self, _new_name = ''):
        ''' init class'''
        self.set_name(_new_name)
        self.flag_min_temp = False
        self.flag_max_temp = False
        self.flag_first_time = True
        self.gross_temp = 0
        self.gross_ctr = 0
        self.min_temp_avr = 0
        self.max_temp_avr = 0
        self.avr_temp = 0
        self.set_cur_temp(0)
        self.flag_first_time = True # need for obtain real data

    def set_name(self, _new_name):
        self.name = _new_name

    def set_cur_temp(self, _new_temperature):
        self.cur_temp = float(_new_temperature)
        self.average_calculation()

    def average_calculation(self):
        ''' calculate average, set min and max values'''
        if (self.flag_first_time):
            self.max_temp_avr = self.cur_temp
            self.min_temp_avr = self.cur_temp
            self.flag_first_time = False
        else:
            self.gross_temp = self.gross_temp + self.cur_temp
            self.gross_ctr = self.gross_ctr + 1
            self.avr_temp = self.gross_temp / self.gross_ctr

        if (self.cur_temp > self.max_temp_avr):
            self.max_temp_avr = self.cur_temp

        if (self.cur_temp < self.min_temp_avr):
            self.min_temp_avr = self.cur_temp


    def get_name(self):
        return self.name

    def get_avr_temp(self):
        return self.avr_temp

    def get_max_temp(self):
        return self.max_temp_avr

    def get_min_temp(self):
        return self.min_temp_avr

    def get_cur_temp(self):
        return self.cur_temp

    def set_critical_temp(self, _min_temp = '', _max_temp= ''):
        if len(str(_min_temp)) > 0:
            self.min_temp = _min_temp
            self.flag_min_temp = True

        if len(str(_max_temp)) > 0:
            self.max_temp = _max_temp
            self.flag_max_temp = True

    def check_critical_temperature(self):
        if self.flag_max_temp:
            if (float(self.cur_temp) > self.max_temp):
                print self.name, "max:", self.cur_temp, " > ", self.max_temp
                return True

        if self.flag_min_temp:
            if (float(self.cur_temp) < self.min_temp):
                print self.name, "min:", self.cur_temp, " < ", self.min_temp
                return True

        return False

class serial_controller():
    def __init__(self, _name = ""):
        ''' init'''
        self.TIMEOUT = 2
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
            #self.ser = serial.Serial(self.port_name, baudrate = self.port_baudrate, writeTimeout = 0)
            self.ser.flushInput()
            self.ser.flushOutput()
            # self.ser.writeTimeout = 0
            # self.ser.xonxoff=1
            self.ser.open()
            print "Serial port", self.port_name, "is open"
        except serial.SerialException:
            print "ERROR in open port", self.port_name
            # sys.exit(0)
        except:
            print "Couldn't read port %i. Traceback:" % _port_name
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
            print "ERROR in isOpen()", self.port_name
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
        if not self.isOpen():
            return False
        try:
            # print "Before PING sending..."
            a = self.ser.write(_to_port)
            # self.ser.flush()
            # print "PING sent!"
        except:
            print "ERROR in write to port", self.port_name
            self.close_port()

    def readline(self):
        ''' if port open read one line '''
        if not self.isOpen():
            return 'Nan'

        #return self.ser.readline()
        try:
            return self.ser.readline()
        except:
            print "ERROR in readline, port", self.port_name
            self.close_port()
            return 'Nan'


    def read(self):
        if not self.isOpen():
            return 0

        return self.ser.read()

    def close_port(self):
        if not self.isOpen():
            print "Port", self.port_name, "was not open"
        else:
            try:
                self.ser.close()
                print "Port", self.port_name, "is closed."
            except:
                print "ERROR in attempt to close port", self.port_name

    def recv(self):
        return self.port.readline()


def get_tmp_from_raw(raw_data_array):
    '''Get the temperature from a string'''
    # if len(raw_data_array)
    start_p = 29
    shift_p = -4
    # temperature_v = raw_data_array[start_p:start_p+shift_p]
    temperature_v = raw_data_array[start_p:shift_p]
    return temperature_v

def send_alarm_mail(subj, msg):
    sender =  sendSMS('YANDEX', subj)
    #sender.set_subj_for_mail(subj)
    result = sender.send_info_sms(msg)

def send_alarm_telegram(subj, msg):
    send_message_ys(subj + ":  " + msg)

def move_html_to_git(file_to_copy):
    #TODO: made it selected based on  - DONE
    #TODO: copy result file to a folder in bitbucket and commit.
        # C:\dev\web_temp
        # /home/yuriy/web_tmp
    current_os = system()
    if current_os == 'Linux':
        str_path_on_os = '/home/ys/web_tmp'
        # in Linux file need to be copy to bitbucket web folder:
        log_file_name = str_path_on_os + file_to_copy
        shutil.copy (file_to_copy, log_file_name)
        #Goto destination folder and push to bitbucket:
        os.chdir(str_path_on_os)
        os.system('pwd')
        os.system('git s')
        os.system("git add .")
        _time = time.strftime("%H:%M:%S")
        os.system("git commit -m 'update temperature " + _time + "'")
        os.system("git push")
        #os.system('python commit_me.py')

    elif current_os == 'Windows':
        #str_path_on_os = 'C:/dev/Python/arduino_temperature_read/'
        str_path_on_os = 'C:\dev\web_temp'
        return # no action, index.html is here.
    else:
        print "Error in move_html_to_git(): no OS selected"

def getOSname():
    current_os = system()
    if current_os == 'Linux':
        print "OS - Linux"
    elif current_os == 'Windows':
        print "OS - Windows"

    # main entrance point:
if __name__ == '__main__' or __name__ == sys.argv[0]:
    print "Main program start."
    print ""
    _port_to_open = "COM11"
    data_raw = "None"
    StopLoop = False

    isConEstablished = False

    # str_path_on_os = select
    getOSname()

    TIME_TO_RECONNECT = 5

    time_received = time.time()
    time_cur = time.time()
    time_ser = time.time()
    time_ping = time.time()
    time_check_critical = time.time()
    time_web_page_done = time.time()
    start = time.time()

    update_time_interval = 10 # initially 10 seconds, for first update. Then up to 1 hour.
    ping_sent_counter = 0
    ping_received_counter = 1 # initial  will be 1

    web_page_gen = web_page_generator()
    web_page_gen.set_connected(False) # show on web page that Arduino disconnected.

    basement = temperature_obj("  Basement:")
    freezer = temperature_obj ("  Freezer: ")

    basement.set_critical_temp(1, 30)
    #freezer.set_critical_temp(1, 30)
    freezer.set_critical_temp(-29, -17)

    # log = logEngine()
    # log.saveMessageToLog("Arduino modem control - init OK.")

    # need to use follow command:
    # >python iot_basement_temperature COM11
    if len(sys.argv) >= 2:
       _port_to_open = str(sys.argv[1])
       # print _port_to_open
    else:
       print "No given serial port. Try to use COM11"
       _port_to_open = "COM11"

    serial_port = serial_controller(_port_to_open)

    send_alarm_telegram("Basement controller started", time.strftime("%Y-%m-%d %H:%M:%S") )
    while not StopLoop:
        try:
            # get time:
            time_cur = time.time()

            if (serial_port.isOpen()):
                data_raw = serial_port.readline()
                # print "RECEIVED: ", data_raw
            else:
                # check time delay and try to open port again:
                if ((time_cur - time_ser) > 5):
                    serial_port.close_port()
                    print "Port not open... Try to open it"
                    serial_port.open_port()
                    time_ser = time.time()
                #end if    
                isConEstablished = False

            if ((time_cur - time_ping) > 10):
                serial_port.ping_device()
                ping_sent_counter = ping_sent_counter + 1
                # print "PING sent:", ping_sent_counter, ", RESPONSED:", ping_received_counter
                print "Lost PING:", ping_sent_counter - ping_received_counter
                time_ping = time.time()
            #endif ping

            # data_raw = serial_port.read()
            # status = data_raw + chr(8)*(len(data_raw)+1)
            #if data_raw == 'Nan':
            #    print "Port not open..."

            if "(1)" in data_raw:
                elapsed = (time.time() - start)
                print "  Time:", time.strftime("%Y-%m-%d %H:%M:%S") # datetime.now()
                uptime_value = str(timedelta(seconds=elapsed))[:-7]
                print "  Uptime:", uptime_value
                # here print data from Arduino sensors:
                basement.set_cur_temp(get_tmp_from_raw(data_raw))
                print basement.get_name(), basement.get_cur_temp(), "*C [",
                print basement.get_min_temp(), "..", basement.get_max_temp(), "]:",
                print "%.2f" % basement.get_avr_temp()
                web_page_gen.set_uptime(uptime_value)
                web_page_gen.set_temp_basement(basement.get_cur_temp(),
                                               basement.get_min_temp(),
                                               basement.get_max_temp(),
                                               basement.get_avr_temp())
                time_received = time.time()
                web_page_gen.run()
                isConEstablished = True
                ping_received_counter = ping_received_counter + 1
            elif "DALLAS" in data_raw:
                freezer.set_cur_temp(get_tmp_from_raw(data_raw))
                print freezer.get_name(), float(freezer.get_cur_temp()), "*C [",
                print freezer.get_min_temp(), "..", freezer.get_max_temp(), "]:",
                print "%.2f" % freezer.get_avr_temp()
                web_page_gen.set_temp_freezer(freezer.get_cur_temp(),
                                              freezer.get_min_temp(),
                                              freezer.get_max_temp(),
                                              freezer.get_avr_temp())
                print "---------------------\n"
                web_page_gen.run()
                time_received = time.time()
            # elif "PING" in data_raw:
            #    print "-INFO: Ping received-\n"
            # elif "INFO" in data_raw:
            #     print data_raw


            # status = r"rec: %3.2f sent: %3.2f  Total: [%3.2f MB]" % (self.bytes_rec, self.bytes_sen, self.total)
            #status = status + chr(8)*(len(status)+1)

            #temperature control every XX sec:
            if ((isConEstablished) and (time_cur - time_check_critical > 3600)):
                # print "Check critical temperatures:"
                time_check_critical = time.time() # add 10 minutes to wait if SMS sent?

                if (basement.check_critical_temperature()):
                    subj_for_mail = 'Basement temperature alert!'
                    str_warning_message = "Basement temperature is critical: " + str(basement.get_cur_temp())
                    #send_alarm_mail(subj_for_mail, str_warning_message)
                    send_alarm_telegram(subj_for_mail, str_warning_message)

                if (freezer.check_critical_temperature()):
                    subj_for_mail = 'Freezer temperature alert!'
                    str_warning_message = "Freezer temperature is critical: " + str(freezer.get_cur_temp())
                    # send_alarm_mail(subj_for_mail, str_warning_message)
                    send_alarm_telegram(subj_for_mail, str_warning_message)

            #endif
            #endif

            # connection control:
            if (time_cur - time_received > 600):
                #
                print "Too long time no info!", time_cur - time_received
                time_received = time.time() + 3600 # add 60 minutes to wait?
                serial_port.close_port()
                web_page_gen.set_connected(False) # show on web page that Arduino disconnected.
                _msg_to_sms = "Arduino disconnected at: " + str(datetime.now())
                print _msg_to_sms
                print "SMS sending to ...."
                subj_for_mail = 'Basement info'
                # 2019-SEP: YS - remove sending alarm, due to port inactivity
                # send messsage:
                # send_alarm_mail(subj_for_mail, _msg_to_sms)

                # 2020-MAY: YS - add telegram messaging instead of e-mail
                send_alarm_telegram(subj_for_mail, _msg_to_sms)
            # end_if

            # create web page in certain interval of time:
            if (time_cur - time_web_page_done > update_time_interval):
                update_time_interval = 1800 #1800 = 0.5 hr.
                # update_time_interval = 900 #900 = 15 min.
                web_page_gen.run()
                time_web_page_done = time.time()
                move_html_to_git(web_page_gen.get_file_name())
            # end_if

            #if not self._silent_mode:
            #    print status,
            #i = i + 1
            #if i > 10:
            #    StopLoop = True
        except KeyboardInterrupt:
            print "\nKey was pressed! Abort the mission."
            StopLoop = True
    #end_while

    serial_port.close_port()
    #endif
    print ""
    print "Main program end."


