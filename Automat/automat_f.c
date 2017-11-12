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
		printf("Въведете име на файла %s: ", text);
		err = scanf("%s", name_file);
	} while (err != 1);

	if ((fp = fopen(name_file, mode)) == NULL)
	{
		printf("\nФайлът <%s> Не може да се отвори - error No % d !!!\n", name_file, errno);
		printf("ГРЕШКА No %d %s", errno, strerror(errno));
		system("pause");
	}
	else
	{
		printf("\nФайлът <%s> е отворен успешно за обработка !\n\n", name_file);
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
			return 1; //--- символът с е разделител ---
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
			return 1; //--- символът с е разделител ---
	}
	return 0;
}

s_word ComputeWord(int c,	// c => текущ символ, който се анализира 
	char *word,				// *word => символен масив за отделяне на дума
	s_word status)			//status => текущ статус на автоматната функция
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
		if (!IsDelimiter(ch)) //ако не е разделите
		{
			status = In_Word; //преминаваме в следващото състояние
			word[index++] = c; //записваме текущия символ в символния масив
		}
		break;

	case In_Word:
		if (IsDelimiter(ch)) //ако е разделител
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

int GetWords(FILE *fp, FILE *fp_out)
{
	int c, countOfWords = 0;
	s_word status = None;
	char word[256];

	rewind(fp); //Връща файловия поинтър към началото на файла(за да може пак да се прочете)

	do
	{
		c = fgetc(fp); //взимаме 1 символ
		status = ComputeWord(c, word, status); //извиква функцията ComputeWord и записва върнатия резултат в статус

		if (status == End_Word && c != '\n') 
		{
			countOfWords++;
			printf("word => %s\n", word); //принтира на екрана
			fprintf(fp_out, "%s\n", word); //записва във файла
		}
	} while (c != EOF); //докато не стигне край на файла

	return countOfWords; //връща броя на думите във файла
}

int isAlpha(int c) //проверява дали символа е буква(аскии кода на символа)
{
	return
		(c >= 65 && c <= 90) || // c >= A && c <= Z
		(c >= 97 && c <= 122) || // проверява дали текущия символ е м/у а и z
		(c >= 192 && c <= 255); // проверява дали текущия символ е м/у а до я и от А до Я (кирилицата)

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
			if (isAlpha(c)) //проверява дали е буква
				state = 2;//преминава в следващото състояние
			break;
		case 2:
			switch (c) //проверява дали символа е пепинателен знак
			{
			case '.':
			case '!':
			case '?':
			case ':':
				countOfSenetences++;
				state = 1; //връща в начално състояние
				break;
			}
			break;
		}
	}
	return countOfSenetences; //връща броя на изреченията
}

void PrintSentences(FILE *fp_in, FILE *fp_out)
{
	int c;
	int mark = 0;
	int flag = 0;
	int letter = 0;

	rewind(fp_in);

	while ((c = fgetc(fp_in)) != EOF) //чете символ по символ докато не стигне EOF
	{
		if (isAlpha(c)) //ако прочетения символ е буква
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

void CopyString(char *newString, const char *oldStirng, int start, int end)//откъде докъде да копира стария стринг, в новия
{
	int i = start;
	while (i <= end)
	{
		newString[i - start] = oldStirng[i];
		i++;
	}
	newString[i] = '\0'; //'затваряме' стринга с терминираща нула
}

char** Split(char *line, int *strings)
{
	char **res = (char**)malloc(64 * sizeof(char*));//заделяме динамично памет за масива от указатели

	int delimiter = 0;
	int count = 0;

	size_t size = strlen(line); //взимаме дължината на прочетения ред
	for (size_t i = 0; i < size; i++) //минаваме през символите
	{
		if (IsDelim(line[i])) //проверяваме дали текущия символ е разделител
		{
			res[count] = (char*)calloc((64), sizeof(char));// заделяме памет за указателя(стринг)
			if (res[count] == NULL)
			{
				printf("Problem Allocating Memory !\n");//ако е неуспешно заделянето на памет
				return 1;
			}
			CopyString(res[count], line, delimiter, i - 1); //копира сегашния стринг в нов без да взима разделителя
			count++;

			while (IsDelim(line[i]))//увеличава брояча докато има последователни разделители
			{
				i++;
			}
			delimiter = i;
		}
		else if (i == size - 1) //докато не стигне края на стринга
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

	*strings = count; //връща броя на новите стрингове
	return res; //връща масив от указатели
}

void IntConstant(FILE *fp_in)
{
	char digit[256];
	char temp[256]; // записваме прочетения ред
	char **numbers;

	int state = 0;//състоянието на автомата
	int index = 0;
	int countOfDigits = 0;

	int flag = 0;

	rewind(fp_in);
	while (fgets(temp, sizeof(temp), fp_in)) //прочита 1 цял ред от подадения файл
	{
		size_t size; //броя на разделените стрингове
		numbers = Split(temp, &size);// разделяме реда на стрингове

		for (size_t row = 0; row < size; row++) 
		{
			flag = 0;
			size_t len = strlen(numbers[row]); //взима дължината на текущия стринг

			for (size_t i = 0; i <= len; i++)//цикъл, чрез който минаваме през символите
			{
				switch (state)
				{
				case 0:
					if (isdigit(numbers[row][i])) //проверяваме дали символа е цифра
					{
						digit[index++] = numbers[row][i]; //записваме го в масива
						state = 2; //сменяме състоянието
					}				
					break;

				case 1:
					if (numbers[row][i] == '+' || numbers[row][i] == '-') //дали започва с +/-
					{
						if (isdigit(numbers[row][i + 1])) //ако почва проверяваме дали другия символ е цифра
						{
							digit[index++] = numbers[row][i];
							state = 2; //сменяме състоянието на автомата
						}
					}
					break;

				case 2:
					if (isdigit(numbers[row][i]) || (numbers[row][i] == 'L' || numbers[row][i] == 'l'))// проверяваме за цифра или L/l (long)
					{
						digit[index++] = numbers[row][i]; //записваме в символа/числото в масива digit и увеличаваме индекса
					}
					else if (numbers[row][i] == '.')
					{
						flag = 1; //вдигаме флага ако намерим .
						digit[index++] = numbers[row][i];
					}
					else 
					{
						if (i == len && flag == 0) //проверяваме дали не е разделител
						{
							
							digit[index] = '\0';//детерминираме стринга
							printf("%s\n", digit);// принтираме на екрана числото

							countOfDigits++;
							digit[0] = '\0'; // нулираме масива с цифрите
							index = 0;
							state = 0; //връщаме в начално състояние
						}
						else
						{
							digit[0] = '\0'; //нулираме масива
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
			free(numbers[i]);//освобождаваме паметта на всеки указател
		}
		free(numbers);//освобождаваме паметта на масива от указатели
	}

	printf("Броя на целите константи е: %d\n", countOfDigits);
}

void RealConstant(FILE *fp_in)
{
	char digit[256];
	char temp[256]; // записваме прочетения ред
	char **numbers;
	int state = 0; //състояние на автомата
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
					if (isdigit(numbers[row][i])) //проверяваме дали символа е цифра
					{
						digit[index++] = numbers[row][i]; //записваме го в масива
						state = 1; //сменяме състоянието
					}
					else if (numbers[row][i] == '+' || numbers[row][i] == '-') //дали започва с +/-
					{
						if (isdigit(numbers[row][i + 1])) //ако почва проверяваме дали другия символ е цифра
						{
							digit[index++] = numbers[row][i];
							state = 1; //сменяме състоянието на автомата
						}
					}
					break;

				case 1:
					if (isdigit(numbers[row][i])) //проверяваме за цифра
					{
						digit[index++] = numbers[row][i];
					}
					else if (numbers[row][i] == '.') //проверка за десетична запетая
					{
						digit[index++] = numbers[row][i];
						state = 2; // преминаваме в следващото състоянието ако има
					}
					else if (numbers[row][i] == 'e' || numbers[row][i] == 'E')//проверяваме за e/E 
					{
						digit[index++] = numbers[row][i];
						state = 3; //ако е намерено отиваме в 3то състояние
					}
					else
					{
						digit[0] = '\0'; //в противен случай нулираме масива с цифрите
						index = 0;
						state = 0; //връщаме в начално състояние
					}
					break;

				case 2:
					if (isdigit(numbers[row][i]))
					{
						digit[index++] = numbers[row][i];
						flag = 1; //вдигаме флага ако намерим цифра
					}
					else if (numbers[row][i] == 'e' || numbers[row][i] == 'E') //проверяваме за e/E 
					{
						digit[index++] = numbers[row][i];
						state = 3; // ако е намерено отиваме в 3то състояние
					}
					else
					{
						if (i == len)//проверяваме дали не е разделител
						{
							if (flag == 1)
							{
								digit[index] = '\0';
								printf("%s\n", digit); // принтираме масива

								countOfNumbers++;
								digit[0] = '\0'; // нулираме масива с цифрите
								index = 0;
								state = 0; //връщаме в начално състояние
							}
							else
							{
								digit[0] = '\0'; //като по-горе
								index = 0;
								state = 0;
							}
						}
					}
					break;

				case 3:
					if (numbers[row][i] == '+' || numbers[row][i] == '-') //проверяваме за +/-
					{
						if (isdigit(numbers[row][i + 1])) //проверяваме дали символ е цифра
						{
							digit[index++] = numbers[row][i];
							state = 4; //отиваме в състояние 4
						}
					}
					else if (isdigit(numbers[row][i])) //дали е цифра 
					{
						digit[index++] = numbers[row][i];
						state = 4;
					}
					else //в притвен случай
					{
						state = 4;
						digit[0] = '\0'; //нулираме масива
						index = 0;
					}
					break;

				case 4:
					if (isdigit(numbers[row][i]))
					{
						digit[index++] = numbers[row][i];
						break;
					}
					else if (i == len)//проверяваме дали не е разделител
					{
						if (flag == 1)
						{
							digit[index] = '\0';
							printf("%s\n", digit); // принтираме масива

							countOfNumbers++;
							digit[0] = '\0'; // нулираме масива с цифрите
							index = 0;
							state = 0; //връщаме в начално състояние
						}
						else
						{
							digit[0] = '\0'; //като по-горе
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
			free(numbers[i]); //освобождаваме паметта на всеки указател
		}
		free(numbers);//освобождаваме паметта на масива от указатели
	}

	printf("Броя на реалните константи е: %d\n", countOfNumbers);
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
	int state = 0; // състоянието на автомата
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
			if (isalpha(ch)) // проверка дали започва с име на диск
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
				folder[index++] = ch; //чрез него ще броим колко символа е името на директорията
				if (index >= 8) //ако стане по дълго от 8
				{
					flag = 1;//вдигаме флага
					index = 0;//нулираме индекса и почваме да презаписваме
				}
				break;
			}
			else
			{
				if (ch == '\\') //ако има прочетена \влизаме в директорията
				{
					slashes++; // колко пъти се влиза в друга директория
					index = 0; // почваме да презаписваме масива folder 
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
		printf("Невалиден дисков файл.\n");
	}
	else
	{
		printf("Директорията е валидна.\n");
	}
}