#!/usr/bin/env python3
from pwn import *
p = process('./math')
# p = remote('10.212.25.14', 19145) # 远程连接
context(os='linux', arch='amd64', log_level='debug') # 开启debug模式,会有输入输出的回显

for i in range(20):
    p.recvuntil('numberA = :')
    a = int(p.recvuntil('\n'))
    p.recvuntil('numberB = :')
    b = int(p.recvuntil('\n'))
    log.info('a: ' + str(a))
    log.info('b: ' + str(b))
    op =  p.recvuntil(':')
    op = chr(op[-4])
    log.info('op: ' + op)
    if op == "*":
        p.sendline(str(a*b))
    elif op == "+":
        p.sendline(str(a+b))
    else:
        p.sendline(str(a-b))

p.interactive()
