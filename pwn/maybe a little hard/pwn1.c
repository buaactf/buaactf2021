#include<stdio.h>
// gcc -fno-stack-protector -z execstack -no-pie -z now -o pwn1 pwn1.c

char p[200];

void welcome(){
    puts("               .__                                ___.                               __    _____                    ");
    puts("__  _  __ ____ |  |   ____  ____   _____   ____   \\_ |__  __ _______  _____    _____/  |__/ ____\\___________  ______");
    puts("\\ \\/ \\/ // __ \\|  | _/ ___\\/  _ \\ /     \\_/ __ \\   | __ \\|  |  \\__  \\ \\__  \\ _/ ___\\   __\\   __\\/ __ \\_  __ \\/  ___/");
    puts(" \\     /\\  ___/|  |_\\  \\__(  <_> )  Y Y  \\  ___/   | \\_\\ \\  |  // __ \\_/ __ \\\\  \\___|  |  |  | \\  ___/|  | \\/\\___ \\ ");
    puts("  \\/\\_/  \\___  >____/\\___  >____/|__|_|  /\\___  >  |___  /____/(____  (____  /\\___  >__|  |__|  \\___  >__|  /____  >");
    puts("             \\/          \\/            \\/     \\/       \\/           \\/     \\/     \\/                \\/           \\/ ");

}

int vul(){
    char buf[0x100];
    return read(0, buf, 0x110);
}

int main(int argc, char const *argv[])
{
    setvbuf(stdin, 0, 2, 0);
    setvbuf(stdout, 0, 2, 0);

    alarm(30);

    welcome();
    puts("shellcode is 1nter3sting.");
    vul();
    return 0;
}