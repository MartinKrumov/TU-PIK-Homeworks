#include <stdio.h>

FILE *getNameFile(char *name_file, char *mode, char *text);
int IsDelimiter(int c);

typedef enum sta_word
{
	None, In_Word, End_Word 
}s_word;

s_word ComputeWord(int c, char *word, s_word status);
int GetWords(FILE *fp, FILE *fp_out);
//-----------------------------------------------------

int isAlpha(int c);
int CountOfSentences(FILE *fp_in);
void PrintSentences(FILE *fp_in, FILE *fp_out);

//--------------------------------
int IsDelim(int c);
void CopyString(char *newString, const char *oldStirng, int start, int end);
char** Split(char *line, int *strings);

void IntConstant(FILE *fp_in);
void RealConstant(FILE *fp_in);
//---------------------------------
int ZabraneniSimvoli(char symbol);
void ValidDirectory(char *path);