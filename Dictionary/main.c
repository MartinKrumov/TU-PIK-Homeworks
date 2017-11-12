#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <conio.h>
#include <Windows.h>
#include <stdlib.h>
#include "menu.h"
#include "dict_func.h"

//------------- ������ -----------------------
char *mainMenu[] =
{
	"�������� <������>",

	"����� �� ���� �� ������.",
	"�������� �� ������ �� ������.",
	"��������� ������� �� ������.",
	"������� �� ���� � �������.",
	"��������� �� �������.",
	"����� �� �������.",
	"��������� �� ������� �� �������� ����.",
	"��������/���������� �� ������.", 
	"�����",
	NULL
};

char *chooseFile[] =
{
	"---������ �����---",
	"�� ����������.",
	"�� �������� ����.",
	"�����.",
	NULL
};

char *sortMenu[] =
{
	"����� �� ���������:",
	"�� ������� ���.",
	"�� ���� �������� �� ������ � ������.",
	"�����.",
	NULL
};

char *saveMenu[] =
{
	"����� �� ����� �� ���������:",
	"����� ���� �� ��� ��� � ����� ��� ���������.",
	"��������� �� �������� � ������� ���� � ���������� \".cnt\".",
	"�����.",
	NULL
};
char *codingMenu[] =
{
	"---���������---",
	"�������� �� �����.",
	"���������� �� �����.",
	"�����.",
	NULL
};

int main()
{
	int mode, submenu;
	FILE *fp_in = NULL;        //������ �������� �� ������ ���� 
		//*fp_dict = NULL;      //������ �������� �� ������� ���� �� ������� 
	char name_file[NAMES]/*, name_dict[NAMES] ,name_code[NAMES]*/;

	char **dict = NULL;        //���������  ����� �� ��������� �� ����������� ������ �� ������� 
	int *countOfWords = NULL;   //��������� int ����� �� ���� �� ���������� �� ������ � ������� 
	int sizeOfDict = SIZE_DICT; //����� ������ �� �������
	int currentIndex = 0;       //�������� ������ �� ����������� �� ���� ���� � ������� 
	int status, flag = 0;

	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	do
	{
		system("cls");
		mode = menu(mainMenu);
		switch (mode)
		{
		case 1:
			submenu = menu(chooseFile);
			switch (submenu)
			{
			case 1:
				fp_in = stdin;
				printf("\n���� �� ���������� - �� ���� �������� - <cntrl/Z> !!!\n\n");
				break;
			case 2:
				fp_in = getNameFile(name_file, "rt", "�� ������");
			}
			break;

		case 2:
			if (fp_in == NULL)
			{
				printf("\nMissing input file !!!\n");
				system("pause");
			}
			else
			{
				DeleteDictionary(dict, countOfWords, currentIndex);
				dict = NULL;
				sizeOfDict = SIZE_DICT;
				currentIndex = 0;
				countOfWords = 0;

				dict = MakeDictionary(fp_in, dict, &sizeOfDict, &currentIndex, &countOfWords, &status);
				flag = 1; //flag ���� ��� �������� ������

				printf("�������� � ������� �������.\n");
				system("pause");
			}
			break;

		case 3:
			system("cls");
			PrintDictionary(dict, currentIndex, countOfWords, status);
			system("pause");
			break;

		case 4:
			if (flag == 0)
			{
				printf("�������� �� � ��������.\n");
				system("pause");
			}
			else
			{
				SearchInDictionary(dict, currentIndex);
				system("pause");
			}
			break;

		case 5:
			if (flag == 0)
			{
				printf("�������� �� � ��������.\n");
			}
			else
			{
				submenu = menu(sortMenu);
				switch (submenu)
				{
				case 1:
					SortByAlphabet(dict, currentIndex, countOfWords);
					break;

				case 2:
					SortByFrequency(dict, countOfWords, currentIndex);
					break;
				}
			}
			system("pause");
			break;

		case 6:
			if (flag == 0)
			{
				printf("�������� �� � ��������.\n");
			}
			else
			{
				submenu = menu(saveMenu);
				switch (submenu)
				{
				case 1:
					SaveDictInFile(dict, countOfWords, currentIndex, name_file, mode);
					break;

				case 2:
					SaveDictInCNTFile(dict, countOfWords, currentIndex, name_file, mode);
					break;
				}
			}
			system("pause");
			break;

		case 7:

			fp_in = getNameFile(name_file, "rt", "�� ������");
			if (fp_in == NULL)
			{
				printf("\nMissing input file !!!\n");
				system("pause");
			}
			else
			{
				DeleteDictionary(dict, countOfWords, currentIndex);
				dict = NULL;
				sizeOfDict = SIZE_DICT;
				currentIndex = 0;
				countOfWords = 0;

				dict = MakeDictionary(fp_in, dict, &sizeOfDict, &currentIndex, &countOfWords, &status);
				flag = 1;
			}
			break;

		case 8:
			if (flag == 0)
			{
				printf("�������� ������������.\n");
			}
			else
			{
				submenu = menu(codingMenu);
				switch (submenu)
				{
				case 1:
					CodingFile(dict, currentIndex, name_file);
					break;

				case 2:
					DecodingFile(dict, currentIndex, name_file);
					break;
				}
				break;
			}
			system("pause");
			break;
		}
	} while (mode != 9);

	if (fp_in)
		fclose(fp_in);

	_fcloseall();
}