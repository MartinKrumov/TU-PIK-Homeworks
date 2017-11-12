#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <conio.h>
#include <Windows.h>
#include <stdlib.h>
#include "menu.h"
#include "dict_func.h"

//------------- менюта -----------------------
char *mainMenu[] =
{
	"Програма <РЕЧНИК>",

	"Избор на вход за текста.",
	"Създване на речник на текста.",
	"Извеждане речника на екрана.",
	"Търсене на дума в речника.",
	"Сортиране на речника.",
	"Запис на речника.",
	"Зареждане на речника от текстови файл.",
	"Кодиране/декодиране на текста.", 
	"Изход",
	NULL
};

char *chooseFile[] =
{
	"---ВХОДЕН ТЕКСТ---",
	"От клавиатура.",
	"От текстови файл.",
	"Назад.",
	NULL
};

char *sortMenu[] =
{
	"Избор на сортиране:",
	"По азбучен ред.",
	"По брой срещания на думата в текста.",
	"Назад.",
	NULL
};

char *saveMenu[] =
{
	"Избор на начин за записване:",
	"Всяка дума на нов ред и срещу нея честотата.",
	"Записване на бройката в отделен файл с разширение \".cnt\".",
	"Назад.",
	NULL
};
char *codingMenu[] =
{
	"---ОБРАБОТКА---",
	"Кодиране на текст.",
	"Декодиране на текст.",
	"Назад.",
	NULL
};

int main()
{
	int mode, submenu;
	FILE *fp_in = NULL;        //файлов указател за входен файл 
		//*fp_dict = NULL;      //файлов указател за изходен файл на речника 
	char name_file[NAMES]/*, name_dict[NAMES] ,name_code[NAMES]*/;

	char **dict = NULL;        //динамичен  масив от указатели за съхраняване думите на речника 
	int *countOfWords = NULL;   //динамичен int масив за броя на срещанията на думите в речника 
	int sizeOfDict = SIZE_DICT; //текущ размер на речника
	int currentIndex = 0;       //актуален индекс за съхраняване на нова дума в речника 
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
				printf("\nВход от КЛАВИАТУРА - за край Въведете - <cntrl/Z> !!!\n\n");
				break;
			case 2:
				fp_in = getNameFile(name_file, "rt", "за четене");
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
				flag = 1; //flag дали има създаден речник

				printf("Речникът е успешно записан.\n");
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
				printf("Речникът не е създаден.\n");
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
				printf("Речникът не е създаден.\n");
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
				printf("Речникът не е създаден.\n");
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

			fp_in = getNameFile(name_file, "rt", "за четене");
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
				printf("Речникът несъществува.\n");
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