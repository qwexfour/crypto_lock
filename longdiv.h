/********************************************************************
������:     �������� �� ��� ������������ ����� �� ����
"���������� ������������� ����������������� ����������"
����:       longdiv.h
�����:      ��������� �.�.
����������: ��������� ������� ������� ������� �����
��������:   ����������
- ����������� ����� longdiv.c � longdiv.h � ������� �������,
- �������� ���� longdiv.c � ������ ������������ ������,
- �������� ���� longdiv.h �����������
#include "longdiv.h"
� ������ ������, ������������� ������� ������� ������� �����.
�������:    1998 - ������� ������
04.06.2005 - �������������� C-������
*********************************************************************/
//#include "stdafx.h"

#define MAX_DIV_OPERAND_SIZE 32 /* ���������� ������ ��������� Div � ������ */


	/*--------------------------------------------------------
	������� ������� ����� Q = U/V, R = U % V
	���������� ������� �� 0 �� ��������������
	--------------------------------------------------------*/
	void Div(
		const unsigned short U[],   /* ������� (sizeU ����) */
		const unsigned short V[],   /* �������� (sizeV ����) */
		unsigned short Q[],         /* ������� (sizeU ����), �.�. NULL */
		unsigned short R[],         /* ������� (sizeV ����), �.�. NULL */
		int sizeU,                  /* ������ ����� U � Q � ������ */
		int sizeV);                 /* ������ ����� V � R � ������ */



/*--------------------------------------------------------
��������� ������� Div.
MAX_DIV_OPERAND_SIZE - ���������� ��������� � ������� ����� ���������� ������� Div.
��� ��������� ���������� ��� ������������ ��������� ������ � ������� Div.
�������� �� ��� ���������, ���� �� ���������, ��� ��� ������.
--------------------------------------------------------*/