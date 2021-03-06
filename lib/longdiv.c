/********************************************************************
Ïðîåêò:     Êîìïëåêò ÏÎ äëÿ ëàáîðàòîðíûõ ðàáîò ïî òåìå
"Ðåàëèçàöèÿ àñèììåòðè÷íûõ êðèïòîãðàôè÷åñêèõ àëãîðèòìîâ"
Ôàéë:       longdiv.c
Àâòîð:      Ãðóíòîâè÷ Ì.Ì.
Ñîäåðæàíèå: Ôóíêöèÿ äåëåíèÿ äëèííûõ ÷èñåë
Îïèñàíèå:   Íåîáõîäèìî
- ñêîïèðîâàòü ôàéëû longdiv.c è longdiv.h â êàòàëîã ïðîåêòà,
- äîáàâèòü ôàéë longdiv.c â ïðîåêò ëàáîðàòîðíîé ðàáîòû,
- âêëþ÷èòü ôàéë longdiv.h èíñòðóêöèåé
#include "longdiv.h"
â íà÷àëå ìîäóëÿ, èñïîëüçóþùåãî ôóíêöèþ äåëåíèÿ äëèííûõ ÷èñåë.
Èñòîðèÿ:    1998 - áàçîâàÿ âåðñèÿ
04.06.2005 - ïåðåñìîòðåííàÿ C-âåðñèÿ ñ èñïîëüçîâàíèåì òèïà Digit
*********************************************************************/

#include <stdlib.h>             /* êîíñòàíòà NULL */
#include "longdiv.h"   /* ôóíêöèÿ Div, êîíñòàíòà MAX_DIV_OPERAND_SIZE */
#include <stdio.h>

/*--------------------------------------------------------
Îïðåäåëåíèå íîâûõ òèïîâ
--------------------------------------------------------*/
typedef unsigned short Digit;   /* òèï öèôðà - áåççíàêîâîå ñëîâî */
typedef unsigned long TwoDigit; /* òèï äâîéíàÿ öèôðà - áåççíàêîâîå äâîéíîå ñëîâî */
typedef long LongDigit;         /* òèï äâîéíàÿ öèôðà ñî çíàêîì */

								/*--------------------------------------------------------
								Êîíñòàíòû
								--------------------------------------------------------*/
#define MAXDIGIT 0xFFFF			/* ìàêñèìàëüíîå çíà÷åíèå öèôðû(âñå áèòû ðàâíû 1) */
#define MAX_DIV_OPERAND_SIZE 32

								/*--------------------------------------------------------
								Ìàêðîñû
								--------------------------------------------------------*/
								/* âûäåëåíèå ìëàäøåé öèôïðû èç äâîéíîé öèôðû twodigit */
#define LODIGIT(twodigit) (Digit)(twodigit)
								/* âûäåëåíèå ñòàðøøåé öèôïðû èç äâîéíîé öèôðû twodigit */
#define HIDIGIT(twodigit) (Digit)((twodigit)>>(sizeof(Digit)*8))
								/* ôîðìèðîâàíèå äâîéíîé öèôðû èç digit1,digit2, ãäå digit2-ìëàäøàÿ,à digit1-ñòàðøàÿ öèôðû */
#define MAKE_TWO_DIGIT(digit1,digit2) (((TwoDigit)(digit1)<<(sizeof(Digit)*8))|(digit2))

								/*--------------------------------------------------------
								Îáíóëåíèå äëèííîãî ÷èñëà num = 0
								--------------------------------------------------------*/
static void Zero(
	Digit num[],                /* îáíóëÿåìîå ÷èñëî (size öèôð) */
	int size)                   /* ðàçìåð ÷èñëà â öèôðàõ */
{
	int i;                      /* èäåêñíàÿ ïåðåìåííàÿ öèêëà */

	for (i = 0; i<size; i++)         /* îðãàíèçàöèÿ öèêëà ïî i */
		num[i] = 0;               /* îáíóëåíèå i-îé öèôðû ÷èñëà num */
}

/*--------------------------------------------------------
Óìíîæåíèå äëèííîãî ÷èñëà íà öèôðó (*pcf,Res) = num * x
--------------------------------------------------------*/
static void Assign(
	Digit num1[],               /* ïîëó÷àòåëü (size öèôð) */
	const Digit num2[],         /* èñòî÷íèê (size öèôð) */
	int size)                   /* ðàçìåð ÷èñåë â öèôðàõ */
{
	int i;                      /* èäåêñíàÿ ïåðåìåííàÿ öèêëà */

	for (i = 0; i<size; i++)         /* îðãàíèçàöèÿ öèêëà ïî i */
		num1[i] = num2[i];        /* ïðèñâîåíèå i-îé öèôðå ÷èñëà num1 i-îé öèôðû ÷èñëà num2 */
}

/*--------------------------------------------------------
Óìíîæåíèå äëèííîãî ÷èñëà íà öèôðó (*pcf,Res) = num * x
--------------------------------------------------------*/
static void ShortMul(
	Digit Res[],                /* ðåçóëüòàò (size öèôð) */
	const Digit num[],          /* ïåðâûé ñîìíîæèòåëü (size öèôð) */
	Digit x,                    /* âòîðîé ñîìíîæèòåëü (1 öèôðà) */
	Digit *pcf,                 /* ïåðåíîñ ñòàðøåãî ðàçðÿäà (1 öèôðà), ì.á. NULL */
	int size)                   /* ðàçìåð ÷èñëà â öèôðàõ */
{
	TwoDigit buf;               /* ïåðåìåííàÿ äëÿ õðàíåíèÿ ïðîìåæóòî÷íîãî ðåçóëüòàòà óìíîæåíèÿ äâóõ ñëîâ */
	Digit cf = 0;                 /* ïåðåìåííàÿ äëÿ õðàíåíèÿ ïðîìåæóòî÷íîãî ðåçóëüòàòà ïåðåíîñà */
	int i;                      /* èäåêñíàÿ ïåðåìåííàÿ öèêëà */

	for (i = 0; i<size; i++)         /* îðãàíèçàöèÿ öèêëà ïî i */
	{
		buf = (TwoDigit)num[i] * x + cf;/* buf - ñóììà ðåóëüòàòà óìíîæåíèÿ è ïðåäûäóùåãî ïåðåíîñà */
		Res[i] = LODIGIT(buf);    /* i-ÿ öèôðà Res - ìëàäøàÿ öèôðà buf */
		cf = HIDIGIT(buf);        /* cf - ïåðåíîñ (ñòàðøàÿ öèôðà buf) */
	}
	if (pcf) *pcf = cf;             /* åñëè àäðåñ pcf íå 0, âîçâðàùàåì ïåðåíîñ */
}

/*--------------------------------------------------------
Äåëåíèå äëèííîãî ÷èñëà íà öèôðó Res = num/x, *pmod = num % x
--------------------------------------------------------*/
static void ShortDiv(
	Digit Res[],                /* ÷àñòíîå (size öèôð) */
	const Digit num[],          /* äåëèìîå  (size öèôð) */
	Digit x,                    /* äåëèòåëü (öèôðà) */
	Digit *pmod,                /* îñòàòîê (öèôðà), ì.á. NULL */
	int size)                   /* ðàçìåð äëèííûõ ÷èñåë â öèôðàõ */
{
	TwoDigit buf = 0;             /* âñïîìîãàòåëüíàÿ ïåðåìåííàÿ */
	int i;                      /* èäåêñíàÿ ïåðåìåííàÿ öèêëà */

	if (!x) return;              /* åñëè x ðàâåí 0 òî áåññëàâíî çàâåðøàåì äåëåíèå */
	for (i = size - 1; i >= 0; i--)      /* îðãàíèçàöèÿ öèêëà ïî i */
	{
		buf <<= sizeof(Digit) * 8;  /* ñòàðøàÿ öèôðà buf - ïðåäûäóùèé îñòàòîê */
		buf |= num[i];            /* ìëàäøàÿ öèôðà buf - i-ÿ öèôðà ÷èñëà num */
		Res[i] = LODIGIT(buf / x);  /* i-ÿ öèôðà Res - ðåçóëüòàò äåëåíèÿ */
		buf %= x;                 /* ìëàäøàÿ öèôðà buf - îñòàòîê */
	}
	if (pmod) *pmod = LODIGIT(buf);/* åñëè àäðåñ pmod íå 0, âîçâðàùàåì îñòàòîê */
}

/*--------------------------------------------------------
Ñëîæåíèå äëèííûõ ÷èñåë (*pcf,Res) = first + second
--------------------------------------------------------*/
static void Add(
	Digit Res[],                /* ñóììà (size öèôð) */
	const Digit first[],        /* ïåðâîå ñëàãàåìîå (size öèôð) */
	const Digit second[],       /* âòîðîå ñëàãàåìîå (size öèôð) */
	Digit *pcf,                 /* ôëàã ïåðåíîñà (1 öèôðà), ì.á. NULL */
	int size)                   /* ðàçìåð ÷èñåë â öèôðàõ */
{
	TwoDigit buf;               /* äëÿ õðàíåíèÿ ïðîìåæóòî÷íîãî ðåçóëüòàòà ñëîæåíèÿ */
	Digit cf = 0;                 /* äëÿ õðàíåíèÿ ïðîìåæóòî÷íîãî ïåðåíîñà */
	int i;                      /* èäåêñíàÿ ïåðåìåííàÿ öèêëà */

	for (i = 0; i<size; i++)         /* îðãàíèçàöèÿ öèêëà ïî i */
	{
		buf = (TwoDigit)first[i] + second[i] + cf;/* ñëîæåíèå i-õ öèôð è ïðåäûäóùåãî ïåðåíîñà */
		Res[i] = LODIGIT(buf);    /* i-ÿ öèôðà Res - ìëàäøàÿ öèôðà ñóììû */
		cf = HIDIGIT(buf);        /* ïåðåíîñ - ñòàðøàÿ öèôðà ñóììû */
	}
	if (pcf) *pcf = cf;            /* åñëè àäðåñ pcf íå 0, âîçâðàùàåì ïåðåíîñ */
}

/*--------------------------------------------------------
Âû÷èòàíèå äëèííûõ ÷èñåë (*pcf,Res) = first - second
--------------------------------------------------------*/
static void Sub(
	Digit Res[],                /* ðàçíîñòü (size öèôð) */
	const Digit first[],        /* óìåíüøàåìîå (size öèôð) */
	const Digit second[],       /* âû÷èòàåìîå (size öèôð) */
	Digit *pcf,                 /* ôëàã çàåìà ðàçðÿäîâ (1 öèôðà), ì.á. NULL */
	int size)                   /* ðàçìåð ÷èñåë â öèôðàõ */
{
	LongDigit buf;              /* çíàêîâàÿ ïåðåìåííàÿ äëÿ âûäåëåíèÿ çàåìà ðàçðÿäîâ */
	Digit cf = 0;                 /* äëÿ õðàíåíèÿ ïðîìåæóòî÷íîãî çàåìà */
	int i;                      /* èäåêñíàÿ ïåðåìåííàÿ öèêëà */

	for (i = 0; i<size; i++)         /* îðãàíèçàöèÿ öèêëà ïî i */
	{
		buf = (LongDigit)first[i] - second[i] - cf;/* âû÷èòàíèå i-õ öèôð ñ ó÷åòîì ïðåäûäóùåãî çàåìà */
		Res[i] = LODIGIT(buf);    /* i-ÿ öèôðà Res - ìëàäøàÿ öèôðà ðàçíîñòè buf */
		cf = HIDIGIT(buf);        /* çàåì - ñòàðøàÿ öèôðà ðàçíîñòè buf */
		if (cf) cf = 1;            /* åñëè çàåì áûë, ôëàãó çàåìà ïðèñâàåâàåì 1 */
	}
	if (pcf) *pcf = cf;            /* åñëè àäðåñ pcf íå 0, âîçâðàùàåì çàåì */
}

/*--------------------------------------------------------
Äåëåíèå äëèííûõ ÷èñåë Q = U/V, R = U % V
Èñêëþ÷åíèå äåëåíèÿ íà 0 íå îáðàáàòûâàåòñÿ
--------------------------------------------------------*/
void Div(
	const Digit U[],            /* äåëèìîå (sizeU öèôð) */
	const Digit V[],            /* äåëèòåëü (sizeV öèôð) */
	Digit Q[],                  /* ÷àñòíîå (sizeU öèôð), ì.á. NULL */
	Digit R[],                  /* îñòàòîê (sizeV öèôð), ì.á. NULL */
	int sizeU,                  /* ðàçìåð ÷èñåë U è Q â öèôðàõ */
	int sizeV)                  /* ðàçìåð ÷èñåë V è R â öèôðàõ */
{
	Digit q, buf1, buf2;                /* äëÿ ïðîìåæóòî÷íîãî õðàíåíèÿ */
	Digit U2[MAX_DIV_OPERAND_SIZE + 1],   /* äëÿ íîðìàëèçîâàííîãî U */
		V2[MAX_DIV_OPERAND_SIZE + 1];   /* äëÿ íîðìàëèçîâàííîãî V */
	TwoDigit inter;                     /* äëÿ ïðîìåæóòî÷íûõ îïåðàöèé */
	int i, j, k;                          /* èíäåêñíûå ïåðåìåííûå */
	Digit d;                            /* íîðìàëèçóþùèé ìíîæèòåëü */

										/*--- Ïðîâåðêè, ïîäãîòîâêà: */
	if (R) Zero(R, sizeV);                /* åñëè àäðåñ îñòàòêà R íå 0, îáíóëÿåì îñòàòîê */
	if (Q) Zero(Q, sizeU);                /* åñëè àäðåñ ÷àñòíîãî Q íå 0, îáíóëÿåì ÷àñòíîå */

	for (i = sizeV - 1; (i >= 0)&(!V[i]); i--);  /* àíàëèç äåëèòåëÿ, îòñåêàåì ñòàðøèå íåçíà÷àùèå íóëè */
	sizeV = i + 1;                          /* íîâûé ðàçìåð äåëèòåëÿ */
	if (!sizeV) return;                  /* èñêëþ÷åíèå "Äåëåíèå íà íîëü" (ïðîñòî óõîäèì) */

	for (k = sizeU - 1; (k >= 0)&(!U[k]); k--);  /* àíàëèç äåëèìîãî, îòñåêàåì ñòàðøèå íåçíà÷àùèå íóëè */
	sizeU = k + 1;                          /* íîâûé ðàçìåð äåëèìîãî */

	if (sizeV>sizeU)                     /* åñëè äåëèòåëü áîëüøå äåëèìîãî, òî */
	{
		if (R) Assign(R, U, sizeU);        /* îñòàòîê ðàâåí äåëèìîìó */
		return;                        /* óõîäèì */
	}
	else if (sizeV == 1)                   /* åñëè äåëèòåëü - 1 öèôðà, òî */
	{
		ShortDiv(Q, U, V[0], R, sizeU);     /* ïðèìåíÿåì óïðîùåííûé àëãîðèòì */
		return;                        /* óõîäèì */
	}

	/*--- Íîðìàëèçàöèÿ: */
	d = (Digit)(((TwoDigit)MAXDIGIT + 1) / ((TwoDigit)V[sizeV - 1] + 1)); /* íîðìàëèçóþùèé ìíîæèòåëü */
	if (d != 1)                            /* åñëè d íå 1, */
	{
		ShortMul(V2, V, d, &buf1, sizeV);   /* óìíîæàåì V íà d */
		V2[sizeV] = buf1;
		ShortMul(U2, U, d, &buf1, sizeU);   /* óìíîæàåì U íà d */
		U2[sizeU] = buf1;
	}
	else
	{                                   /* åñëè d == 1, */
		Assign(V2, V, sizeV);             /* V2 = V */
		V2[sizeV] = 0;
		Assign(U2, U, sizeU);             /* U2 = U */
		U2[sizeU] = 0;
	}

	/*--- Îñíîâíîé öèêë */
	for (j = sizeU; j >= sizeV; j--)           /* îðãàíèçàöèÿ ãëàâíîãî öèêëà ïî j (sizeU-sizeV ðàç) */
	{
		/*--- Î÷åðåäíàÿ öèôðà ÷àñòíîãî */
		inter = MAKE_TWO_DIGIT(U2[j], U2[j - 1]); /* ïðèãîäèòñÿ */
		if (U2[j] == V2[sizeV - 1])          /* åñëè ñòàðøèå öèôðû ðàâíû, */
			q = MAXDIGIT;                 /* öèôðà ÷àñòíîãî q = MAXDIGIT */
		else {                        /* èíà÷å */
			q = (Digit)(inter / V2[sizeV - 1]);/* j-þ öèôðó ÷àñòíîãî q íàõîäèì äåëåíèåì */
											   /* åñëè q âåëèêîâàò, */

											   /*--- Êîððåêöèÿ öèôðû ÷àñòíîãî */
			if (((TwoDigit)V2[sizeV - 2] * q)>(MAKE_TWO_DIGIT((Digit)(inter%V2[sizeV - 1]), U2[j - 2])))
			{
				q--;
			}
		}/* êîððåêöèÿ öèôðû ÷àñòíîãî óìåíüøåíèåì q íà 1 */

		 /*--- Âû÷èòàíèå êðàòíîãî äåëèòåëÿ */
		ShortMul(R, V2, q, &buf1, sizeV);   /* óìíîæàåì V íà q */
		Sub(U2 + j - sizeV, U2 + j - sizeV, R, &buf2, sizeV);/* âû÷èòàåì ðåçóëüòàò óìíîæåíèÿ */
		inter = (LongDigit)U2[j] - buf1 - buf2;
		U2[j] = LODIGIT(inter);

		/*--- Êîððåêöèÿ îñòàòêà è ÷àñòíîãî */
		if (HIDIGIT(inter))              /* åñëè ðåçóëüòàò øàãà îòðèöàòåëüíûé, */
		{                               /* êîìïåíñèðóþùåå ñëîæåíèå */
			Add(U2 + j - sizeV, U2 + j - sizeV, V2, &buf2, sizeV);
			U2[j] += buf2;
			q--;                        /* êîððåêöèÿ öèôðû ÷àñòíîãî q íà 1 */
		}
		if (Q)                           /* åñëè àäðåñ ÷àñòíîãî Q íå 0, */
			Q[j - sizeV] = q;               /* çàïîìèíàåì j-þ öèôðó ÷àñòíîãî q */
	}

	/*--- Çàâåðøåíèå */
	if (R)                               /* åñëè àäðåñ îñòàòêà R íå 0, */
	{
		ShortDiv(R, U2, d, NULL, sizeV);    /* äåíîðìàëèçàöèÿ îñòàòêà R */
	}
}

