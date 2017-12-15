/********************************************************************
������:     �������� �� ��� ������������ ����� �� ����
"���������� ������������� ����������������� ����������"
����:       longdiv.c
�����:      ��������� �.�.
����������: ������� ������� ������� �����
��������:   ����������
- ����������� ����� longdiv.c � longdiv.h � ������� �������,
- �������� ���� longdiv.c � ������ ������������ ������,
- �������� ���� longdiv.h �����������
#include "longdiv.h"
� ������ ������, ������������� ������� ������� ������� �����.
�������:    1998 - ������� ������
04.06.2005 - �������������� C-������ � �������������� ���� Digit
*********************************************************************/

#include <stdlib.h>             /* ��������� NULL */
#include "longdiv.h"   /* ������� Div, ��������� MAX_DIV_OPERAND_SIZE */
#include <stdio.h>

/*--------------------------------------------------------
����������� ����� �����
--------------------------------------------------------*/
typedef unsigned short Digit;   /* ��� ����� - ����������� ����� */
typedef unsigned long TwoDigit; /* ��� ������� ����� - ����������� ������� ����� */
typedef long LongDigit;         /* ��� ������� ����� �� ������ */

								/*--------------------------------------------------------
								���������
								--------------------------------------------------------*/
#define MAXDIGIT 0xFFFF			/* ������������ �������� �����(��� ���� ����� 1) */
#define MAX_DIV_OPERAND_SIZE 32

								/*--------------------------------------------------------
								�������
								--------------------------------------------------------*/
								/* ��������� ������� ������ �� ������� ����� twodigit */
#define LODIGIT(twodigit) (Digit)(twodigit)
								/* ��������� �������� ������ �� ������� ����� twodigit */
#define HIDIGIT(twodigit) (Digit)((twodigit)>>(sizeof(Digit)*8))
								/* ������������ ������� ����� �� digit1,digit2, ��� digit2-�������,� digit1-������� ����� */
#define MAKE_TWO_DIGIT(digit1,digit2) (((TwoDigit)(digit1)<<(sizeof(Digit)*8))|(digit2))

								/*--------------------------------------------------------
								��������� �������� ����� num = 0
								--------------------------------------------------------*/
static void Zero(
	Digit num[],                /* ���������� ����� (size ����) */
	int size)                   /* ������ ����� � ������ */
{
	int i;                      /* �������� ���������� ����� */

	for (i = 0; i<size; i++)         /* ����������� ����� �� i */
		num[i] = 0;               /* ��������� i-�� ����� ����� num */
}

/*--------------------------------------------------------
��������� �������� ����� �� ����� (*pcf,Res) = num * x
--------------------------------------------------------*/
static void Assign(
	Digit num1[],               /* ���������� (size ����) */
	const Digit num2[],         /* �������� (size ����) */
	int size)                   /* ������ ����� � ������ */
{
	int i;                      /* �������� ���������� ����� */

	for (i = 0; i<size; i++)         /* ����������� ����� �� i */
		num1[i] = num2[i];        /* ���������� i-�� ����� ����� num1 i-�� ����� ����� num2 */
}

/*--------------------------------------------------------
��������� �������� ����� �� ����� (*pcf,Res) = num * x
--------------------------------------------------------*/
static void ShortMul(
	Digit Res[],                /* ��������� (size ����) */
	const Digit num[],          /* ������ ����������� (size ����) */
	Digit x,                    /* ������ ����������� (1 �����) */
	Digit *pcf,                 /* ������� �������� ������� (1 �����), �.�. NULL */
	int size)                   /* ������ ����� � ������ */
{
	TwoDigit buf;               /* ���������� ��� �������� �������������� ���������� ��������� ���� ���� */
	Digit cf = 0;                 /* ���������� ��� �������� �������������� ���������� �������� */
	int i;                      /* �������� ���������� ����� */

	for (i = 0; i<size; i++)         /* ����������� ����� �� i */
	{
		buf = (TwoDigit)num[i] * x + cf;/* buf - ����� ��������� ��������� � ����������� �������� */
		Res[i] = LODIGIT(buf);    /* i-� ����� Res - ������� ����� buf */
		cf = HIDIGIT(buf);        /* cf - ������� (������� ����� buf) */
	}
	if (pcf) *pcf = cf;             /* ���� ����� pcf �� 0, ���������� ������� */
}

/*--------------------------------------------------------
������� �������� ����� �� ����� Res = num/x, *pmod = num % x
--------------------------------------------------------*/
static void ShortDiv(
	Digit Res[],                /* ������� (size ����) */
	const Digit num[],          /* �������  (size ����) */
	Digit x,                    /* �������� (�����) */
	Digit *pmod,                /* ������� (�����), �.�. NULL */
	int size)                   /* ������ ������� ����� � ������ */
{
	TwoDigit buf = 0;             /* ��������������� ���������� */
	int i;                      /* �������� ���������� ����� */

	if (!x) return;              /* ���� x ����� 0 �� ��������� ��������� ������� */
	for (i = size - 1; i >= 0; i--)      /* ����������� ����� �� i */
	{
		buf <<= sizeof(Digit) * 8;  /* ������� ����� buf - ���������� ������� */
		buf |= num[i];            /* ������� ����� buf - i-� ����� ����� num */
		Res[i] = LODIGIT(buf / x);  /* i-� ����� Res - ��������� ������� */
		buf %= x;                 /* ������� ����� buf - ������� */
	}
	if (pmod) *pmod = LODIGIT(buf);/* ���� ����� pmod �� 0, ���������� ������� */
}

/*--------------------------------------------------------
�������� ������� ����� (*pcf,Res) = first + second
--------------------------------------------------------*/
static void Add(
	Digit Res[],                /* ����� (size ����) */
	const Digit first[],        /* ������ ��������� (size ����) */
	const Digit second[],       /* ������ ��������� (size ����) */
	Digit *pcf,                 /* ���� �������� (1 �����), �.�. NULL */
	int size)                   /* ������ ����� � ������ */
{
	TwoDigit buf;               /* ��� �������� �������������� ���������� �������� */
	Digit cf = 0;                 /* ��� �������� �������������� �������� */
	int i;                      /* �������� ���������� ����� */

	for (i = 0; i<size; i++)         /* ����������� ����� �� i */
	{
		buf = (TwoDigit)first[i] + second[i] + cf;/* �������� i-� ���� � ����������� �������� */
		Res[i] = LODIGIT(buf);    /* i-� ����� Res - ������� ����� ����� */
		cf = HIDIGIT(buf);        /* ������� - ������� ����� ����� */
	}
	if (pcf) *pcf = cf;            /* ���� ����� pcf �� 0, ���������� ������� */
}

/*--------------------------------------------------------
��������� ������� ����� (*pcf,Res) = first - second
--------------------------------------------------------*/
static void Sub(
	Digit Res[],                /* �������� (size ����) */
	const Digit first[],        /* ����������� (size ����) */
	const Digit second[],       /* ���������� (size ����) */
	Digit *pcf,                 /* ���� ����� �������� (1 �����), �.�. NULL */
	int size)                   /* ������ ����� � ������ */
{
	LongDigit buf;              /* �������� ���������� ��� ��������� ����� �������� */
	Digit cf = 0;                 /* ��� �������� �������������� ����� */
	int i;                      /* �������� ���������� ����� */

	for (i = 0; i<size; i++)         /* ����������� ����� �� i */
	{
		buf = (LongDigit)first[i] - second[i] - cf;/* ��������� i-� ���� � ������ ����������� ����� */
		Res[i] = LODIGIT(buf);    /* i-� ����� Res - ������� ����� �������� buf */
		cf = HIDIGIT(buf);        /* ���� - ������� ����� �������� buf */
		if (cf) cf = 1;            /* ���� ���� ���, ����� ����� ����������� 1 */
	}
	if (pcf) *pcf = cf;            /* ���� ����� pcf �� 0, ���������� ���� */
}

/*--------------------------------------------------------
������� ������� ����� Q = U/V, R = U % V
���������� ������� �� 0 �� ��������������
--------------------------------------------------------*/
void Div(
	const Digit U[],            /* ������� (sizeU ����) */
	const Digit V[],            /* �������� (sizeV ����) */
	Digit Q[],                  /* ������� (sizeU ����), �.�. NULL */
	Digit R[],                  /* ������� (sizeV ����), �.�. NULL */
	int sizeU,                  /* ������ ����� U � Q � ������ */
	int sizeV)                  /* ������ ����� V � R � ������ */
{
	Digit q, buf1, buf2;                /* ��� �������������� �������� */
	Digit U2[MAX_DIV_OPERAND_SIZE + 1],   /* ��� ���������������� U */
		V2[MAX_DIV_OPERAND_SIZE + 1];   /* ��� ���������������� V */
	TwoDigit inter;                     /* ��� ������������� �������� */
	int i, j, k;                          /* ��������� ���������� */
	Digit d;                            /* ������������� ��������� */

										/*--- ��������, ����������: */
	if (R) Zero(R, sizeV);                /* ���� ����� ������� R �� 0, �������� ������� */
	if (Q) Zero(Q, sizeU);                /* ���� ����� �������� Q �� 0, �������� ������� */

	for (i = sizeV - 1; (i >= 0)&(!V[i]); i--);  /* ������ ��������, �������� ������� ���������� ���� */
	sizeV = i + 1;                          /* ����� ������ �������� */
	if (!sizeV) return;                  /* ���������� "������� �� ����" (������ ������) */

	for (k = sizeU - 1; (k >= 0)&(!U[k]); k--);  /* ������ ��������, �������� ������� ���������� ���� */
	sizeU = k + 1;                          /* ����� ������ �������� */

	if (sizeV>sizeU)                     /* ���� �������� ������ ��������, �� */
	{
		if (R) Assign(R, U, sizeU);        /* ������� ����� �������� */
		return;                        /* ������ */
	}
	else if (sizeV == 1)                   /* ���� �������� - 1 �����, �� */
	{
		ShortDiv(Q, U, V[0], R, sizeU);     /* ��������� ���������� �������� */
		return;                        /* ������ */
	}

	/*--- ������������: */
	d = (Digit)(((TwoDigit)MAXDIGIT + 1) / ((TwoDigit)V[sizeV - 1] + 1)); /* ������������� ��������� */
	if (d != 1)                            /* ���� d �� 1, */
	{
		ShortMul(V2, V, d, &buf1, sizeV);   /* �������� V �� d */
		V2[sizeV] = buf1;
		ShortMul(U2, U, d, &buf1, sizeU);   /* �������� U �� d */
		U2[sizeU] = buf1;
	}
	else
	{                                   /* ���� d == 1, */
		Assign(V2, V, sizeV);             /* V2 = V */
		V2[sizeV] = 0;
		Assign(U2, U, sizeU);             /* U2 = U */
		U2[sizeU] = 0;
	}

	/*--- �������� ���� */
	for (j = sizeU; j >= sizeV; j--)           /* ����������� �������� ����� �� j (sizeU-sizeV ���) */
	{
		/*--- ��������� ����� �������� */
		inter = MAKE_TWO_DIGIT(U2[j], U2[j - 1]); /* ���������� */
		if (U2[j] == V2[sizeV - 1])          /* ���� ������� ����� �����, */
			q = MAXDIGIT;                 /* ����� �������� q = MAXDIGIT */
		else {                        /* ����� */
			q = (Digit)(inter / V2[sizeV - 1]);/* j-� ����� �������� q ������� �������� */
											   /* ���� q ���������, */

											   /*--- ��������� ����� �������� */
			if (((TwoDigit)V2[sizeV - 2] * q)>(MAKE_TWO_DIGIT((Digit)(inter%V2[sizeV - 1]), U2[j - 2])))
			{
				q--;
			}
		}/* ��������� ����� �������� ����������� q �� 1 */

		 /*--- ��������� �������� �������� */
		ShortMul(R, V2, q, &buf1, sizeV);   /* �������� V �� q */
		Sub(U2 + j - sizeV, U2 + j - sizeV, R, &buf2, sizeV);/* �������� ��������� ��������� */
		inter = (LongDigit)U2[j] - buf1 - buf2;
		U2[j] = LODIGIT(inter);

		/*--- ��������� ������� � �������� */
		if (HIDIGIT(inter))              /* ���� ��������� ���� �������������, */
		{                               /* �������������� �������� */
			Add(U2 + j - sizeV, U2 + j - sizeV, V2, &buf2, sizeV);
			U2[j] += buf2;
			q--;                        /* ��������� ����� �������� q �� 1 */
		}
		if (Q)                           /* ���� ����� �������� Q �� 0, */
			Q[j - sizeV] = q;               /* ���������� j-� ����� �������� q */
	}

	/*--- ���������� */
	if (R)                               /* ���� ����� ������� R �� 0, */
	{
		ShortDiv(R, U2, d, NULL, sizeV);    /* �������������� ������� R */
	}
}
