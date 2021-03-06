
#include <stdlib.h>
#include <time.h>
#include "longdiv.h"
#include <string.h>
#include <stdio.h>
#include "RSA.h"

#ifndef RSA_h
#define RSA_h

#define MAXNUMBER ((number)(-1))

#define LENGTH_BITS 256 //length of number 
#define LENGTH_2BYTES LENGTH_BITS / 16 //length 2-õáàéòîâûõ ÷èñåë

typedef unsigned short number;
typedef unsigned long twonumber;

#define LOWORD(T)((unsigned short)(T))
#define HIWORD(T)((unsigned short)(T>>16))
#define MAKELONG(a,b) ((((unsigned long long int)(a))<<16)+b)

//äëèíà ÷èñëà à
int LengthLong(number a[]) {
	int i = LENGTH_2BYTES - 1;
	while ( i >= 0 && a[i] == 0 ) 
		i--;
	return i + 1;
}

//çàíóëèòü à ëèíîé äî 16 2-õáàéòîâûõ ñëîâ
void ZeroLong(number a[]) {
    int i;
    for (i = 0; i < LENGTH_2BYTES; i++) {
		a[i] = 0;
	}
}

//ïðèñâîåíèå ÷èñëó à ÷èñëî b
void AssignLong(number a[], number b[]) {
	ZeroLong(a);
    int i;
    for (i = 0; i < LENGTH_2BYTES; i++) {
		a[i] = b[i];
	}
}

//c = a + b
static void SumLong(number a[], number b[], number c[], number *per) {
	unsigned short d = 0;
	unsigned int T = 0;
	int i = 0;
	for (i = 0; i < LENGTH_2BYTES; i++) {
		T = (unsigned long)a[i] + (unsigned long)b[i] + d;
		c[i] = LOWORD(T);
		d = HIWORD(T);
	}
	c[LENGTH_2BYTES] = d;
	*per = d;
}

//c = a - b
static void DiffLong(number a[], number b[], number c[]) {
	unsigned short d = 0;
	unsigned int T = 0;
	int i = 0;
	for (i = 0; i < LENGTH_2BYTES; i++) {
		T = (unsigned long)a[i] - (unsigned long)b[i] - d;
		c[i] = LOWORD(T);
		if (HIWORD(T) == 0) d = 0;
		else d = 1;
	}
}

// 1 åñëè à>b, -1 åñëè a<b, 0 åñëè a=b
int CmpLong(number a[], number b[]) {
	int i = 0;
	int s = LengthLong(a);
	int t = LengthLong(b);
	if (s > t) return 1;
	if (s < t) return -1;

	for (i = s - 1; i >= 0; i--) {
		if (a[i] > b[i]) return 1;
		if (a[i] < b[i]) return -1;
	}
	return 0;
}

//ñãåíåðèðîâàòü ñë÷àéíîå a
void RandLong(number a[], int s) {

	int i = 0;
    //srand( time( NULL ) );
	for (i = 0; i < s; i++) {
		a[i] = LOWORD(rand() % 256) + LOWORD(rand() << 8);
	}
}

//c = a * x
static void ShortMultLong(number a[], number c[], number x) {
	int T = 0;
	int n = LengthLong(a);
	int i = 0;
	for (i = 0; i < n; i++) {
		T = a[i] * x + HIWORD(T);
		c[i] = LOWORD(T);
	}
	c[i] = HIWORD(T);
}

//c = a * b
static void MultLong(number a[], number b[], number c[]) {
	unsigned short d = 0;
	unsigned int T = 0;
	int i = 0;
	int j = 0;

	for (i = 0; i < 2 * LENGTH_2BYTES; i++) {
		c[i] = 0;
	}

	for (i = 0; i < LengthLong(a); i++) {
		d = 0;
		for (j = 0; j < LengthLong(b); j++) {
			T = (unsigned long)c[i + j] + (unsigned long)a[i] * (unsigned long)b[j] + (unsigned long)d;
			c[i + j] = LOWORD(T);
			d = HIWORD(T);
		}
		c[i + j] = d;
	}
}

//c = a^2
static void SquareLong(number a[], number c[]) {
	number tmp[LENGTH_2BYTES];
	AssignLong(tmp, a);
	MultLong(a, tmp, c);
}

//ïå÷àòü ÷èñëà
void PrintfLong(number a[]) {
	int i = 0;
	i = LENGTH_2BYTES - 1;
	//if (i == -1) printf("%04x ", a[0]);
	for (i; i >= 0; i--) {
		printf("%04x ", a[i]);
	}
	printf("\n");
}

static void ShiftLong(number a[], number k) {
	int i = 0;
	i = LengthLong(a) - 1;

	for (i; i >= 0; i--) {
		a[i + k] = a[i];
	}
	for (i = 0; i < k; i++) {
		a[i] = 0;
	}
}

// c = a div x, r = a mod x
static void ShortDivLong(number a[], number x, number c[], number *r) {
	int n = LengthLong(a);
	int i = 0;
	ZeroLong(r);
	twonumber T = 0;
	if (x == 0) return;

	for (i = n - 1; i >= 0; i--) {
		T <<= sizeof(number) * 8;
		T |= a[i];
		c[i] = LOWORD(T / x);
		T %= x;
	}
	*r = LOWORD(T);
}

// c = a*b mod n
void MulModLong(number a[], number b[], number n[], number c[]) {
	number d[2 * LENGTH_2BYTES];
	number q[2 * LENGTH_2BYTES];
	ZeroLong(c);
	MultLong(a, b, d);
	Div(d, n, q, c, LENGTH_2BYTES*2, LengthLong(n));
}

// c = a^2 mod n
void SquareModLong(number a[], number n[], number c[]) {
	number d[LENGTH_2BYTES * 2];
	number tmp[LENGTH_2BYTES * 2];
	SquareLong(a, d);
	Div(d, n, tmp, c, LENGTH_2BYTES*2, LengthLong(n));
}

// c = a^b mod p
void PowerModBinary(number a[], number b[], number mod[], number result[]) {
	
	int s = LengthLong(b)*LENGTH_2BYTES;
	int i = 0;
	int n = LengthLong(mod);
	number tmp[LENGTH_2BYTES];
	ZeroLong(tmp);
	ZeroLong(result);
	result[0] = 0x0001;
	for (i = s - 1; i >= 0; i = i-1) {
		SquareModLong(result, mod, tmp);
		AssignLong(result, tmp);
		if ((b[i / 16] >> (i % 16)) & 1) {
			MulModLong(a, result, mod, tmp);
			AssignLong(result, tmp);
		}
	}
}

//ïîäïèñü òåêñòà m
void SignatureRSA(number m[], number close_key_d[], number open_key_n[],number result[]) {
	PowerModBinary(m, close_key_d, open_key_n, result);
}

//ïðîâåðêà ïîäïèñè, 1-ïðîéäåíà, -1-íå ïðîéäåíà
int VerificationSignatureRSA(number m[], number s[], number open_key_e[], number open_key_n[]) {
	number m1[LENGTH_2BYTES];
	number m2[LENGTH_2BYTES];
	number tmp[LENGTH_2BYTES];

	ZeroLong(m1);
	ZeroLong(m2);
	ZeroLong(tmp);
	Div( m, open_key_n, tmp, m2, LENGTH_2BYTES, LENGTH_2BYTES );
	PowerModBinary(s, open_key_e, open_key_n, m1);
	if (CmpLong(m2, m1) == 0) return 1;
	return -1;
}

//a*x+b*y = Íîä(a,b) = d
static void ExtendedEuklidLong(number a[], number b[], number x[], number y[], number d[]) {
	number q[LENGTH_2BYTES];
	number r[LENGTH_2BYTES];
	number x1[LENGTH_2BYTES];
	number x2[LENGTH_2BYTES];
	number y1[LENGTH_2BYTES];
	number y2[LENGTH_2BYTES];
	number nul[LENGTH_2BYTES];
	number tmpa[LENGTH_2BYTES];
	number tmpb[LENGTH_2BYTES];
	number tmp[LENGTH_2BYTES*2];
	number per[1] = { 0x0000 };

	ZeroLong(x);
	ZeroLong(y);
	ZeroLong(d);
	ZeroLong(q);
	ZeroLong(r);
	ZeroLong(x1);
	ZeroLong(x2);
	ZeroLong(y1);
	ZeroLong(y2);
	ZeroLong(nul);
	ZeroLong(tmp);
	
	AssignLong(tmpa, a);
	AssignLong(tmpb, b);

	if (CmpLong(b, nul) == 0) {
		AssignLong(d, a);
		ZeroLong(x);
		x[0] = 0;
		ZeroLong(y);
		return;
	}

	x2[0] = 0x0001;
	y[1] = 0x0001;

	while (CmpLong(tmpb, nul) == 1) {
		ZeroLong(tmp);
		Div(tmpa, tmpb, q, r, LengthLong(tmpa), LengthLong(tmpb));
		MultLong(q, x1, tmp);
		DiffLong(x2, tmp, x);
		ZeroLong(tmp);
		MultLong(q, y1, tmp);
		DiffLong(y2, tmp, y);
		AssignLong(tmpa, tmpb);
		AssignLong(tmpb, r);
		AssignLong(x2, x1);
		AssignLong(x1, x);
		AssignLong(y2, y1);
		AssignLong(y1, y);
	}
	AssignLong(d, tmpa);
	AssignLong(x, x2);
	AssignLong(y, y2);
}

//c - îáðàòíûé ê a ïî ìîäóëþ n, (ðàáîòàåò ñ îøèáêàìè èç çà áåççíàêîâûõ ÷èñåë)
static void InverseLong(number a[], number n[], number c[]) {
	number d[LENGTH_2BYTES];
	number x[LENGTH_2BYTES];
	number y[LENGTH_2BYTES];
	number one[LENGTH_2BYTES];
	number tmp[LENGTH_2BYTES];
	number per[1] = { 0x0001 };

	ZeroLong(d);
	ZeroLong(x);
	ZeroLong(y);
	ZeroLong(one);
	ZeroLong(tmp);
	one[0] = 0x0001;

	ExtendedEuklidLong(a, n, x, y, d);
	if (CmpLong(d, one) == 0) { 
		Div(x, n, y, tmp, LengthLong(x), LengthLong(n));
		AssignLong(c, tmp);
		return;
	}
	ZeroLong(c);
}

//òåñò íà ïðîñòîòó, åñëè 1 - òåñò ïðîéäåí, åñëè -1 - íå ïðîéäåí
int TestMilleraRabina(number a[]) {
	number two = 0x0002;
	number one[LENGTH_2BYTES];
	number nul[LENGTH_2BYTES];
	number tmp_a[LENGTH_2BYTES];
	number tmp[LENGTH_2BYTES+1];
	number tmp2[LENGTH_2BYTES];
	number res[LENGTH_2BYTES];
	number rand_numb[LENGTH_2BYTES];
	number x[LENGTH_2BYTES];
	number a_1[LENGTH_2BYTES]; //a_1 = a-1
	int s = 0; //a-1 = r * 2^s
	int i = 0;
	int j = 0;

	ZeroLong(one);
	one[0] = 0x0001;
	ZeroLong(nul);
	ZeroLong(res);
	ZeroLong(rand_numb);

	ZeroLong(tmp);
	ZeroLong(tmp_a);
	AssignLong(a_1, a);
	a_1[0] = a_1[0] - 1;

	AssignLong(tmp_a, a_1);
	/**/
	while (CmpLong(res,nul)==0) {
		
		ShortDivLong(tmp_a, two, tmp, res);
		if (CmpLong(res,nul)==0) {
			AssignLong(tmp_a, tmp);
			s = s + 1;
		}
	}
#if 0
	if (s == 0) { 
		SumLong(a, one, tmp, res);
		AssignLong(a, tmp);
		return -1;
	}
#endif

	ZeroLong(tmp);

	for (j = 0; j < 100; j++) {
		RandLong(rand_numb, LengthLong(tmp_a));
		Div(rand_numb, tmp_a, tmp2, tmp, LengthLong(rand_numb), LengthLong(tmp_a));
		AssignLong(rand_numb, tmp);
		PowerModBinary(rand_numb, tmp_a, a, x);
		if ((CmpLong(x, one) == 0) | (CmpLong(x, a_1)==0)) {
			continue;
		}
		for (i = 0; i < s; i++) {
			SquareModLong(x, a, tmp);
			AssignLong(x, tmp);
			if (CmpLong(x, one) == 0) return -1;
			if (CmpLong(x, a_1) == 0) break;
		}
		if (i == s) return -1;
	}
	return 1;
}

//ãåíåðàöèÿ ïðîñòîãî ÷èñëà a, çàäàííîé äëèíû
void GenPrimeNumber(number a[], int Length_a) {
	RandLong(a, Length_a);
	number two[LENGTH_2BYTES];
	number tmp[LENGTH_2BYTES*2];
	ZeroLong(two);
	ZeroLong(tmp);
	two[0] = 0x0002;
	number r[1] = { 0 };

	if( a[0] % 2 == 0 )
	{
		a[0]++;
	}
	while (TestMilleraRabina(a)!=1)
	{
		SumLong(a, two, tmp, r);
		AssignLong(a, tmp);
	}
}

//ãåíåðàöèÿ îòêðûòîã è çàêðûòîãî êëþ÷åé
void GenKeys(number open_key_e[], number open_key_n[], number close_key_d[]) {
	number p[LENGTH_2BYTES];
	number q[LENGTH_2BYTES];
	number e[LENGTH_2BYTES];
	number d[LENGTH_2BYTES];
	number n[LENGTH_2BYTES * 2];
	number Euler_n[LENGTH_2BYTES*2];
	number tmp[LENGTH_2BYTES];
	number tmp1[LENGTH_2BYTES];
	number one[LENGTH_2BYTES];
	number prover[LENGTH_2BYTES * 2+1];


	ZeroLong(p);
	ZeroLong(q);
	ZeroLong(e);
	ZeroLong(d);
	ZeroLong(Euler_n);
	ZeroLong(tmp);
	ZeroLong(tmp1);
	ZeroLong(one);
	one[0] = 0x0001;
	ZeroLong(prover);
	ZeroLong(open_key_e);
	ZeroLong(open_key_n);
	ZeroLong(close_key_d);

	GenPrimeNumber(p, LENGTH_2BYTES / 2);
	GenPrimeNumber(q, LENGTH_2BYTES / 2);
	MultLong(p, q, n);
	AssignLong(open_key_n, n);
	
	DiffLong(p, one, tmp);
	
	AssignLong(p, tmp);
	
	DiffLong(q, one, tmp);
	AssignLong(q, tmp);
	MultLong(p, q, Euler_n);

	while (CmpLong(prover, one) != 0) {
		RandLong(e, LENGTH_2BYTES);
		Div(e, Euler_n, tmp1, tmp, LengthLong(e), LengthLong(Euler_n));
		AssignLong(e, tmp);
		InverseLong(e, Euler_n, d);
		MulModLong(e, d, Euler_n, prover);
	}
	AssignLong(open_key_e, e);
	AssignLong(close_key_d, d);
	
}

void NumberToStr(number a[],char *res) {
	int i = LENGTH_2BYTES - 1;
	int j = 0;
	for (i, j = 0; i >= 0; i--, j+=5) {
		sprintf(&res[j], "%04x ", a[i]);
	}
}

void StrToNumber(char *a, number res[]) {
	int i = LENGTH_2BYTES -1;
	int j = 0;
	int k = 0;
	for (i, j; i >= 0; i--, j += 5) {
		sscanf(&a[j], "%04x", &k);
		res[i] = (number)k;
	}
	return;
}

void HashFunction(char *c, number res[]) {
	int s = strlen(c);
	//printf("%d \n", s);
	int i = 0;
	ZeroLong( res );
	for (i = 0; i < s - 1; i++)
	{
		if( c[i] == '\n' )
		{
			break;
		}
		res[i%16] += (number)c[i]*(i + 1);
	}
}

#endif /*RSA_h*/
//1505Ooo

