#include<stdio.h>
#include<stdlib.h>

// gcc -fno-stack-protector -no-pie -z lazy -o easyrop easyrop.c


int main(int argc, char const *argv[])
{
    setvbuf(stdin, 0, 2, 0);
    setvbuf(stdout, 0, 2, 0);

    alarm(30);
    char buf[0x40];
    read(0, buf, 0x100);
    return 0;
}