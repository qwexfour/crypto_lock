#include "stdafx.h"
#include "stdlib.h"
#include "time.h"
#include "longdiv.h"
#include <stdlib.h>

#define MAXNUMBER ((number)(-1))

#define LENGTH_BITS 256 //длина числа в битах
#define LENGTH_2BYTES LENGTH_BITS / 16 //длина 2-хбайтовых чисел

typedef unsigned short number;
typedef unsigned long twonumber;

#define LOWORD(T)((unsigned short)(T))
#define HIWORD(T)((unsigned short)(T>>16))
#define MAKELONG(a,b) ((((unsigned long long int)(a))<<16)+b)

//длина числа а
int LengthLong(number a[]);

//занулить а длиной до 16 2-хбайтовых слов
void ZeroLong(number a[]);

//присвоение числу а число b
void AssignLong(number a[], number b[]);

// 1 если а>b, -1 если a<b, 0 если a=b
int CmpLong(number a[], number b[]);

//печать числа до 256 бит
void PrintfLong(number a[]);

//сгенерировать слчайное a
void RandLong(number a[], int s);

// c = a*b mod n
void MulModLong(number a[], number b[], number n[], number c[]);

// c = a^2 mod n
void SquareModLong(number a[], number n[], number c[]);

// c = a^b mod p
void PowerModBinary(number a[], number b[], number mod[], number result[]);

//подпись текста m
void SignatureRSA(number m[], number close_key_d[], number open_key_n[], number result[]);

//проверка подписи, 1-пройдена, -1-не пройдена
int VerificationSignatureRSA(number m[], number s[], number oped_key_e[], number open_key_n[]);

//генераци€ простого числа a, заданной длины
void GenPrimeNumber(number a[], int Length_a);

//генераци€ открытого и закрытого ключей 
void GenKeys(number open_key_e[], number open_key_n[], number close_key_d[]);