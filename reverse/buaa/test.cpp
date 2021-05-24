// test.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include <iostream>

#include <windows.h>
#include <winternl.h>
#include <tlhelp32.h>


#include <stdlib.h>
#include <setjmp.h>


using namespace std;

//#define SHOW_DEBUG_INFO 1


#ifdef SHOW_DEBUG_INFO
#define JUNKCODE5() asm volatile("nop")
#define JUNKCODE7() asm volatile("nop")
#define JUNKCODE9() asm volatile("nop")
#define JUNKCODE11() asm volatile("nop")
#else
#define JUNKCODE5()  asm volatile("mov $0x55555555,%eax")
#define JUNKCODE7()  asm volatile("mov $0x77777777,%eax;inc %eax;inc %eax;")
#define JUNKCODE9()  asm volatile("mov $0x99999999,%eax;inc %eax;inc %eax;inc %eax;inc %eax;")
#define JUNKCODE11() asm volatile("mov $0xAAAAAAAA,%eax;mov $0xBBBBBBBB,%eax;inc %eax;")
#endif // SHOW_DEBUG_INFO

#define DRX_BP_OFFSET 0x1000
#define MAIN_CODE_LEN 0x80C
#define DEBUG_CODE_LEN 0x620

//#define ror(num,n)  ((num)>>(n))|((num)<<(32-(n)))

/// ---BigNum struct start---
#define BIG_NUM_BYTE_COUNT 0x100

typedef struct _BigNum
{
	int nLen;
	unsigned char bData[BIG_NUM_BYTE_COUNT];
} BigNum;


typedef struct _MyFlag
{
	BigNum n;
	BigNum e;
	BigNum plain;
	BigNum result;
	BigNum mulresult;
	BigNum flag;
	BigNum cipher;
} MyFlag;
MyFlag mf;
/// ---BigNum struct end---
char rsa_n[] = "3D1AC4A3AA4CE61D0FC58ACFE27DFC1382122F92EEF4DF86323F593B51B7B1A9";
int rsa_e = 3;
//char rsa_plain[] = "5646F636B6E657A6F55646F636C6C6568637F576572656469647E616F5163727";
char rsa_cipher[] = "DC3F91EEA2D395082B7730F4294CA5E1C82C04DFA92C85A82A4EB05A4D82B94";

char sbase[] = ".0123456789:!ABCDEFGHIJKLMNOPQRSTUVWXYZ";

jmp_buf mark;

int ShowStrByNumBase(unsigned int num);
unsigned int StrToNum(char* s, int len);

int main();

/// ---BigNum functions declear start---
int BigNumInitStr(BigNum *num, char* s, int slen);
int BigNumInitNum(BigNum* numOut, int n);

int BigNumCmp(BigNum *num1, BigNum *num2);
int BigNumSub(BigNum *numOut, BigNum *num1, BigNum *num2);
int BigNumMul(BigNum *numOut, BigNum *num1, BigNum *num2);
int BigNumMod(BigNum *numOut, BigNum *num1, BigNum *num2);
int BigNumModInt(BigNum *num1, int num2);
int BigNumShl(BigNum *numOut, BigNum *num1, int num2);
int BigNumShr(BigNum *numOut, BigNum *num1, int num2);
/// ---BigNum functions declear end---


/// ---BigNum functions content start---
void BigNumPrint(BigNum* t, bool bRev = false)
{
	int i, l;
	l = t->nLen;
	cout << "t[" << l << "] = ";
	char c, d;
	if (bRev)
	{
		for (i = l - 1; i >= 0; i--)
		{
			c = t->bData[i] >> 4;
			c += (c < 10 ? 48 : 55);
			d = t->bData[i] & 0x0F;
			d += (d < 10 ? 48 : 55);
			cout << c << d << ' ';
		}
	}
	else
	{
		for (i = 0; i < l; i++)
		{
			c = t->bData[i] >> 4;
			c += (c < 10 ? 48 : 55);
			d = t->bData[i] & 0x0F;
			d += (d < 10 ? 48 : 55);
			cout << c << d << ' ';
		}
	}
	cout << endl;
}

void BaseConv(BigNum *num, char* sbase)
{
	int i, len, k;
	char cc[BIG_NUM_BYTE_COUNT] = { 0 };
	int bb[BIG_NUM_BYTE_COUNT] = { 0 };
	int ans[BIG_NUM_BYTE_COUNT] = { 0 };
	int olds = 256, news = strlen(sbase);

	if (olds == news) return;

	len = num->nLen;
	k = 0;
	for (i = 0; i < len; i++) bb[i] = num->bData[i];
	bb[len] = 0; //set high bit before to 0
	while (len)
	{
		for (i = len; i >= 1; --i)
		{
			bb[i - 1] += bb[i] % news * olds;
			bb[i] /= news;
		}
		ans[k++] = bb[0] % news;
		bb[0] /= news;
		while (len > 0 && !bb[len - 1]) --len;
	}
	for (i = 0; i < k; i++) cc[i] = sbase[ans[i]];
	cout << "new str[" << k << "] = " << cc << endl;
}

int BigNumInitStr(BigNum *num, char* s, int slen)
{
	int i, k = 0;
	int bb[BIG_NUM_BYTE_COUNT + BIG_NUM_BYTE_COUNT] = { 0 };
	char c;
	int n;

	if (slen > BIG_NUM_BYTE_COUNT + BIG_NUM_BYTE_COUNT) return 0;

	for (i = 0; i < slen; i++) {
		c = s[i];
		if (c >= 'A' && c <= 'F') n = c - 'A' + 10;
		else if (c >= '0' && c <= '9') n = c - '0';
		else return -1;
		bb[i] = n;
		k++;
	}
	num->nLen = (k + 1) >> 1;
	if (k % 2 == 1)
	{
		num->bData[k >> 1] = bb[k - 1];
		k--;
	}
	for (i = 0; i < k; i += 2)
		num->bData[i >> 1] = (bb[i + 1] << 4 | bb[i]);

	return num->nLen;
}

int BigNumInitBase(BigNum* num, char* s, int slen, char* sbase)
{
	int i, j, k;
	int baselen;
	int bb[BIG_NUM_BYTE_COUNT] = { 0 };
	unsigned char c;
	int news = 256;

	baselen = strlen(sbase);
	for (i = 0; i < slen; i++) {
		c = s[i];
		k = -1;
		for (j = 0; j < baselen; j++) {
			if (sbase[j] == c) {
				k = j;
				break;
			}
		}
		if (k < 0) return -1;
		bb[i] = k;
	}

	if (bb[slen - 1] == 0) return 0; //high bit cannot set to 0

	bb[slen] = 0; //set high bit before to 0
	k = 0;
	while (slen)
	{
		for (i = slen; i >= 1; --i)
		{
			bb[i - 1] += bb[i] % news * baselen;
			bb[i] /= news;
		}
		num->bData[k++] = bb[0] % news;
		bb[0] /= news;
		while (slen > 0 && !bb[slen - 1]) --slen;
	}
	if (k >= BIG_NUM_BYTE_COUNT) return -1;
	num->nLen = k;
	return k;
}

int BigNumInitNum(BigNum* numOut, int n)
{
	numOut->bData[sizeof(n)] = 0;
	memcpy(numOut->bData, &n, sizeof(n));
	int i = sizeof(n);
	while (i > 0 && numOut->bData[i - 1] == 0)
	{
		i--;
	}
	numOut->nLen = i;

	return i;
}

int BigNumCmp(BigNum *num1, BigNum *num2)
{
	int i;

	if (num1->nLen > num2->nLen) return 1;
	if (num1->nLen < num2->nLen) return -1;
	for (i = num1->nLen - 1; i >= 0; i--)
	{
		if (num1->bData[i] > num2->bData[i]) return 1;
		if (num1->bData[i] < num2->bData[i]) return -1;
	}
	return 0;
}


int BigNumSub(BigNum *numOut, BigNum *num1, BigNum *num2)
{
	int i, ll;
	int nn = 0;
	unsigned char bb[BIG_NUM_BYTE_COUNT] = { 0 };

	if (num1->nLen >= num2->nLen) ll = num1->nLen;
	else ll = num2->nLen;
	for (i = 0; i < ll; i++)
	{
		if (i < num1->nLen) nn += num1->bData[i];
		if (i < num2->nLen) nn -= num2->bData[i];
		bb[i] = nn & 0xFF;
		nn >>= 8;
	}
	while (nn && i < BIG_NUM_BYTE_COUNT)
	{
		bb[i++] = nn & 0xFF;
		nn >>= 8;
	}
	while (i > 0 && bb[i - 1] == 0)
	{
		i--;
	}
	numOut->nLen = i;
	memcpy(numOut->bData, bb, i);

	return i;
}

int BigNumMul(BigNum *numOut, BigNum *num1, BigNum *num2)
{
	int i, j, pi, pj;
	int nn = 0;
	unsigned char bb[BIG_NUM_BYTE_COUNT * 2] = { 0 };

	if (num1->nLen + num2->nLen > BIG_NUM_BYTE_COUNT + 1) {
		numOut->nLen = 0;
		numOut->bData[0] = 0;
		return num1->nLen + num2->nLen;
	}
	for (i = 0; i < num1->nLen + num2->nLen - 1; i++)//5:3:7 0-6
	{
		if (i < num1->nLen) pi = i; //i:pi 0 1 2 3 4 5 6:0 1 2 3 4 4 4   i-pi:0 0 0 0 0 1 2
		else pi = num1->nLen - 1;
		if (i < num2->nLen) pj = i; //i:pj 0 1 2 3 4 5 6:0 1 2 2 2 2 2   i-pj:0 0 0 1 2 3 4
		else pj = num2->nLen - 1;

		for (j = i - pj; j <= pi; j++)//num1:j:0-0 0-1 0-2 1-3 2-4 3-4 4-4
		{                            //num2:  0-0 1-0 2-0 2-0 2-0 2-1 2-2
			nn += (int)num1->bData[j] * (int)num2->bData[i - j];
		}                            //pi-j:  0-0 1-0 2-0 2-0 2-0 1-0 0-0
		bb[i] = nn & 0xFF;      //pi-j+i-pi:  0-0 1-0 2-0 2-0 2-0 2-1 2-2
		nn >>= 8;
	}
	while (nn)
	{
		bb[i++] = nn & 0xFF;
		nn >>= 8;
	}
	while (i > 0 && bb[i - 1] == 0)
	{
		i--;
	}
	if (i > BIG_NUM_BYTE_COUNT) {
		numOut->nLen = 0;
		numOut->bData[0] = 0;
	}
	else {
		memcpy(numOut->bData, bb, i);
		numOut->nLen = i;
	}
	return i;
}


int BigNumMod(BigNum *numOut, BigNum *num1, BigNum *num2)
{
	int i, c;
	unsigned char bb[BIG_NUM_BYTE_COUNT * 2] = { 0 };
	BigNum tt;

	i = BigNumCmp(num1, num2);
	if (i < 0)
	{
		numOut->nLen = num1->nLen;
		memcpy(&numOut->bData[0], &num1->bData[0], num1->nLen);
		return num2->nLen;
	}
	if (i == 0)
	{
		numOut->nLen = 0;
		numOut->bData[0] = 0;
		return 0;
	}
	c = num1->nLen - num2->nLen;
	tt.nLen = num2->nLen;
	memcpy(&tt.bData[0], &num1->bData[c], num2->nLen);
	for (i = c; i >= 0; i--)
	{
		while (BigNumCmp(&tt, num2) >= 0)
		{
			bb[i]++;
			BigNumSub(&tt, &tt, num2);
		}
		if (i > 0)
		{
			BigNumShl(&tt, &tt, 8);
			tt.bData[0] = num1->bData[i - 1];
			if (tt.nLen == 0 && tt.bData[0] > 0) tt.nLen = 1;
		}
	}
	memcpy(numOut->bData, &tt.bData[0], tt.nLen);
	numOut->nLen = tt.nLen;

	return tt.nLen;
}

int BigNumModInt(BigNum *num1, int num2)
{
	int i;
	int nn = 0;

	if (num2 <= 0) return 0;

	for (i = num1->nLen - 1; i >= 0; i--)
	{
		nn <<= 8;
		nn += num1->bData[i];
		if (nn > num2) nn %= num2;
	}

	return nn;
}

int BigNumShl(BigNum *numOut, BigNum *num1, int num2)
{
	int i, c, n;
	int nn = 0;
	unsigned char bb[BIG_NUM_BYTE_COUNT * 2] = { 0 };

	c = num2 >> 3;
	n = num2 % 8;
	if (c + num1->nLen > BIG_NUM_BYTE_COUNT)
	{
		numOut->nLen = 0;
		numOut->bData[0] = 0;
		return c + num1->nLen;
	}
	for (i = 0; ; i++)
	{
		if (i < num1->nLen) nn += (num1->bData[i] << n);
		bb[i + c] = nn & 0xFF;
		nn >>= 8;
		if (i >= num1->nLen && nn == 0) break;
	}
	i = c + num1->nLen + 1;
	while (i > 0 && bb[i - 1] == 0)
	{
		i--;
	}
	if (i > BIG_NUM_BYTE_COUNT) {
		numOut->nLen = 0;
		numOut->bData[0] = 0;
	}
	else {
		memcpy(numOut->bData, bb, i);
		numOut->nLen = i;
	}
	return i;
}

int BigNumShr(BigNum *numOut, BigNum *num1, int num2)
{
	int i, c, n, b;
	int nn = 0;
	unsigned char bb[BIG_NUM_BYTE_COUNT * 2] = { 0 };

	c = num2 >> 3;
	n = num2 % 8;
	b = (1 << n) - 1;
	for (i = num1->nLen - 1; i >= c; i--)
	{
		nn <<= 8;
		nn += num1->bData[i];
		bb[i - c] = nn >> n;
		nn &= b;
	}
	if (num1->nLen - c > 0) i = num1->nLen - c;
	else i = 0;
	while (i > 0 && bb[i - 1] == 0)
	{
		i--;
	}
	if (i > BIG_NUM_BYTE_COUNT) {
		numOut->nLen = 0;
		numOut->bData[0] = 0;
	}
	else {
		memcpy(numOut->bData, bb, i);
		numOut->nLen = i;
	}
	return i;
}
/// ---BigNum functions content end---


int ShowStrByNumBase(unsigned int num)
{
	int n;
	char c;


	n = strlen(sbase);
	
	while (num > 0) {
	
		c = sbase[num % n];
	
		num /= n;
	
		cout << c;
	
	}
	return 0;
}

unsigned int StrToNum(char* s, int len)
{
	
	unsigned int n = 0;
	
	int baselen = strlen(sbase);

	int i, k;

	for (i = 0; i < len; i++) {
		
		for (k = 0; k < baselen; k++)
			if (sbase[k] == s[i]) break;
		
		if (k >= baselen) return 0;
		
		n *= baselen;
	
		n += k;
		
	}

	return n;
}


int main()
{
	int slen;



	BigNumInitStr(&mf.n, rsa_n, strlen(rsa_n)); // n

	BigNumInitNum(&mf.e, rsa_e);//e

	//BigNumInitStr(&mf.plain, rsa_plain, strlen(rsa_plain)); // plain

	BigNumInitStr(&mf.cipher, rsa_cipher, strlen(rsa_cipher)); // cipher


	
	char rsa_flag[0x100] = { 0 };
	cout << "请输入序列号:";
	cin >> rsa_flag;
	cout << endl;
		
	slen = strlen(rsa_flag);
	if (slen != 64)
	{
		cout << "输入错误，重新试一下" << endl;
	}
	else
	{
		BigNumInitStr(&mf.flag, rsa_flag, strlen(rsa_flag)); // flag

		BigNumInitNum(&mf.mulresult, 1);



		for (int i = 0; i < 3; i++)
		{
			BigNumMul(&mf.mulresult, &mf.mulresult, &mf.flag);
		}
		BigNumMod(&mf.mulresult, &mf.mulresult, &mf.n);
		cout << "加密结果" << endl;
		BigNumPrint(&mf.mulresult, TRUE);
		cout << endl;

		if (!BigNumCmp(&mf.mulresult, &mf.cipher))
		{
			cout << "输入正确！！！" << endl;
		}
		else
		{
			cout << "输入错误，重新试一下" << endl;
		}
		
	}
	return 0;
