import socket
import vosk
import sys
import argparse
import os
import queue
import json

if __name__ == "__main__":

    #os.chdir("D:\\crack\\")

    q = queue.Queue()


    def process(result, sock):
        # indexStart = result.find('"text" : ')
        # indexEnd = result.find('"}')
        # cleanResult = result[indexStart + 10:indexEnd - 2]
        # if (cleanResult == "" or cleanResult == "the"):
        #     # print('Iritanc, begon!')
        #     return
        # now = str(datetime.now())
        # print(now + " : " + cleanResult)
        # logFile = open(r"log.txt", "a")
        # logFile.write(now + " : " + cleanResult + "\n")
        # logFile.close()
        # trigger = ["vtb", "v d b"]

        j = json.loads(result)
        s = j["text"]
        print(j)
        if not s or s == "the": return
        sock.sendall(s.encode('utf-8'))



    def int_or_str(text):
        """Helper function for argument parsing."""
        try:
            return int(text)
        except ValueError:
            return text


    def callback(indata, frames, time, status):
        """This is called (from a separate thread) for each audio block."""
        if status:
            print("thisline,42,isaidsforsomereason.")  # print(status, file=sys.stderr) #as of 2/22/2022, this line breaks shit for some reason
        q.put(bytes(indata))


    parser = argparse.ArgumentParser(add_help = False)
    args, remaining = parser.parse_known_args()

    parser = argparse.ArgumentParser(
        description = __doc__,
        formatter_class = argparse.RawDescriptionHelpFormatter,
        parents = [parser])
    parser.add_argument(
        '-f', '--filename', type = str, metavar = 'FILENAME',
        help = 'audio file to store recording to')
    parser.add_argument(
        '-m', '--model', type = str, metavar = 'MODEL_PATH',
        help = 'Path to the model')
    parser.add_argument(
        '-r', '--samplerate', type = int, help = 'sampling rate')
    parser.add_argument(
        '-t', '--trigger', type = int, help = 'query trigger string to look for')
    args = parser.parse_args(remaining)

    try:
        if args.model is None:
            args.model = "model"

        if not os.path.exists(args.model):
            print("Please download a model for your language from https://alphacephei.com/vosk/models")
            print("and unpack as 'model' in the current folder.")
            parser.exit(0)
        if args.samplerate is None:
            args.samplerate = 44100

        model = vosk.Model(args.model)

        if args.filename:
            dump_fn = open(args.filename, "wb")
        else:
            dump_fn = None

        rec = vosk.KaldiRecognizer(model, args.samplerate)

        ADDR = ("0.0.0.0", 42069)
        lastBit = ""
        while True:
            with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
                sock.bind(ADDR)
                sock.listen()
                print(f"Listening on {ADDR[0]}:{ADDR[1]}")

                conn, addr = sock.accept()
                print(f"Connected to {addr}")
                while True:
                    data = conn.recv(1024)
                    if not data: break;

                    if rec.AcceptWaveform(data):
                        process(rec.Result(), conn)
                    else:
                        p = json.loads(rec.PartialResult())["partial"]
                        if p != lastBit:
                            print(p)
                            lastBit = p
                       
                    if dump_fn is not None:
                        dump_fn.write(data)

    except KeyboardInterrupt:
        print('\nDone')
        parser.exit(0)
    except Exception as e:
        parser.exit(type(e).__name__ + ': ' + str(e))