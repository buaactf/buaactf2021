# bzb's gift WP

## 考点

格密码，格基规约

## WP

题意是给一个$64$维向量$\{a_0,...,a_{63}\}$和$y$,求解$64$维向量$\{x_0,...,x_{63}\}$，使得$\sum_{i=0}^{63}a_ix_i=y$，其中$x_i<256$

很显然能发现是密码学课上提到的Knapsack问题，只不过前者问题是求解01序列，这个是求解一个更大的序列，在做法上我们只需要把每个$a_i$拆成8个数，分别是$a_i*128,a_i*64,...a_i$，变成求解512维向量的背包问题了。

对于Knapsack问题的做法，可以参考的是`《Lattice Reduction Attack on the Knapsack》Mark Stamp`的论文，核心就是构造一个如下的矩阵，然后对$M$矩阵进行`LLL algorithm`的变换之后，会得到一个$M'$矩阵，我们只需在这个新矩阵找到全为01的列便是答案了。

$$
MV = \begin{bmatrix}
   I_{n*n} & 0_{n*1} \\
   A_{n*1} & -y_{1*1} \\
  \end{bmatrix} 
  \begin{bmatrix}
   X_{n*1} \\
   1_{1*1} \\
  \end{bmatrix}=
  \begin{bmatrix}
   X_{n*1} \\
   0_{1*1} \\
  \end{bmatrix}=W
$$
故先构造出$M$矩阵，然后直接调用sage的`LLL`函数得到矩阵$M'$，大概需要处理5分钟的时间，得到序列后与enc进行异或就能得到flag了。

## exp

先sage

```python
x = []
X = []
with open('data', 'rb') as fr:
    for i in range(64):
        x.append(int(fr.readline().strip().split()[-1]))
    y = int(fr.readline().strip().split()[-1])

for i in range(64):
    for j in range(8):
        X.append(x[i]<<(7-j))

n = len(X)

M = matrix(ZZ, n+1, n+1)

for i in range(n):
    M[i, i] = 1
for i in range(n):
    M[i, n] = X[i]
M[n, n] = -int(y)

M = M.LLL() 

for i in range(0, n + 1):
    List = M.row(i).list()
    flag = True
    for item in List:
        if item != 0 and item != 1:
            flag = False
            break
    if flag:
        with open('answer.txt', 'wb') as fw:
            fw.write((' '.join(map(str, List)) + '\n').encode())


```

后python

```python
A = []
with open('answer.txt', 'rb') as fr:
    X = list(map(int,fr.readline().strip().split()))[:-1]
    for i in range(64):
        temp = 0
        for j in range(8):
            id = i*8 + j
            temp += (X[id]<<(7-j))
        A.append(temp)

with open('enc', 'rb') as f1:
    X = list(map(int, f1.readline().strip().split()))

flag = ''
for i in range(64):
    flag+=(chr(X[i]^A[i]))
print(flag)

```

