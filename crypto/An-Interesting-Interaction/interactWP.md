# an Interesting Interaction WP

## 考点

简单的数学题

## WP

题目大意：

每关生成一个随机的token，token长度为$l$。会给用户长为$l$的随机数数组$data$，让你通过若干次交互猜出token。

交互过程是这样的，输入$x$，返回$f(x)$，其中$f(x)=\sum_{i=1}^l \dfrac{ord(token_i)}{x-data_i}$。

做法的话就是每次输入$data_i+1$再加个eps向下取整就是$ord(token_i)$了。

因为$data$数组都是32bit级别的，而$ord(token_i)$是8bit级别的。
$$
f(data_j+1)=\sum_{i=1}^l \dfrac{ord(token_i)}{data_j+1-data_i}=\sum_{i=1,i\neq j}^l \dfrac{ord(token_i)}{data_j-data_i}+ord(token_j)
$$
考虑那一坨分子，都是8bit，分母都是32bit的，分子除以分母约等于0，所以可以忽略不计，直接取整数部分即可。

（其实正常输出就会找到规律了）

- 78.0000004407198->78
- 85.00000090808578->85
- 73.99999895794052->74
- 106.99999216584122->107
- 81.99999673919476->82

## exp

```python
import string
from hashlib import md5
from pwn import *

sock = remote('10.212.25.14', 25554)

buf = sock.recvline().decode()
eps = 1e-3

def tryout(a):
    a = str(a)
    sock.recv()
    sock.send('1')
    sock.recvline()
    sock.send(a)
    res = float(sock.recvline().decode().replace('\n', '').split(' ')[-1])
    print(res)
    return int(res + eps)


print(buf)
buf = buf.replace('\n', '').replace('+', ' ').replace(')', '').split()

ans = buf[-1]
proof = buf[3]
dic = string.digits + string.ascii_letters
for i in dic:
    for j in dic:
        for k in dic:
            for l in dic:
                buf = i + j + k + l + proof
                if md5(buf.encode()).hexdigest() == ans:
                    sock.send(i + j + k + l)
                    break

print(sock.recvuntil('Can you find the flag through the calculating?').decode())
for i in range(5):
    buf = sock.recvline().decode()
    while ', ' not in buf:
        buf = sock.recvline().decode()
    data = []
    exec('data = ' + buf)

    print(data)
    token = ''
    for x in data:
        token += chr(tryout(x + 1))
    print(token)

    sock.recv()
    sock.send('2')
    print(sock.recv().decode())
    sock.send(token)
    print(sock.recv().decode())
    sock.send('')
    # sock.interactive()
    sock.send('')
    if i == 4:
        print(sock.recv().decode())
        sock.send('')
        print(sock.recv().decode())

print(sock.recv().decode())
```

