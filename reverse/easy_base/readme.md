# easy_base

## 题目描述

换表Base64

附件：easy_base

## 题目难度

简单

## 出题过程

将换表部分藏到了init函数中，同时添加了反调试和花指令，增加了静态调试的难度，以此提倡选手使用动态调试。

源码：easy_base.c

## 官方WP

动态调试：

由于使用ptrace函数进行反调试，反调试程度不高，因此直接修改ptrace的返回值

随后找到Base表的内存地址，dump下来并进行Base64的求解即可

静态调试：

由于Linux系统中，ELF文件在调用main函数之前会先调用init函数，因此可以进入init函数中寻找换表格

从start进入_libc_csu_init，可以看到 `v4 = &_do_global_dtors_aux_fini_array_entry - &_frame_dummy_init_array_entry;`

即随后的for循环就是调用这两个地址之间的所有函数

进入该地址即可找到换表函数 `__libc_init`，dump下来并调一调顺序就可以，或者手动把花指令以及反调试去掉，然后再动调
