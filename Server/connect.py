## Standard Libaries
import os
import sys
import inquirer
import serial
import serial.tools.list_ports
import configparser
import datetime
import string
import logging

## Project Libraries
from inquirerOptions import *

# CONSTANTS
BAUD_RATE = 115200

# RUN SPECIFIC VARIABLES
cwd = os.getcwd
logFileFolder = str(cwd) + '/logs/'
if not os.path.exists(logFileFolder):
    os.makedirs(logFileFolder)

# FUNCTIONS

def processCommand(command):
    print('Processing Command', command)
    if command.get('MainMenu') == 'Quit':
        sys.exit()

def monitor():
    pass

def automaticTests():
    pass

def settings():
    pass

def quit():
    pass

def checkIfCompatible():
    if os.name  == 'nt':
        print("Windows is not supported yet. Sorry")
        sys.exit()

def main():
    checkIfCompatible()
    exit = False
    while not exit:
        command = inquirer.prompt(mainMenu)
        processCommand(command)
if __name__ == '__main__':
    main()