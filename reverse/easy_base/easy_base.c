#include <stdio.h>
#include <string.h>
#include <sys/ptrace.h>
#define BASE64_PAD '='

int has_debugger()
{
    int debugger;
    if (ptrace(PTRACE_TRACEME, 0, NULL, NULL) == 0) {
        // return 0 means success, so no debugger is attached
        debugger = 0;
    }
    else {
        debugger = 1;
    }
    return debugger;
}



void __attribute__((constructor)) __libc_init();

char base58_table[] = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";

char base64_table[] = {
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
	'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
	'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
	'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
	'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
	'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
	'w', 'x', 'y', 'z', '0', '1', '2', '3',
	'4', '5', '6', '7', '8', '9', '+', '/',
};

char cipher[] = "ZmxhZ3tpYXlFTUdCTFIzVCJfIkwoIz0mWV5LL2ZKT3kwNWlzJy0ofQ==";

unsigned int base64_encode(const unsigned char *in, unsigned int inlen, char *out);

int main(){
    printf("Please input your flag:\n");        //flag{Th15_iS_TrU3_FL4G&&I_L0vE_y0u!2764}
    char in[50];
    char out[70];
    scanf("%45s", in);
    int len = strlen(in);
    if(len != 40) {
        printf("Wrong length!\n");
        return 0;
    }
    unsigned int out_len = base64_encode(in, len, out);
    for (int i = 0; i < out_len; ++i){
        if (out[i] != cipher[i]){
            printf("Wrong flag!\n");
            return 0;
        }
    }
    printf("Congratulations!!!\n");
    return 0;
}

unsigned int base64_encode(const unsigned char *in, unsigned int inlen, char *out) {
	int s;
	unsigned int i;
	unsigned int j;
	unsigned char c;
	unsigned char l;

	s = 0;
	l = 0;
	for (i = j = 0; i < inlen; i++) {
		c = in[i];

		switch (s) {
		case 0:
			s = 1;
			out[j++] = base64_table[(c >> 2) & 0x3F];
			break;
		case 1:
			s = 2;
			out[j++] = base64_table[((l & 0x3) << 4) | ((c >> 4) & 0xF)];
			break;
		case 2:
			s = 0;
			out[j++] = base64_table[((l & 0xF) << 2) | ((c >> 6) & 0x3)];
			out[j++] = base64_table[c & 0x3F];
			break;
		}
		l = c;
	}

	switch (s) {
	case 1:
		out[j++] = base64_table[(l & 0x3) << 4];
		out[j++] = BASE64_PAD;
		out[j++] = BASE64_PAD;
		break;
	case 2:
		out[j++] = base64_table[(l & 0xF) << 2];
		out[j++] = BASE64_PAD;
		break;
	}

	out[j] = 0;

	return j;
}

void __libc_init(){
    char trick[70];
 //   void* ppp = base64_table - 4;
 //   mprotect(base64_table-0x4,0x100, PROT_READ| PROT_WRITE);
    trick[0] = '!';
    trick[2] = '&';
    trick[1] = '@';
    trick[3] = '*';
    trick[4 + 48] = 'w';
    trick[4 + 49] = 'x';
    trick[4 + 50] = 'z';
    trick[4 + 51] = 'y';
    trick[4 + 52] = 'o';
    trick[4 + 53] = 'F';
    trick[4 + 54] = 'U';
    trick[4 + 55] = '3';
    trick[4 + 56] = 'P';
    trick[4 + 57] = 's';
    trick[4 + 58] = 'u';
    trick[4 + 59] = 'g';
    trick[4 + 60] = '1';
    trick[4 + 61] = '5';
    trick[4 + 62] = '/';
    trick[4 + 63] = 'H';
    trick[4 + 32] = 'b';
    trick[4 + 33] = 'h';
    trick[4 + 34] = 'a';
    trick[4 + 35] = 'e';
    trick[4 + 36] = 'k';
    trick[4 + 37] = 'd';
    trick[4 + 38] = 'm';
    trick[4 + 39] = '9';
    trick[4 + 40] = '8';
    trick[4 + 41] = 'D';
    trick[4 + 42] = 'G';
    trick[4 + 43] = 'j';
    trick[4 + 44] = 'R';
    trick[4 + 45] = 't';
    trick[4 + 46] = 'n';
    trick[4 + 47] = '4';
    trick[4 + 16] = 'Q';
    trick[4 + 17] = 'I';
    trick[4 + 18] = 'W';
    trick[4 + 19] = 'C';
    trick[4 + 20] = 'p';
    trick[4 + 21] = 'V';
    trick[4 + 22] = 'S';
    trick[4 + 23] = 'T';
    trick[4 + 24] = '0';
    trick[4 + 25] = 'Z';
    trick[4 + 26] = 'Y';
    trick[4 + 27] = 'E';
    trick[4 + 28] = 'c';
    trick[4 + 29] = 'N';
    trick[4 + 30] = '+';
    trick[4 + 31] = 'f';
    trick[4 + 0] = 'v';
    trick[4 + 1] = 'M';
    trick[4 + 2] = 'r';
    trick[4 + 3] = 'X';
    trick[4 + 4] = 'l';
    trick[4 + 5] = 'K';
    trick[4 + 6] = 'B';
    trick[4 + 7] = '2';
    trick[4 + 8] = '6';
    trick[4 + 9] = '7';
    trick[4 + 10] = 'O';
    trick[4 + 11] = 'q';
    trick[4 + 12] = 'L';
    trick[4 + 13] = 'J';
    trick[4 + 14] = 'i';
    trick[4 + 15] = 'A';

    //strncpy(base64_table - 4, trick, 68);



    /*
    asm("push 0x6(%rip)");
    asm("jmp junk1");
    */
    asm("leaq strcpy3,%rax");
    asm("push %rax");
    asm("jmp strcpy1");
//    asm("call junk1");
    asm("strcpy3:");
    if (has_debugger() == 0) {
        asm("movq $base64_table-0x4,%rcx");//dst
        asm("leaq -0x50(%rbp),%rax");//scr

        asm("movl $0x44,%edx");
        asm("movq %rax,%rsi");
        asm("movq %rcx,%rdi");

        asm("call strncpy");
    }
    asm("jmp strcpy2");
    asm("strcpy1:");
    asm("ret");
    asm("strcpy2:");
/* __asm {
        mov     edx, 0x44
        mov     rsi, rax
        mov     rdi, rcx
        call    _strncpy
    }*/
    return;
}