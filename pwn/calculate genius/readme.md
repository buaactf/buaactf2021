#  calculate genius

这题由于程序中有对时间的限制, 如果人工计算很难在规定时间完成, 提示大家学会使用pwntools工具与服务器进行交互, 教程如下

https://bbs.pediy.com/thread-247217.htm



通过ida反编译得知, 已知a和b的值, 连续答对20次随机加减乘的结果,即可拿到shell, 每次a和b的数字还有运算符都会给出, 利用pwntools中的recv和send函数拿到shell

![g58XYq.png](https://z3.ax1x.com/2021/05/19/g58XYq.png)

~~~python
from pwn import *
p = process('./math')
# p = remote('10.212.25.14', 19145) # 远程连接
context(os='linux', arch='amd64', log_level='debug') # 开启debug模式,会有输入输出的回显

for i in range(20):
    p.recvuntil('numberA = :')
    a = int(p.recvuntil('\n'))
    p.recvuntil('numberB = :')
    b = int(p.recvuntil('\n'))
    log.info('a: ' + str(a))
    log.info('b: ' + str(b))
    op =  p.recvuntil(':')
    op = chr(op[-4])
    log.info('op: ' + op)
    if op == "*":
        p.sendline(str(a*b))
    elif op == "+":
        p.sendline(str(a+b))
    else:
        p.sendline(str(a-b))

p.interactive()
~~~

