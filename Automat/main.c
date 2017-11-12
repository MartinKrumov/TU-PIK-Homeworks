#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <Windows.h>
#include "menu.h"
#include "automat_f.h"

#define NAMES 256

char *txtMenu[] =
{
	"\t������ ����",

	"����� �� ������ ����.",
	"����������� �� ������ � ��������� �� ������ ��� ����.",
	"����������� �� ����������� � ��������� �� ����������� ��� ����.", 
	"������������ �� � ���������.",
	"������������ �� ������ ���������.",
	"�������� �� ������� ����������.",
	"�����.",
	NULL
};

int main()
{
	int mode;
	FILE *fp_in = NULL, *fp_out = NULL;
	char inputFName[NAMES], outputFName[NAMES];
	char path[NAMES];
	int flag = 0;
	int flag2 = 0;

	size_t countOfWords = 0;
	size_t countOfSentences = 0;

	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	do
	{
		system("cls");
		mode = menu(txtMenu);
		switch (mode)
		{
		case 1:
			if ((fp_in = getNameFile(inputFName, "rb", "�� ������")) != NULL)
			{
				flag = 1;
			}
			break;

		case 2:
			if (flag == 0)
			{
				printf("\n�� ��� �������� ���� �� ���������!\n");
				system("pause");
				break;
			}

			if ((fp_out = getNameFile(outputFName, "wt", "�� �����")) != NULL)
			{
				flag2 = 1;
				system("cls");
				countOfWords = GetWords(fp_in, fp_out);
				if (countOfWords)
					printf("\n\n����������� ���� ��� ������ %s ��: %d\n\n", inputFName, countOfWords);
			}

			system("pause");
			if (flag2 == 1)
				fclose(fp_out);
			break;

		case 3:
			if (flag == 0)
			{
				printf("\n�� ��� �������� ���� �� ���������!\n");
				system("pause");
				break;
			}

			if ((fp_out = getNameFile(outputFName, "wt", "�� �����")) != NULL)
			{
				system("cls");
				PrintSentences(fp_in, fp_out);
				countOfSentences = CountOfSentences(fp_in);

				printf("\n\n����������� ��������� ��� ������ %s ��: %d\n\n", inputFName, countOfSentences);
			}
			system("pause");
			if (flag2 == 1)
				fclose(fp_out);
			break;

		case 4:
			if (flag == 0)
			{
				printf("\n�� ��� �������� ���� �� ���������!\n");
				system("pause");
				break;
			}

			IntConstant(fp_in);
			system("pause");
			break;

		case 5:
			if (flag == 0)
			{
				printf("\n�� ��� �������� ���� �� ���������!\n");
				system("pause");
				break;
			}
			
			RealConstant(fp_in);
			system("pause");
			break;

		case 6:
			printf("�������� ���������� �� �����������: ");
			fscanf(stdin, "%s", path);
			ValidDirectory(path);

			system("pause");
			break;
		}
	} while (mode != 7);
	_fcloseall(); // ������� ������ ������
}

