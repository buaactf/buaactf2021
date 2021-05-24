#include<stdio.h>
#include <stdlib.h>
#include<string.h>
// gcc -no-pie -z lazy -o pwn5 pwn5.c


int main(int argc, char const *argv[])
{
    setvbuf(stdin, 0, 2, 0);
    setvbuf(stdout, 0, 2, 0);

    alarm(30);

    char buf[0x20];
    puts("ROP is easy, but how do u bypass the canary~");
    puts("make a try!");
    read(0, buf, 0x30);
    puts("Airdrop is coming!");
    puts(buf);
    puts("It's time for you to show off");
    read(0, buf, 0x80);

    return 0;
}