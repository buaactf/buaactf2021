# !/usr/bin/env python3
from Crypto.Util.number import *
from hashlib import md5
import socketserver
import signal
import string
import random
import os

flag = 'flag{Die_java_r3tired_fr0m_BUAAACM}'
MENU = br'''[+] 1.function
[+] 2.check_answer
[+] 3.exit
'''


class Task(socketserver.BaseRequestHandler):
    def _recvall(self):
        BUFF_SIZE = 2048
        data = b''
        while True:
            part = self.request.recv(BUFF_SIZE)
            data += part
            if len(part) < BUFF_SIZE:
                break
        return data.strip()

    def send(self, msg, newline=True):
        try:
            if newline:
                msg += b'\n'
            self.request.sendall(msg)
        except:
            pass

    def recv(self, prompt=b'[-] '):
        self.send(prompt, newline=False)
        return self._recvall()

    def proof_of_work(self):
        random.seed(os.urandom(8))
        proof = ''.join([random.choice(string.ascii_letters + string.digits) for _ in range(20)])
        _hexdigest = md5(proof.encode()).hexdigest()
        self.send(f"[+] md5(XXXX+{proof[4:]}) == {_hexdigest}".encode())
        x = self.recv(prompt=b'[+] Plz tell me XXXX: ')
        if len(x) != 4 or md5(x + proof[4:].encode()).hexdigest() != _hexdigest:
            return False
        return True

    def function(self, x):
        res = 0
        for i in range(self.lenth):
            numerator = ord(self.token[i])
            denominator = x - self.data[i]
            try:
                tmp = numerator / denominator
            except Exception as e:
                self.send(b'[+] Error!')
                return
            res += tmp
        return res

    def handle(self):
        signal.alarm(1000)
        if not self.proof_of_work():
            return

        self.send(b'[+] Welcome!')
        self.send(b'[+] Can you find the flag through the calculating?')

        self.score = 0
        self.token = ''.join(random.sample(string.ascii_letters + string.digits, 16))
        self.lenth = len(self.token)
        self.data = []
        for i in range(self.lenth):
            self.data.append(getRandomInteger(32))
        self.send(str(self.data).encode())

        while True:
            self.send(MENU, newline=False)
            choice = self.recv()
            if (choice == b'1'):
                self.send(b"[+] Plz give me your x: ")
                now = int(self.recv().strip().decode())
                now = self.function(now)
                self.send(("[+] let me show you the answer: " + str(now)).encode())
            elif (choice == b'2'):
                guess = self.recv().strip().decode()
                if (guess == self.token):
                    self.score += 1
                    self.send(b"[+] You win!")
                    self.send(("[!] Now your score: " + str(self.score)).encode())

                    self.token = ''.join(
                        [random.choice(string.digits + string.ascii_letters) for i in range((self.score + 1) * 16)])
                    self.lenth = len(self.token)
                    self.data = []
                    for i in range(self.lenth):
                        self.data.append(getRandomInteger(32))
                    self.send(str(self.data).encode())

                    if (self.score >= 5):
                        self.send(flag.encode())
                else:
                    self.send(b'[+] What do you want to say???')
                    self.send(b'[!] Go away!')
                    break
            else:
                break

        self.request.close()


class ThreadedServer(socketserver.ThreadingMixIn, socketserver.TCPServer):
    pass


class ForkedServer(socketserver.ForkingMixIn, socketserver.TCPServer):
    pass


if __name__ == "__main__":
    HOST, PORT = '0.0.0.0', 25554
    server = ForkedServer((HOST, PORT), Task)
    server.allow_reuse_address = True
    server.serve_forever()
