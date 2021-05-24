#include <stdio.h>
#include <windows.h>
#define _CRT_SECURE_NO_WARNINGS
#define one 	65535
#define round	8
#define maxim 	65537

unsigned mul(unsigned a, unsigned b) //算模乘法
{
	long int p;
	long unsigned q;
	if (a == 0) p = maxim - b;
	else
		if (b == 0) p = maxim - a;
		else {
			q = (unsigned long)a * (unsigned long)b;
			p = (q & one) - (q >> 16);
			if (p <= 0) p = p + maxim;
		}
	return (unsigned)(p & one);
}


void key(unsigned short uskey[9], unsigned short Z[7][10])
{
	unsigned short S[54];
	int i, j, r;
	for (i = 1; i < 9; i++) S[i - 1] = uskey[i];
	/* shifts */
	for (i = 8; i < 54; i++)
	{
		if ((i + 2) % 8 == 0)			/* for S[14],S[22],... */
			S[i] = ((S[i - 7] << 9) ^ (S[i - 14] >> 7)) & one;
		else if ((i + 1) % 8 == 0)			/* for S[15],S[23],... */
			S[i] = ((S[i - 15] << 9) ^ (S[i - 14] >> 7)) & one;
		else
			S[i] = ((S[i - 7] << 9) ^ (S[i - 6] >> 7)) & one;
	}

	/* get subkeys */

	for (r = 1; r <= round + 1; r++)
		for (j = 1; j < 7; j++)
			Z[j][r] = S[6 * (r - 1) + j - 1];
}

void cip(unsigned short in[5], unsigned short out[5], unsigned short Z[7][10])
{
	unsigned r, x1, x2, x3, x4, kk, t1, t2, a;
	x1 = in[1]; x2 = in[2]; x3 = in[3]; x4 = in[4];
	for (r = 1; r <= 8; r++) 			/* the round function */
	{
		/* the group operation on 64-bits block */
		x1 = mul(x1, Z[1][r]);		x4 = mul(x4, Z[4][r]);//算模乘法 65537
		x2 = (x2 + Z[2][r]) & one;	x3 = (x3 + Z[3][r]) & one;  //算模加法65536
				/* the function of the MA structure */
		kk = mul(Z[5][r], (x1 ^ x3));
		t1 = mul(Z[6][r], (kk + (x2 ^ x4)) & one);
		t2 = (kk + t1) & one;
		/* the involutary permutation PI */
		x1 = x1 ^ t1;		x4 = x4 ^ t2;
		a = x2 ^ t2;		x2 = x3 ^ t1;	x3 = a;
	}

	/* the output transformation */
	out[1] = mul(x1, Z[1][round + 1]);
	out[4] = mul(x4, Z[4][round + 1]);
	out[2] = (x3 + Z[2][round + 1]) & one;
	out[3] = (x2 + Z[3][round + 1]) & one;
}

int main() {
	WORD wSN[5] = {0};
	WORD wSubKey[7][10] = { 0 };
	WORD wiDeaKey[9] = {0, 0x1155, 0x6654, 0x4332, 0x2155, 0x4433, 0x2554, 0x4332, 0x1155};
	char input[15];
	WORD wCipherText[5] = { 0 };
	bool flag = 0;

	printf("Do you hear the Beep sing?\n");
	fflush(stdin);
	scanf_s("%s", input, 15);
//	printf("%s\n",input);

	if (strncmp(input, "flag{",5) != 0) {
		printf("Wrong input!");
		return 1;
	}
	else if (input[13] != '}') {
		printf("Wrong input!");
		return 1;
	}


	for (int i = 1; i < 5; i++) {
		wSN[i] = input[(i - 1) * 2 + 5] << 8 | input[(i - 1) * 2 + 1 + 5];
	}
	key(wiDeaKey, wSubKey);
	cip(wSN, wCipherText, wSubKey);

	for (int i = 1; i < 5; i++) {
		switch (wCipherText[i]) {
		case 0x0eac:{		//5544332
			Beep(831, 500);
			Beep(831, 500);
			Beep(740, 500);
			Beep(740, 500);
			Beep(682, 500);
			Beep(682, 500);
			Beep(622, 500);
			break;
			}
		case 0x37bf: {	//1155665
			Beep(554, 500);
			Beep(554, 500);
			Beep(831, 500);
			Beep(831, 500);
			Beep(932, 500);
			Beep(932, 500);
			Beep(831, 500);
			break;
		}
		case 0x523d:{		//44332215544332
			Beep(740, 500);
			Beep(740, 500);
			Beep(682, 500);
			Beep(682, 500);
			Beep(622, 500);
			Beep(622, 500);
			Beep(554, 500);
			Beep(831, 500);
			Beep(831, 500);
			Beep(740, 500);
			Beep(740, 500);
			Beep(682, 500);
			Beep(682, 500);
			Beep(622, 500);
			break;
		}
		case 0x76a3: {	//11556i54433221
			Beep(554, 500);
			Beep(554, 500);
			Beep(831, 500);
			Beep(831, 500);
			Beep(932, 500);
			Beep(1109, 500)
			Beep(831, 500);
			Beep(740, 500);
			Beep(740, 500);
			Beep(682, 500);
			Beep(682, 500);
			Beep(622, 500);
			Beep(622, 500);
			Beep(554, 500);
			break;
		}
		default:
			if (flag == 0) {
				printf("No No No. Did you hear the voice?\n");
				flag = 1;
			}
			break;	
		}
	}

}