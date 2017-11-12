#define _CRT_SECURE_NO_WARNINGS
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include "automat_f.h"
#include <conio.h>
#include <stdio.h>
#include <ctype.h>


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
		printf("������ No %d %s", errno, strerror(errno));
		system("pause");
	}
	else
	{
		printf("\n������ <%s> � ������� ������� �� ��������� !\n\n", name_file);
		system("pause");
	}
	return fp;
}

int IsDelimiter(int c)
{
	int i = 0;
	int delimiters[] = { '"', '\'', ' ', ',', '.', '?', '!', ':', ';', '/', '\\', '*',
		'<', '>', '(', ')', '{', '}', '[', ']', '\n', '\t', '\r', EOF, 0  };

	while (delimiters[i])
	{
		if (c == delimiters[i++])
			return 1; //--- �������� � � ���������� ---
	}
	return 0;
}

int IsDelim(int c)
{
	int i = 0;
	int delimiters[] = { '"', '\'', ' ', ',', '?', '!', ':', ';', '/', '\\', '*',
		'<', '>', '(', ')', '{', '}', '[', ']', '\n', '\t', '\r', EOF, 0  };

	while (delimiters[i])
	{
		if (c == delimiters[i++])
			return 1; //--- �������� � � ���������� ---
	}
	return 0;
}

s_word ComputeWord(int c,	// c => ����� ������, ����� �� ��������� 
	char *word,				// *word => �������� ����� �� �������� �� ����
	s_word status)			//status => ����� ������ �� ����������� �������
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
		if (!IsDelimiter(ch)) //��� �� � ���������
		{
			status = In_Word; //����������� � ���������� ���������
			word[index++] = c; //��������� ������� ������ � ��������� �����
		}
		break;

	case In_Word:
		if (IsDelimiter(ch)) //��� � ����������
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

int GetWords(FILE *fp, FILE *fp_out)
{
	int c, countOfWords = 0;
	s_word status = None;
	char word[256];

	rewind(fp); //����� �������� ������� ��� �������� �� �����(�� �� ���� ��� �� �� �������)

	do
	{
		c = fgetc(fp); //������� 1 ������
		status = ComputeWord(c, word, status); //������� ��������� ComputeWord � ������� �������� �������� � ������

		if (status == End_Word && c != '\n') 
		{
			countOfWords++;
			printf("word => %s\n", word); //�������� �� ������
			fprintf(fp_out, "%s\n", word); //������� ��� �����
		}
	} while (c != EOF); //������ �� ������ ���� �� �����

	return countOfWords; //����� ���� �� ������ ��� �����
}

int isAlpha(int c) //��������� ���� ������� � �����(����� ���� �� �������)
{
	return
		(c >= 65 && c <= 90) || // c >= A && c <= Z
		(c >= 97 && c <= 122) || // ��������� ���� ������� ������ � �/� � � z
		(c >= 192 && c <= 255); // ��������� ���� ������� ������ � �/� � �� � � �� � �� � (����������)

}

int CountOfSentences(FILE *fp_in)
{
	int c;
	size_t countOfSenetences = 0;
	int state = 1;

	rewind(fp_in);

	while ((c = fgetc(fp_in)) != EOF)
	{
		switch (state)
		{
		case 1:
			if (isAlpha(c)) //��������� ���� � �����
				state = 2;//��������� � ���������� ���������
			break;
		case 2:
			switch (c) //��������� ���� ������� � ����������� ����
			{
			case '.':
			case '!':
			case '?':
			case ':':
				countOfSenetences++;
				state = 1; //����� � ������� ���������
				break;
			}
			break;
		}
	}
	return countOfSenetences; //����� ���� �� �����������
}

void PrintSentences(FILE *fp_in, FILE *fp_out)
{
	int c;
	int mark = 0;
	int flag = 0;
	int letter = 0;

	rewind(fp_in);

	while ((c = fgetc(fp_in)) != EOF) //���� ������ �� ������ ������ �� ������ EOF
	{
		if (isAlpha(c)) //��� ���������� ������ � �����
		{
			mark = 0;
			flag = 0;
			letter = 1;
		}

		if (mark == 0 && letter == 1)
		{
			printf("%c", c);
			fprintf(fp_out, "%c", c);
		}
		if (c == '.' || c == '!' || c == '?' || c == ':')
		{
			mark = 1;
			letter = 0;
		}
		if (mark == 1 && flag == 0)
		{
			fprintf(fp_out, "\n");
			fprintf(stdout, "\n");
			mark = 0;
			flag = 1;
		}
	}
}

void CopyString(char *newString, const char *oldStirng, int start, int end)//������ ������ �� ������ ������ ������, � �����
{
	int i = start;
	while (i <= end)
	{
		newString[i - start] = oldStirng[i];
		i++;
	}
	newString[i] = '\0'; //'���������' ������� � ����������� ����
}

char** Split(char *line, int *strings)
{
	char **res = (char**)malloc(64 * sizeof(char*));//�������� ��������� ����� �� ������ �� ���������

	int delimiter = 0;
	int count = 0;

	size_t size = strlen(line); //������� ��������� �� ���������� ���
	for (size_t i = 0; i < size; i++) //�������� ���� ���������
	{
		if (IsDelim(line[i])) //����������� ���� ������� ������ � ����������
		{
			res[count] = (char*)calloc((64), sizeof(char));// �������� ����� �� ���������(������)
			if (res[count] == NULL)
			{
				printf("Problem Allocating Memory !\n");//��� � ��������� ���������� �� �����
				return 1;
			}
			CopyString(res[count], line, delimiter, i - 1); //������ �������� ������ � ��� ��� �� ����� �����������
			count++;

			while (IsDelim(line[i]))//��������� ������ ������ ��� �������������� �����������
			{
				i++;
			}
			delimiter = i;
		}
		else if (i == size - 1) //������ �� ������ ���� �� �������
		{
			res[count] = (char*)calloc((64), sizeof(char));
			if (res[count] == NULL)
			{
				printf("Problem Allocating Memory!\n");
				return 1;
			}
			CopyString(res[count], line, delimiter, i);

			count++;
		}
	}

	*strings = count; //����� ���� �� ������ ���������
	return res; //����� ����� �� ���������
}

void IntConstant(FILE *fp_in)
{
	char digit[256];
	char temp[256]; // ��������� ���������� ���
	char **numbers;

	int state = 0;//����������� �� ��������
	int index = 0;
	int countOfDigits = 0;

	int flag = 0;

	rewind(fp_in);
	while (fgets(temp, sizeof(temp), fp_in)) //������� 1 ��� ��� �� ��������� ����
	{
		size_t size; //���� �� ����������� ���������
		numbers = Split(temp, &size);// ��������� ���� �� ���������

		for (size_t row = 0; row < size; row++) 
		{
			flag = 0;
			size_t len = strlen(numbers[row]); //����� ��������� �� ������� ������

			for (size_t i = 0; i <= len; i++)//�����, ���� ����� �������� ���� ���������
			{
				switch (state)
				{
				case 0:
					if (isdigit(numbers[row][i])) //����������� ���� ������� � �����
					{
						digit[index++] = numbers[row][i]; //��������� �� � ������
						state = 2; //������� �����������
					}				
					break;

				case 1:
					if (numbers[row][i] == '+' || numbers[row][i] == '-') //���� ������� � +/-
					{
						if (isdigit(numbers[row][i + 1])) //��� ����� ����������� ���� ������ ������ � �����
						{
							digit[index++] = numbers[row][i];
							state = 2; //������� ����������� �� ��������
						}
					}
					break;

				case 2:
					if (isdigit(numbers[row][i]) || (numbers[row][i] == 'L' || numbers[row][i] == 'l'))// ����������� �� ����� ��� L/l (long)
					{
						digit[index++] = numbers[row][i]; //��������� � �������/������� � ������ digit � ����������� �������
					}
					else if (numbers[row][i] == '.')
					{
						flag = 1; //������� ����� ��� ������� .
						digit[index++] = numbers[row][i];
					}
					else 
					{
						if (i == len && flag == 0) //����������� ���� �� � ����������
						{
							
							digit[index] = '\0';//������������� �������
							printf("%s\n", digit);// ���������� �� ������ �������

							countOfDigits++;
							digit[0] = '\0'; // �������� ������ � �������
							index = 0;
							state = 0; //������� � ������� ���������
						}
						else
						{
							digit[0] = '\0'; //�������� ������
							index = 0;
							state = 0;
						}
					}
					break;
				}
			}
		}

		for (size_t i = 0; i < size; i++) 
		{
			free(numbers[i]);//������������� ������� �� ����� ��������
		}
		free(numbers);//������������� ������� �� ������ �� ���������
	}

	printf("���� �� ������ ��������� �: %d\n", countOfDigits);
}

void RealConstant(FILE *fp_in)
{
	char digit[256];
	char temp[256]; // ��������� ���������� ���
	char **numbers;
	int state = 0; //��������� �� ��������
	int index = 0;

	int countOfNumbers = 0;
	int flag = 0;
	rewind(fp_in);

	while (fgets(temp, sizeof(temp), fp_in)) 
	{
		size_t size;
		numbers = Split(temp, &size);
		for (size_t row = 0; row < size; row++)
		{

			size_t len = strlen(numbers[row]);
			for (size_t i = 0; i <= len; i++)
			{
				switch (state)
				{
				case 0:
					flag = 0;
					if (isdigit(numbers[row][i])) //����������� ���� ������� � �����
					{
						digit[index++] = numbers[row][i]; //��������� �� � ������
						state = 1; //������� �����������
					}
					else if (numbers[row][i] == '+' || numbers[row][i] == '-') //���� ������� � +/-
					{
						if (isdigit(numbers[row][i + 1])) //��� ����� ����������� ���� ������ ������ � �����
						{
							digit[index++] = numbers[row][i];
							state = 1; //������� ����������� �� ��������
						}
					}
					break;

				case 1:
					if (isdigit(numbers[row][i])) //����������� �� �����
					{
						digit[index++] = numbers[row][i];
					}
					else if (numbers[row][i] == '.') //�������� �� ��������� �������
					{
						digit[index++] = numbers[row][i];
						state = 2; // ����������� � ���������� ����������� ��� ���
					}
					else if (numbers[row][i] == 'e' || numbers[row][i] == 'E')//����������� �� e/E 
					{
						digit[index++] = numbers[row][i];
						state = 3; //��� � �������� ������� � 3�� ���������
					}
					else
					{
						digit[0] = '\0'; //� �������� ������ �������� ������ � �������
						index = 0;
						state = 0; //������� � ������� ���������
					}
					break;

				case 2:
					if (isdigit(numbers[row][i]))
					{
						digit[index++] = numbers[row][i];
						flag = 1; //������� ����� ��� ������� �����
					}
					else if (numbers[row][i] == 'e' || numbers[row][i] == 'E') //����������� �� e/E 
					{
						digit[index++] = numbers[row][i];
						state = 3; // ��� � �������� ������� � 3�� ���������
					}
					else
					{
						if (i == len)//����������� ���� �� � ����������
						{
							if (flag == 1)
							{
								digit[index] = '\0';
								printf("%s\n", digit); // ���������� ������

								countOfNumbers++;
								digit[0] = '\0'; // �������� ������ � �������
								index = 0;
								state = 0; //������� � ������� ���������
							}
							else
							{
								digit[0] = '\0'; //���� ��-����
								index = 0;
								state = 0;
							}
						}
					}
					break;

				case 3:
					if (numbers[row][i] == '+' || numbers[row][i] == '-') //����������� �� +/-
					{
						if (isdigit(numbers[row][i + 1])) //����������� ���� ������ � �����
						{
							digit[index++] = numbers[row][i];
							state = 4; //������� � ��������� 4
						}
					}
					else if (isdigit(numbers[row][i])) //���� � ����� 
					{
						digit[index++] = numbers[row][i];
						state = 4;
					}
					else //� ������� ������
					{
						state = 4;
						digit[0] = '\0'; //�������� ������
						index = 0;
					}
					break;

				case 4:
					if (isdigit(numbers[row][i]))
					{
						digit[index++] = numbers[row][i];
						break;
					}
					else if (i == len)//����������� ���� �� � ����������
					{
						if (flag == 1)
						{
							digit[index] = '\0';
							printf("%s\n", digit); // ���������� ������

							countOfNumbers++;
							digit[0] = '\0'; // �������� ������ � �������
							index = 0;
							state = 0; //������� � ������� ���������
						}
						else
						{
							digit[0] = '\0'; //���� ��-����
							index = 0;
							state = 0;
						}
					}
					break;
				default:
					break;
				}
			}	
		}

		for (size_t i = 0; i < size; i++) 
		{
			free(numbers[i]); //������������� ������� �� ����� ��������
		}
		free(numbers);//������������� ������� �� ������ �� ���������
	}

	printf("���� �� �������� ��������� �: %d\n", countOfNumbers);
}

int ZabraneniSimvoli(char symbol)
{
	switch (symbol)
	{
	case '|': return 1;
	case '/': return 1;
	case '\\': return 1;
	case '"': return 1;
	case '<': return 1;
	case '>': return 1;
	case ':': return 1;
	case '?': return 1;
	case '*': return 1;
	default: break;
	}
	return 0;
}

void ValidDirectory(char *path)
{
	int state = 0; // ����������� �� ��������
	int index = 0;
	char folder[64];

	size_t slashes = 0;
	int flag; 

	size_t length = strlen(path);
	for (size_t i = 0; i < length - 1; i++)
	{
		char ch = path[i];
		switch (state)
		{
		case 0:
			if (isalpha(ch)) // �������� ���� ������� � ��� �� ����
			{
				if (path[i + 1] != ':')
				{
					flag = 1;
				}
				state = 1;
			}
			else
				flag = 1;
			break;

		case 1:
			if (ch == ':')
				state = 2;
			else
				flag = 1;
			break;

		case 2:
			if (ch == '\\')
				state = 3;
			else
				state = 3;
			break;

		case 3:
			if (!ZabraneniSimvoli(ch) || (ch >= '0' && ch <= '9') || ch == '-' || isalpha(ch))
			{
				folder[index++] = ch; //���� ���� �� ����� ����� ������� � ����� �� ������������
				if (index >= 8) //��� ����� �� ����� �� 8
				{
					flag = 1;//������� �����
					index = 0;//�������� ������� � ������� �� ������������
				}
				break;
			}
			else
			{
				if (ch == '\\') //��� ��� ��������� \������� � ������������
				{
					slashes++; // ����� ���� �� ����� � ����� ����������
					index = 0; // ������� �� ������������ ������ folder 
					state = 4;
					break;
				}
			}
			return 0;

		case 4:
			if (ZabraneniSimvoli(ch))
				return 0;
			else
				state = 3;
			break;

		default:
			break;
		}
	}

	if (slashes >= 8 || flag == 1)
	{
		printf("��������� ������ ����.\n");
	}
	else
	{
		printf("������������ � �������.\n");
	}
}