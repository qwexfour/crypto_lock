#include "stdafx.h"
#include "stdlib.h"
#include "time.h"
#include "longdiv.h"
#include <stdlib.h>

#define MAXNUMBER ((number)(-1))

#define LENGTH_BITS 256 //����� ����� � �����
#define LENGTH_2BYTES LENGTH_BITS / 16 //����� 2-��������� �����

typedef unsigned short number;
typedef unsigned long twonumber;

#define LOWORD(T)((unsigned short)(T))
#define HIWORD(T)((unsigned short)(T>>16))
#define MAKELONG(a,b) ((((unsigned long long int)(a))<<16)+b)

//����� ����� �
int LengthLong(number a[]);

//�������� � ������ �� 16 2-��������� ����
void ZeroLong(number a[]);

//���������� ����� � ����� b
void AssignLong(number a[], number b[]);

// 1 ���� �>b, -1 ���� a<b, 0 ���� a=b
int CmpLong(number a[], number b[]);

//������ ����� �� 256 ���
void PrintfLong(number a[]);

//������������� �������� a
void RandLong(number a[], int s);

// c = a*b mod n
void MulModLong(number a[], number b[], number n[], number c[]);

// c = a^2 mod n
void SquareModLong(number a[], number n[], number c[]);

// c = a^b mod p
void PowerModBinary(number a[], number b[], number mod[], number result[]);

//������� ������ m
void SignatureRSA(number m[], number close_key_d[], number open_key_n[], number result[]);

//�������� �������, 1-��������, -1-�� ��������
int VerificationSignatureRSA(number m[], number s[], number oped_key_e[], number open_key_n[]);

//��������� �������� ����� a, �������� �����
void GenPrimeNumber(number a[], int Length_a);

//��������� ��������� � ��������� ������ 
void GenKeys(number open_key_e[], number open_key_n[], number close_key_d[]);