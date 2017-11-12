//------------ file dict_func.h  -----------------------
#include<stdio.h>
#define SIZE_DICT 1000 //������� ������ �� ����������� ������
#define DELTA_SIZE 500 // ������ �� ����������� �� ����������� ������ 
#define NAMES 128

int IsDelimiter(char *delimiters, char c);
int IsDelim(int c);
// ---- �������� �������� �� ���� ---- 
void ComputeWord(int c, char *word, int *ind, int *status);
// --- (int c        - ����� ������;
// ---  int *br_word - ����� ���� �� ������
// ---  char *word   - �������� ����� �� ��������� �� ����;
// ---  int *status  - ����� ������ �� ���������� �����;
//----------------------------------------------------------------------------------
//---- ����������� ���������� ����� �� ����� �������� ���� � �������, ���� �����
//---- ����������� ������� �� ���������� �����, ���������� �������� �� ������
void DeleteDictionary(char **dict, int *count_words, int index_dict);

void PrintDictionary(char **dict, int index_dict, int *count_words, int status); //������� ������� �� ������

//����� ������ � ������� - ����� ������� � ��� ��������, ��� -1
int FindInToDictionary(char **dict, int index_dict, char *word);

// --- ������ ���� � �������, ���� ����� �������� �� ���������� ����� � ���������� ���� ---
char **AddToDictionary(char **dict, int *size_dict, int *index, int **count_words, char *word);
char **MakeDictionary(FILE *fp, char **dict, int *size_dict, int *index, int **count_words, int *status);

//int SaveDictionary(char **dict, int index, int *count_words, char *name_file, FILE *fp);
//int ReadDictionary(char ***dict, int *size_dict, int *index_dict, int **count_words, char *name_file, FILE *fp);

//��������� � �������� �� �������  
void CodingFile(char **dict, int index, char *inputFName);
void DecodingFile(char **dict, int index, char *inputFName);

int my_fgetc(FILE *fp); //�������� ������ �� ������ �� ������������ �� EOF 

void ToUpper(unsigned char *line);
void ToLower(unsigned char *line);

FILE *getNameFile(char *name_file, char *mode, char *text);

void SearchInDictionary(char **dict, int index_dict);

void SaveDictInFile(char **dict, int *values, int size, char *inputFName, int mode);
void SaveDictInCNTFile(char **dict, int *values, int size, char *inputFName, int mode);

void SortByAlphabet(char **dict, int size, int *countOfWords);//��������� �� ������� �� ������� ���
void SortByFrequency(char **dict, int *arr, int size);//��������� �� ������� �� �������
void SwapPointers(const char **firstString, const char **secondString);

typedef enum sta_word
{
	None, In_Word, End_Word
}s_word;

s_word CompWord(int c, char *word, s_word status);
int IsNumber(int c, int state);
void BuildNum(int c, int *number, int *flag);
