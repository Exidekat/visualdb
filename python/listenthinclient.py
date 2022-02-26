#!/usr/bin/env python3

import argparse
import os
import queue
import sounddevice as sd
import vosk
import sys
import re
from datetime import datetime
import socket

HOST = "192.168.4.78"  # The server's hostname or IP address
PORT = 42069  # The port used by the server

q = queue.Queue()

def process(cleanResult):
    now = str(datetime.now())
    print(now + " : " + cleanResult)
    logFile = open(r"log.txt", "a")
    logFile.write(now + " : " + cleanResult + "\n")
    logFile.close()  
    queryFile = open(r"query.txt", "w+")
    queryFile.write(cleanResult)
    queryFile.close()

def int_or_str(text):
    """Helper function for argument parsing."""
    try:
        return int(text)
    except ValueError:
        return text

def callback(indata, frames, time, status):
    """This is called (from a separate thread) for each audio block."""
    if status:
        print("thisline,42,isaidsforsomereason.")#print(status, file=sys.stderr) #as of 2/22/2022, this line breaks shit for some reason
    q.put(bytes(indata))

parser = argparse.ArgumentParser(add_help=False)
parser.add_argument(
    '-l', '--list-devices', action='store_true',
    help='show list of audio devices and exit')
args, remaining = parser.parse_known_args()
if args.list_devices:
    print(sd.query_devices())
    parser.exit(0)
parser = argparse.ArgumentParser(
    description=__doc__,
    formatter_class=argparse.RawDescriptionHelpFormatter,
    parents=[parser])
parser.add_argument(
    '-f', '--filename', type=str, metavar='FILENAME',
    help='audio file to store recording to')
parser.add_argument(
    '-d', '--device', type=int_or_str,
    help='input device (numeric ID or substring)')
parser.add_argument(
    '-r', '--samplerate', type=int, help='sampling rate')
parser.add_argument(
    '-t', '--trigger', type=int, help='query trigger string to look for')
args = parser.parse_args(remaining)

try:
    if args.samplerate is None:
        device_info = sd.query_devices(args.device, 'input')
        # soundfile expects an int, sounddevice provides a float:
        args.samplerate = int(device_info['default_samplerate'])
        print(args.samplerate)

    if args.filename:
        dump_fn = open(args.filename, "wb")
    else:
        dump_fn = None

    with sd.RawInputStream(samplerate=args.samplerate, blocksize = 8000, device=args.device, dtype='int16',
                            channels=1, callback=callback):
            print('#' * 80)
            print('Listening to speech. Press Ctrl+C to stop the recording.')
            print('#' * 80)

            #rec = vosk.KaldiRecognizer(model, args.samplerate)
            with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
                print(f"connection attempt to {HOST}:{PORT}...")
                s.connect((HOST, PORT))
                s.setblocking(False)
                print(f"connected to {HOST}:{PORT}") 
                while True:
                    data = q.get()
                    s.sendall(data)
                    #print("frame sent")
                    try:
                        rdata = s.recv(1024)
                        if rdata is not None:
                            #print(rdata.decode('utf-8'))
                            process(rdata.decode('utf-8'))
                    except OSError as e:
                        pass
                    except Exception as e:
                        print(e)
            if dump_fn is not None:
                dump_fn.write(data)

except KeyboardInterrupt:
    print('\nDone')
    parser.exit(0)
except Exception as e:
    parser.exit(type(e).__name__ + ': ' + str(e))

