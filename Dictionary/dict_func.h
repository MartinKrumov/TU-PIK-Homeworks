//------------ file dict_func.h  -----------------------
#include<stdio.h>
#define SIZE_DICT 1000 //начален размер на динамичните масиви
#define DELTA_SIZE 500 // стъпка на увеличаване на динамичните масиви 
#define NAMES 128

int IsDelimiter(char *delimiters, char c);
int IsDelim(int c);
// ---- потоково отделяне на дума ---- 
void ComputeWord(int c, char *word, int *ind, int *status);
// --- (int c        - текущ символ;
// ---  int *br_word - текущ брой на думите
// ---  char *word   - символен масив за отделянер на дума;
// ---  int *status  - текущ статус на автоматния модел;
//----------------------------------------------------------------------------------
//---- освобождава заделената памет за всяка отделена дума в речника, след което
//---- освобождава паметта за динамичния масив, съхраняващ адресите на думите
void DeleteDictionary(char **dict, int *count_words, int index_dict);

void PrintDictionary(char **dict, int index_dict, int *count_words, int status); //извежда речника на екрана

//търси думата в речника - връща индекса й при намиране, или -1
int FindInToDictionary(char **dict, int index_dict, char *word);

// --- добавя дума в речника, като връща началото на динамичния масив с добавената дума ---
char **AddToDictionary(char **dict, int *size_dict, int *index, int **count_words, char *word);
char **MakeDictionary(FILE *fp, char **dict, int *size_dict, int *index, int **count_words, int *status);

//int SaveDictionary(char **dict, int index, int *count_words, char *name_file, FILE *fp);
//int ReadDictionary(char ***dict, int *size_dict, int *index_dict, int **count_words, char *name_file, FILE *fp);

//записване и кодиране на речника  
void CodingFile(char **dict, int index, char *inputFName);
void DecodingFile(char **dict, int index, char *inputFName);

int my_fgetc(FILE *fp); //директно четене на символ от клавиатурата до EOF 

void ToUpper(unsigned char *line);
void ToLower(unsigned char *line);

FILE *getNameFile(char *name_file, char *mode, char *text);

void SearchInDictionary(char **dict, int index_dict);

void SaveDictInFile(char **dict, int *values, int size, char *inputFName, int mode);
void SaveDictInCNTFile(char **dict, int *values, int size, char *inputFName, int mode);

void SortByAlphabet(char **dict, int size, int *countOfWords);//сортиране на речника по азбучен ред
void SortByFrequency(char **dict, int *arr, int size);//сортиране на речника по честота
void SwapPointers(const char **firstString, const char **secondString);

typedef enum sta_word
{
	None, In_Word, End_Word
}s_word;

s_word CompWord(int c, char *word, s_word status);
int IsNumber(int c, int state);
void BuildNum(int c, int *number, int *flag);
