import string
from hashlib import md5
from pwn import *

sock = remote('10.212.25.14', 25554)

buf = sock.recvline().decode()
eps = 1e-3
'''
sendlineafter
'''
def tryout(a):
    a = str(a)
    sock.recv()
    sock.send('1')
    sock.recvline()
    sock.send(a)
    res = float(sock.recvline().decode().replace('\n', '').split(' ')[-1])
    print(res)
    return int(res + eps)


print(buf)
buf = buf.replace('\n', '').replace('+', ' ').replace(')', '').split()

ans = buf[-1]
proof = buf[3]
dic = string.digits + string.ascii_letters
for i in dic:
    for j in dic:
        for k in dic:
            for l in dic:
                buf = i + j + k + l + proof
                if md5(buf.encode()).hexdigest() == ans:
                    sock.send(i + j + k + l)
                    break

print(sock.recvuntil('Can you find the flag through the calculating?').decode())
for i in range(5):
    buf = sock.recvline().decode()
    while ', ' not in buf:
        buf = sock.recvline().decode()
    data = []
    exec('data = ' + buf)

    print(data)
    token = ''
    for x in data:
        token += chr(tryout(x + 1))
    print(token)

    sock.recv()
    sock.send('2')
    print(sock.recv().decode())
    sock.send(token)
    print(sock.recv().decode())
    sock.send('')
    # sock.interactive()
    sock.send('')
    if i == 4:
        print(sock.recv().decode())
        sock.send('')
        print(sock.recv().decode())

print(sock.recv().decode())
