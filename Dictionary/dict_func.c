#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "dict_func.h"

FILE *getNameFile(char *name_file, char *mode, char *text)
{
	FILE *fp;
	int err;
	errno = 0;

	do
	{
		printf("Въведете име на файла %s: ", text);
		err = scanf("%s", name_file);
	} while (err != 1);

	if ((fp = fopen(name_file, mode)) == NULL)
	{
		printf("\nФайлът <%s> Не може да се отвори - error No % d !!!\n", name_file, errno);
		printf("ГРЕШКА No %d %s\n", errno, strerror(errno));
		system("pause");
	}
	else
	{
		printf("\nФайлът <%s> е отворен успешно за обработка !\n\n", name_file);
		system("pause");
	}
	return fp;
}

char delimiters_word[] = { ' ', ',', '.', '?', '"', '=', '!', ':', ';', '<', '>',
'/', '\\', '*', '(', ')', '{', '}', '[', ']', '\n', '\t', '\r', 0 };

int IsDelimiter(char *delimiters, char c)
{
	int i = 0;
	while (delimiters[i])
	{
		if (c == delimiters[i++])
			return 1; //символът с е разделител
	}
	return 0;
}

void ComputeWord(int c,        //--- текущ символ, които се анализира ---
	char *word,   //--- символен масив за отделяне на дума
	int *ind,     //--- текуща позиция в думата
	int *status)   //--- текущ статус на автоматната функция	
{
	char ch = c;
	if (*status == 2)
	{
		*status = 0;
		*ind = 0;
	}
	switch (*status)
	{
	case 0:
		if (!IsDelimiter(delimiters_word, ch))
		{
			*status = 1;
			word[(*ind)++] = c;
			word[*ind] = 0;
		}
		break;

	case 1:
		if (IsDelimiter(delimiters_word, ch))
			*status = 2;
		else
		{
			word[(*ind)++] = c;
			word[*ind] = 0;
		}
	}
}

char **MakeDictionary(FILE *fp, char **dict, int *size_dict, int *index, int **count_words, int *status)
{
	int c, countOfWords = 0, ind = 0;
	int sta = 0;
	char word[256];
	int(*fff)(FILE *fp);
	char **work;
	//----------------------------------------------
	if (fp == stdin)
		fff = my_fgetc;
	else
		fff = fgetc;
	//----------------------------------------------
	rewind(fp);
	while ((c = fff(fp)) != EOF)
	{
		ComputeWord(c, word, &ind, &sta);

		if (sta == 2)
		{
			countOfWords++;
			ToLower(word);
			if ((work = AddToDictionary(dict, size_dict, index, count_words, word)) == NULL)
			{
				*status = 0;
				return dict;
			}
			else
			{
				*status = 1;
				dict = work;
			}
		}
	}
	if (sta == 1)
	{
		countOfWords++;
		ToLower(word);
		if ((work = AddToDictionary(dict, size_dict, index, count_words, word)) == NULL)
		{
			*status = 0;
			return dict;
		}
		else
		{
			*status = 1;
			dict = work;
		}
	}
	return dict;
}

char **AddToDictionary(char **dict, int *size_dict, int *index, int **count_words, char *word)
{
	int ind;
	char **work_dict;
	int *work_count_words;

	if (dict == NULL)
	{
		if ((work_dict = (char **)malloc(*size_dict*sizeof(char *)+1)) == NULL)
			return NULL;
		dict = work_dict;
	}
	if (*count_words == NULL)
	{
		if ((work_count_words = (int *)malloc(*size_dict*sizeof(int))) == NULL)
		{
			return NULL;
		}
		*count_words = work_count_words;
	}
	if ((ind = FindInToDictionary(dict, *index, word)) != -1)
	{
		(*count_words)[ind]++;
		return dict;
	}
	if ((dict[*index] = (char *)malloc(sizeof(char)*(strlen(word) + 1))) == NULL)
	{
		return NULL;
	}

	strcpy(dict[*index], word);
	(*count_words)[*index] = 1;
	(*index)++;

	if (*index == *size_dict)
	{
		*size_dict += DELTA_SIZE;
		if ((work_dict = (char **)realloc(dict, *size_dict* sizeof(char *))) == NULL)
		{
			return NULL;
		}

		dict = work_dict;
		if ((work_count_words = (int *)realloc(*count_words, *size_dict*sizeof(int))) == NULL)
		{
			return NULL;
		}
		*count_words = work_count_words;
	}
	return dict;
}

int FindInToDictionary(char **dict, int index_dict, char *word)
{
	for (size_t i = 0; i < index_dict; i++)
	{
		if (strcmp(dict[i], word) == 0)
			return i;
	}

	return -1;
}

int my_fgetc(FILE *fp)
{
	int ch;
	ch = _getche();

	if (ch == 26)
		return EOF;

	if (ch == '\r')
	{
		printf("\n");
		ch = '\n';
	}
	return ch;
}

void ToLower(unsigned char *line)//работи за кирилица!
{
	int len;
	len = strlen(line);
	for (size_t i = 0; i < len; i++)
	{
		if (line[i] >= 0xC0 && line[i] <= 0xDF)
			line[i] = line[i] + 0x20;
		else
			line[i] = (unsigned char)tolower(line[i]);
	}
}

void ToUpper(unsigned char *line)//работи за кирилица!
{
	int i, len;
	len = strlen(line);
	for (i = 0; i < len; i++)
	{
		if (line[i] >= 0xE0)
			line[i] = line[i] - 0x20;
		else
			line[i] = (unsigned char)toupper(line[i]);
	}
}

void DeleteDictionary(char **dict, int *count_words, int index_dict)
{
	if (dict) //aко речника не е празен
	{
		for (size_t i = 0; i < index_dict; i++)
		{
			free(dict[i]); //пъвро освобождаваме всяка дума, после целия речник
		}

		free(dict);
		free(count_words);
	}
}

void PrintDictionary(char **dict, int index_dict, int *count_words, int status)
{
	if (index_dict) //ако има думи в речника
		printf("   РЕЧНИК:\n");

	printf("  БРОЙ | ДУМИ\n");
	for (size_t i = 0; i < index_dict; i++)
	{
		printf("\n  %d\t %s", count_words[i], dict[i]);
		if (i > 0 && !(i % 128))
		{
			printf("\n--Press any key to continue--\n");
			_getch();
		}
	}
	if (!index_dict)
		printf("\nРечникът е празен !!!\n\n");
	else
	{
		printf("\nБрой на думите в речника -> %d", index_dict);

		if (!status)
			printf(" - речника е недовършен - липса на достатъчнo ПАМЕТ !!!\n\n");
		else
			printf("\n\n");
	}
}

void SearchInDictionary(char **dict, int index_dict)
{
	char word[64];//масив в който ще запишем думата която търсим
	printf("Въведете дума за търсене в речника: ");
	scanf("%s", &word);
	ToLower(word); //правим я само с малки букви, защото в речника пазим думите с малки букви
	int found = 0;

	for (size_t i = 0; i < index_dict; i++)//цикъл с който минаваме всички думи в речника
	{
		if (strcmp(dict[i], word) == 0) //сравняваме въведената дума с тези в речника
		{
			found = 1; //ако се среща вдигаме влага
		}
	}

	if (found == 1)
		printf("Думата *%s* се среща в речникa\n", word);
	else
		printf("Думата *%s* не се среща в речникa\n", word);
}

void SwapPointers(const char **firstString, const char **secondString) //функцията която разменя адресите на 2 указателя
{
	const char *temp = *secondString;
	*secondString = *firstString;
	*firstString = temp;
}

void SortByAlphabet(char **dict, int size, int *countOfWords)
{
	int temp2;
	int swap = 1;

	while (swap)
	{
		swap = 0;
		for (size_t i = 0; i < size; i++) //взимаме 1 дума
		{
			for (size_t j = i + 1; j < size; j++) //за останалите думи
			{
				if (strcmp(dict[i], dict[j]) > 0)
				{
					SwapPointers(&dict[i], &dict[j]); //разменяме адресите на думите

					temp2 = countOfWords[i]; //после сортираме и срещането спрямо азбуката
					countOfWords[i] = countOfWords[j];
					countOfWords[j] = temp2;

					swap = 1;
				}
			}
		}
	}
	printf("Сортирането e успешно.\n");
}

void SortByFrequency(char **dict, int *values, int size)
{
	int temp;
	int swap = 1;

	while (swap)
	{
		swap = 0;
		for (size_t i = 0; i < size - 1; i++)
		{
			if (values[i] > values[i + 1])
			{
				temp = values[i];
				values[i] = values[i + 1];
				values[i + 1] = temp;

				SwapPointers(&dict[i], &dict[i + 1]); //разменяме адресите на думите

				swap = 1;
			}
		}
	}
	printf("Сортирането е успешно.\n");
}

void SaveDictInFile(char **dict, int *values, int size, char *inputFName, int mode)
{
	char outputFileName[NAMES];
	FILE *fp_out;

	printf("\nВъведете името на файла за записване на речника: ");
	scanf("%s", outputFileName);

	if (!(fp_out = fopen(outputFileName, "w")))
	{
		printf("Проблем със файлът за записване.\n");
		system("pause");
		return;
	}
	//записва на 1вия ред от файла от къде е създаден резника спрямо кой избор за обработка сме избрали
	if (mode == 1)
		fprintf(fp_out, "Речника е създаден от клавиатурата\n");
	else
		fprintf(fp_out, "Речника е създаден от: \"%s\"\n", inputFName);

	for (size_t i = 0; i < size; i++) //минаваме през всички думи
	{
		fprintf(fp_out, "%s - %d\n", dict[i], values[i]); //записваме във файла димите и колко пъти се среща всяка
	}

	if (fp_out) //затваря отворения файл, ако няма такъв нищо не прави
		fclose(fp_out);

	printf("\nЗаписването е успешно!\n");
}

void SaveDictInCNTFile(char **dict, int *values, int size, char *inputFName, int mode)
{
	char outputFileName[NAMES];
	char cntFile[NAMES];

	FILE *fp_cnt = NULL;
	FILE *fp_out = NULL;

	printf("\nВъведете името на файла за записване на речника: ");
	scanf("%s", outputFileName);

	if (!(fp_out = fopen(outputFileName, "w")))
	{
		printf("Проблем със файлът за записване.\n");
		system("pause");
		return;
	}
	//записва на 1вия ред от файла от къде е създаден резника спрямо кой избор за обработка сме избрали
	if (mode == 1)
		fprintf(fp_out, "Речника е създаден от клавиатурата\n");
	else
		fprintf(fp_out, "Речника е създаден от: \"%s\"\n", inputFName);

	strcpy(cntFile, outputFileName);

	if (strchr(cntFile, '.')) //kогато намерим '.' в името на фйала 
		strcat(cntFile, ".cnt"); //конкатенираме след нея името на файла с.cnt

	fp_cnt = fopen(cntFile, "w"); //отваряме cnt файла за писане
	for (size_t i = 0; i < size; i++)
	{
		fprintf(fp_out, "%s\n", dict[i]); //записва думата в текстовия файл
		fprintf(fp_cnt, "%d\n", values[i]); //записва честотата на думата в .cnt файла
	}

	if (fp_out)
		fclose(fp_out);
	if (fp_cnt) //затваря отворения файл, ако няма такъв нищо не прави
		fclose(fp_cnt);

	printf("\nЗаписването е успешно!\n");
}

void CodingFile(char **dict, int index, char *inputFName)
{
	char outputFileName[NAMES];
	char word[256]; //в този стринг ще записваме дума, взета от файла
	char temp[256]; //после ще я прекопираме и ще я направим с малки букви
	s_word status = None;
	int c;

	FILE *fp_in;
	FILE *fp_out;


	if ((fp_in = fopen(inputFName, "r")) == NULL)
	{
		printf("\nФайлът \"%s\" не може да се отвори!", inputFName);
		system("pause");
		return;
	}

	printf("\nВъведете файл за записване на кодирания текст: ");
	scanf("%s", outputFileName);

	if (!(fp_out = fopen(outputFileName, "w")))
	{
		printf("Проблем със файлът за записване.\n");
		system("pause");
		return;
	}

	rewind(fp_in);

	while ((c = fgetc(fp_in)) != EOF)
	{
		status = CompWord(c, word, status);
		if (status == End_Word)
		{
			strcpy(temp, word);//копираме съдържанието на word в temp
			ToLower(temp); //правим я в ToLower, в случай че съдържа главни букви за да я срваним правилно
			for (size_t i = 0; i < index; i++)
			{
				ToLower(dict[i]);
				if ((strcmp(temp, dict[i])) == 0)
				{
					fprintf(fp_out, "%d", i);
				}
			}
		}
		if (IsDelimiter(delimiters_word, c)) //ако е разделител просто го презаписваме в новия файл
		{
			fprintf(fp_out, "%c", c);
		}
	}

	if (fp_out)
		fclose(fp_out);

	printf("\nКодирането е успешно!\n");
	system("pause");
}

s_word CompWord(int c, char *word, s_word status)
{
	static int index = 0;
	int ch = c;

	if (status == End_Word) // ако е в крайно състояние
	{
		status = None; //връщаме автомата в начално
		index = 0;
	}
	switch (status)
	{
	case None:
		if (!IsDelimiter(delimiters_word, ch)) //ако не е разделите
		{
			status = In_Word; //преминаваме в следващото състояние
			word[index++] = c; //записваме текущия символ в символния масив
		}
		break;

	case In_Word:
		if (IsDelimiter(delimiters_word, ch)) //ако е разделител
		{
			status = End_Word; //минаваме в крайно състояние
			word[index] = 0;
		}
		else
		{
			word[index++] = c; //записваме сегашния символ
		}
	}
	return status; //връщаме състоянието на автомата
}

void DecodingFile(char **dict, int index, char *inputFName)
{
	char decodeFName[NAMES], outputFileName[NAMES];
	int state = 0, c;
	int num = 0;
	int flag = 0;

	FILE *fp_out;
	FILE *decode;

	printf("\nВъведете файл за декодиране: ");
	scanf("%s", decodeFName);
	if ((decode = fopen(decodeFName, "r")) == NULL)
	{
		printf("Файлът \"%s\" не може да се отвори!", decodeFName);
		system("pause");
		return;
	}

	printf("\nВъведете файл за записване на кодирания текст: ");
	scanf("%s", outputFileName);
	if ((fp_out = fopen(outputFileName, "w")) == NULL)
	{
		printf("\nФайлът \"%s\" не може да се отвори!", outputFileName);
		system("pause");
		return;
	}

	while ((c = fgetc(decode)) != EOF)
	{
		state = IsNumber(c, state);
		if (state == 1)
		{
			BuildNum(c, &num, &flag);
		}

		if (state == 2)
		{
			fprintf(fp_out, "%s", dict[num]);
			flag = 0; //сваляме флагът
			num = 0; //нулираме числото
		}
		if (IsDelimiter(delimiters_word, c))//ако е разделител просто го презаписваме в новия файл
			fprintf(fp_out, "%c", c);
	}

	if (fp_out)
		fclose(fp_out);
	if (decode)
		fclose(decode);

	printf("\nДекодирането е успешно!\n");
	system("pause");
}

void BuildNum(int c, int *number, int *flag) //функция която сглобява числата
{
	if (*flag == 0)
	{
		*number = (c - '0'); //парсваме символа (char) в int
		*flag = 1;
	}
	else
	{
		*number *= 10;
		*number += (c - '0');
	}
}

int IsNumber(int c, int state) //автоматна функция за намиране на цели числа
{
	size_t index = 0;

	if (state == 2)
		state = 0;

	switch (state)
	{
	case 0:
		if (isdigit(c))
		{
			state = 1;
		}
		break;
	case 1:
		if (!isdigit(c)) //докато прочетените символи са числа, не променяме състоянието на автомата
			state = 2;

		break;
	}
	return state;
}