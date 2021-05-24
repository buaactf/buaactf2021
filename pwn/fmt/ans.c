#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
int main()
	{
	setvbuf(stdin, 0, 2, 0);
    	setvbuf(stdout, 0, 2, 0);
    	alarm(30);
	char buf[100];
	long long a = 0;
	long long b = 0;
	int fp = open("/dev/urandom", O_RDONLY);
	read(fp, &a, 2);
	read(fp, &b, 2);
	close(fp);
	long long num;
	puts("I have a gift, so who are you bro?");
	read(0, buf, 100);
	puts("hello~ it is a simple plus, but only lucky dogs can receive gift.");
	long long *num_p = &num;
	scanf("%lld", num_p);
	printf("let's guess");
	printf(buf);
	if (a + b == num){
	    puts("you are so lucky");
	    system("/bin/sh");
	}
	else{
	    puts("you are not lucky, see you next time~");
	}

	return 0;
}
//gcc -o 
