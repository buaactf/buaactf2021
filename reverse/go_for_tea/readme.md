

# Go for Tea

## 题目描述

Go语言逆向

附件：go_for_tea.exe

## 难度

中等

## 出题

源码：go_for_tea.go

flag格式： `flag{xxxxxxxx-xxxxxxxx-xxxxxxxx-xxxxxxxx}`

加密流程：

- 从flag中提取v0 v1 u0 u1
- 简单的伪随机数算法生成key
- 进行两次xtea加密

build:

`go build -ldflags "-s" cup_of_tea.go`

## 考点

Go语言去符号信息还原

XTEA加密算法的识别，及解密算法实现

## 官方WP

先还原符号化，通过寻找main进入主函数

由于go语言的特点，反编译的结果十分难看，可以结合动调以及汇编进行分析

`lea     rcx, aFlagXXXX  ; "flag{%x-%x-%x-%x}"` 可以看到输入的是四个十六进制数

输入后有几个判断，通过调试可以知道当输入了四个数字时，即可进入目标分支中

加密部分反编译代码如下

```c
  while ( v2 < 32 )
  {
    ++v2;
    v4 = ((v1 + ((16 * v1) ^ (v1 >> 5))) ^ (v3 + v7[v3 & 3])) + result;
    v1 += (v4 + ((16 * v4) ^ (v4 >> 5))) ^ (v3 + v7[((unsigned int)(v3 + 0x19521025) >> 11) & 3] + 0x19521025);
    v3 += 0x19521025;
    result = v4;
  }
```

是delta修改过的XTEA加密算法，其中 `v7` 显然为加密的key，利用动调直接去内存中找到具体的key

```
debug066:000000C000145E90 dd 68017634h
debug066:000000C000145E94 dd 5ADB5C5Dh
debug066:000000C000145E98 dd 9278D9D2h
debug066:000000C000145E9C dd 0FA3EEAA3h
```

最后通过输出字符串得知密文为 `509568428 972337464 1169251202 832286518`

写一个XTEA的解密即可获得flag

解密脚本：solve.cpp

最后按照格式连成flag即可
