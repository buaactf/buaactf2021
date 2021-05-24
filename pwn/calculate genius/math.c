#include <stdlib.h>
#include <stdio.h>
#include <time.h> 
#include <stdlib.h>
#include <unistd.h>

int main(){
	setvbuf(stdin, 0, 2, 0);
    setvbuf(stdout, 0, 2, 0);
    alarm(30);
	
	int i, a, b, ans, cal, cnt = 0;
	srand((unsigned)time( NULL ) );
	int date = 3939;
	for (i = 0; i < 20; i ++){
		printf("round%d\n", i);	
		a = abs(rand() % date);
       		b = abs(rand() % date);
		printf("numberA = :%d\n", a);
        	printf("numberB = :%d\n", b);
		cal = abs(rand() % 3);
		if (cal == 0){
			printf("please tell me answer a + b:");
			scanf("%d", &ans);
			if (ans == a + b){
				printf("right\n");
				cnt ++;
			}
			else{
				printf("wrong\n");
			}
			printf("\n");
		}
		else if(cal == 1){
			printf("please tell me answer a * b:");
                        scanf("%d", &ans);
                        if (ans == a * b){
                                printf("right\n");
                                cnt ++;
                        }
                        else{
                                printf("wrong\n");
                        }
			printf("\n");
		}
		else if(cal == 2){
                        printf("please tell me answer a - b:");
                        scanf("%d", &ans);
                        if (ans == a - b){
                                printf("right\n");
                                cnt ++;
                        }
                        else{
                                printf("wrong\n");
                        }
                        printf("\n");
                }
	}	
	if (cnt > 19)
		system("/bin/sh");

	return 0;
}
