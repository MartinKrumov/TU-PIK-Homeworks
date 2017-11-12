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
		printf("�������� ��� �� ����� %s: ", text);
		err = scanf("%s", name_file);
	} while (err != 1);

	if ((fp = fopen(name_file, mode)) == NULL)
	{
		printf("\n������ <%s> �� ���� �� �� ������ - error No % d !!!\n", name_file, errno);
		printf("������ No %d %s\n", errno, strerror(errno));
		system("pause");
	}
	else
	{
		printf("\n������ <%s> � ������� ������� �� ��������� !\n\n", name_file);
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
			return 1; //�������� � � ����������
	}
	return 0;
}

void ComputeWord(int c,        //--- ����� ������, ����� �� ��������� ---
	char *word,   //--- �������� ����� �� �������� �� ����
	int *ind,     //--- ������ ������� � ������
	int *status)   //--- ����� ������ �� ����������� �������	
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

void ToLower(unsigned char *line)//������ �� ��������!
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

void ToUpper(unsigned char *line)//������ �� ��������!
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
	if (dict) //a�� ������� �� � ������
	{
		for (size_t i = 0; i < index_dict; i++)
		{
			free(dict[i]); //����� ������������� ����� ����, ����� ����� ������
		}

		free(dict);
		free(count_words);
	}
}

void PrintDictionary(char **dict, int index_dict, int *count_words, int status)
{
	if (index_dict) //��� ��� ���� � �������
		printf("   ������:\n");

	printf("  ���� | ����\n");
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
		printf("\n�������� � ������ !!!\n\n");
	else
	{
		printf("\n���� �� ������ � ������� -> %d", index_dict);

		if (!status)
			printf(" - ������� � ���������� - ����� �� ���������o ����� !!!\n\n");
		else
			printf("\n\n");
	}
}

void SearchInDictionary(char **dict, int index_dict)
{
	char word[64];//����� � ����� �� ������� ������ ����� ������
	printf("�������� ���� �� ������� � �������: ");
	scanf("%s", &word);
	ToLower(word); //������ � ���� � ����� �����, ������ � ������� ����� ������ � ����� �����
	int found = 0;

	for (size_t i = 0; i < index_dict; i++)//����� � ����� �������� ������ ���� � �������
	{
		if (strcmp(dict[i], word) == 0) //���������� ���������� ���� � ���� � �������
		{
			found = 1; //��� �� ����� ������� �����
		}
	}

	if (found == 1)
		printf("������ *%s* �� ����� � ������a\n", word);
	else
		printf("������ *%s* �� �� ����� � ������a\n", word);
}

void SwapPointers(const char **firstString, const char **secondString) //��������� ����� ������� �������� �� 2 ���������
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
		for (size_t i = 0; i < size; i++) //������� 1 ����
		{
			for (size_t j = i + 1; j < size; j++) //�� ���������� ����
			{
				if (strcmp(dict[i], dict[j]) > 0)
				{
					SwapPointers(&dict[i], &dict[j]); //��������� �������� �� ������

					temp2 = countOfWords[i]; //����� ��������� � ��������� ������ ��������
					countOfWords[i] = countOfWords[j];
					countOfWords[j] = temp2;

					swap = 1;
				}
			}
		}
	}
	printf("����������� e �������.\n");
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

				SwapPointers(&dict[i], &dict[i + 1]); //��������� �������� �� ������

				swap = 1;
			}
		}
	}
	printf("����������� � �������.\n");
}

void SaveDictInFile(char **dict, int *values, int size, char *inputFName, int mode)
{
	char outputFileName[NAMES];
	FILE *fp_out;

	printf("\n�������� ����� �� ����� �� ��������� �� �������: ");
	scanf("%s", outputFileName);

	if (!(fp_out = fopen(outputFileName, "w")))
	{
		printf("������� ��� ������ �� ���������.\n");
		system("pause");
		return;
	}
	//������� �� 1��� ��� �� ����� �� ���� � �������� ������� ������ ��� ����� �� ��������� ��� �������
	if (mode == 1)
		fprintf(fp_out, "������� � �������� �� ������������\n");
	else
		fprintf(fp_out, "������� � �������� ��: \"%s\"\n", inputFName);

	for (size_t i = 0; i < size; i++) //�������� ���� ������ ����
	{
		fprintf(fp_out, "%s - %d\n", dict[i], values[i]); //��������� ��� ����� ������ � ����� ���� �� ����� �����
	}

	if (fp_out) //������� ��������� ����, ��� ���� ����� ���� �� �����
		fclose(fp_out);

	printf("\n����������� � �������!\n");
}

void SaveDictInCNTFile(char **dict, int *values, int size, char *inputFName, int mode)
{
	char outputFileName[NAMES];
	char cntFile[NAMES];

	FILE *fp_cnt = NULL;
	FILE *fp_out = NULL;

	printf("\n�������� ����� �� ����� �� ��������� �� �������: ");
	scanf("%s", outputFileName);

	if (!(fp_out = fopen(outputFileName, "w")))
	{
		printf("������� ��� ������ �� ���������.\n");
		system("pause");
		return;
	}
	//������� �� 1��� ��� �� ����� �� ���� � �������� ������� ������ ��� ����� �� ��������� ��� �������
	if (mode == 1)
		fprintf(fp_out, "������� � �������� �� ������������\n");
	else
		fprintf(fp_out, "������� � �������� ��: \"%s\"\n", inputFName);

	strcpy(cntFile, outputFileName);

	if (strchr(cntFile, '.')) //k����� ������� '.' � ����� �� ����� 
		strcat(cntFile, ".cnt"); //������������� ���� ��� ����� �� ����� �.cnt

	fp_cnt = fopen(cntFile, "w"); //�������� cnt ����� �� ������
	for (size_t i = 0; i < size; i++)
	{
		fprintf(fp_out, "%s\n", dict[i]); //������� ������ � ��������� ����
		fprintf(fp_cnt, "%d\n", values[i]); //������� ��������� �� ������ � .cnt �����
	}

	if (fp_out)
		fclose(fp_out);
	if (fp_cnt) //������� ��������� ����, ��� ���� ����� ���� �� �����
		fclose(fp_cnt);

	printf("\n����������� � �������!\n");
}

void CodingFile(char **dict, int index, char *inputFName)
{
	char outputFileName[NAMES];
	char word[256]; //� ���� ������ �� ��������� ����, ����� �� �����
	char temp[256]; //����� �� � ����������� � �� � �������� � ����� �����
	s_word status = None;
	int c;

	FILE *fp_in;
	FILE *fp_out;


	if ((fp_in = fopen(inputFName, "r")) == NULL)
	{
		printf("\n������ \"%s\" �� ���� �� �� ������!", inputFName);
		system("pause");
		return;
	}

	printf("\n�������� ���� �� ��������� �� ��������� �����: ");
	scanf("%s", outputFileName);

	if (!(fp_out = fopen(outputFileName, "w")))
	{
		printf("������� ��� ������ �� ���������.\n");
		system("pause");
		return;
	}

	rewind(fp_in);

	while ((c = fgetc(fp_in)) != EOF)
	{
		status = CompWord(c, word, status);
		if (status == End_Word)
		{
			strcpy(temp, word);//�������� ������������ �� word � temp
			ToLower(temp); //������ � � ToLower, � ������ �� ������� ������ ����� �� �� � ������� ��������
			for (size_t i = 0; i < index; i++)
			{
				ToLower(dict[i]);
				if ((strcmp(temp, dict[i])) == 0)
				{
					fprintf(fp_out, "%d", i);
				}
			}
		}
		if (IsDelimiter(delimiters_word, c)) //��� � ���������� ������ �� ������������ � ����� ����
		{
			fprintf(fp_out, "%c", c);
		}
	}

	if (fp_out)
		fclose(fp_out);

	printf("\n���������� � �������!\n");
	system("pause");
}

s_word CompWord(int c, char *word, s_word status)
{
	static int index = 0;
	int ch = c;

	if (status == End_Word) // ��� � � ������ ���������
	{
		status = None; //������� �������� � �������
		index = 0;
	}
	switch (status)
	{
	case None:
		if (!IsDelimiter(delimiters_word, ch)) //��� �� � ���������
		{
			status = In_Word; //����������� � ���������� ���������
			word[index++] = c; //��������� ������� ������ � ��������� �����
		}
		break;

	case In_Word:
		if (IsDelimiter(delimiters_word, ch)) //��� � ����������
		{
			status = End_Word; //�������� � ������ ���������
			word[index] = 0;
		}
		else
		{
			word[index++] = c; //��������� �������� ������
		}
	}
	return status; //������� ����������� �� ��������
}

void DecodingFile(char **dict, int index, char *inputFName)
{
	char decodeFName[NAMES], outputFileName[NAMES];
	int state = 0, c;
	int num = 0;
	int flag = 0;

	FILE *fp_out;
	FILE *decode;

	printf("\n�������� ���� �� ����������: ");
	scanf("%s", decodeFName);
	if ((decode = fopen(decodeFName, "r")) == NULL)
	{
		printf("������ \"%s\" �� ���� �� �� ������!", decodeFName);
		system("pause");
		return;
	}

	printf("\n�������� ���� �� ��������� �� ��������� �����: ");
	scanf("%s", outputFileName);
	if ((fp_out = fopen(outputFileName, "w")) == NULL)
	{
		printf("\n������ \"%s\" �� ���� �� �� ������!", outputFileName);
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
			flag = 0; //������� ������
			num = 0; //�������� �������
		}
		if (IsDelimiter(delimiters_word, c))//��� � ���������� ������ �� ������������ � ����� ����
			fprintf(fp_out, "%c", c);
	}

	if (fp_out)
		fclose(fp_out);
	if (decode)
		fclose(decode);

	printf("\n������������ � �������!\n");
	system("pause");
}

void BuildNum(int c, int *number, int *flag) //������� ����� �������� �������
{
	if (*flag == 0)
	{
		*number = (c - '0'); //�������� ������� (char) � int
		*flag = 1;
	}
	else
	{
		*number *= 10;
		*number += (c - '0');
	}
}

int IsNumber(int c, int state) //��������� ������� �� �������� �� ���� �����
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
		if (!isdigit(c)) //������ ����������� ������� �� �����, �� ��������� ����������� �� ��������
			state = 2;

		break;
	}
	return state;
}