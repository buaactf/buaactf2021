#include<stdio.h>
#include <stdlib.h>
#include<string.h>
// gcc -fno-stack-protector -z noexecstack -no-pie -z now -o pwn2 pwn2.c

int crazyread(){
    char buf[0x400];
    puts("All LOLers know the will of 7");
    puts("Salute to Clearlove");
    read(0, buf, 0x200);
    return salute(buf);
}

int salute(char *s){
  char dest[11];
  char l;

  l = strlen(s);
  if ( l == 7 )
  {
    puts("Seven!");
    strcpy(dest, s);
  }
  else
  {
    puts("Invalid!");
  }
  return 0;
}

void backdoor(){
    system("/bin/sh");
}

int main(int argc, char const *argv[])
{
    setvbuf(stdin, 0, 2, 0);
    setvbuf(stdout, 0, 2, 0);

    alarm(30);

    char buf[0x25];
    memset(buf,0 ,0x20);
    puts("Do u know how to bypass strlen?");
    read(0, buf, 0x20);
    if(strlen(buf) > 10)
        exit(0);
    if(buf[16] == 77)
        crazyread();

    return 0;
}