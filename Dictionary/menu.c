#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <conio.h>

short menu(char *arr[])
{
	int k, i = 1;
	char ch;

	printf("%s\n", arr[0]); //�������� ������� ��� �� ���������
	while (arr[i])
	{
		printf("\n<%d> - %s", i, arr[i]); //�������� ������ �� ������ NULL
		i++;
	}

	printf("\n\n������ �����: ");
	do
	{
		ch = _getch();//����� ���� ���� ������ 
		k = ch - '0'; //�������� char � int (����� ���� ���� � �� ���� ���� �� 0-��)
	} while (k <= 0 || k > i);

	printf("%c\n", ch);

	return ch - '0'; // ����� �����
}