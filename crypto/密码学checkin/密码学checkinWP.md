# 密码学checkin 

## 考点

简单rsa

## writeup

看到n不大，上yafu分解或者到http://www.factordb.com/上分别得到p,q。

或者发现加密指数为3，直接枚举$c+k\times N$然后用gmpy2库开三次根也能得到明文。