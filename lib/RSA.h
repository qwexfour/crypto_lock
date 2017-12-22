
#include <stdlib.h>
#include <time.h>
#include "longdiv.h"

#define MAXNUMBER ((number)(-1))

#define LENGTH_BITS 256 //length of number bits
#define LENGTH_2BYTES LENGTH_BITS / 16 //length of 2-bytes numbers

typedef unsigned short number;   //usage: number a[ LENGTH_2BYTES ];
typedef unsigned long twonumber;

#define LOWORD(T)((unsigned short)(T))
#define HIWORD(T)((unsigned short)(T>>16))
#define MAKELONG(a,b) ((((unsigned long long int)(a))<<16)+b)

//length of number à
int LengthLong(number a[]);

void ZeroLong(number a[]);

//a = b
void AssignLong(number a[], number b[]);

// compare, 1 - a>b, -1 - a<b, 0 - a=b
int CmpLong(number a[], number b[]);

//print number before 256 bits
void PrintfLong(number a[]);

//generation rand a
void RandLong(number a[], int s);

// c = a*b mod n
void MulModLong(number a[], number b[], number n[], number c[]);

// c = a^2 mod n
void SquareModLong(number a[], number n[], number c[]);

// c = a^b mod p
void PowerModBinary(number a[], number b[], number mod[], number result[]);

//signature m
void SignatureRSA(number m[], number close_key_d[], number open_key_n[], number result[]);

//verification of signature, 1- ok, -1- error
int VerificationSignatureRSA(number m[], number s[], number open_key_e[], number open_key_n[]);

//generation prime number a
void GenPrimeNumber(number a[], int Length_a);

//generation open and close keys
void GenKeys(number open_key_e[], number open_key_n[], number close_key_d[]);

void NumberToStr(number a[], char *res);

void StrToNumber(char *a, number res[]);

void HashFunction(char *c, number res[]);

