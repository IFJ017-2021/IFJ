/**
 *  @file str.h
 *
 *  @brief Header file for str.c (Header file that enables working with dynamic strings)
 *
 *  @author <xvasin11> Ladislav Vasina
 */


/**
 * @brief string structure
 */

typedef struct{
    char *data;    // Var to store the string
    int length;   // String lenght
    int capacity; // Memory allocated
} string;

int strInit(string *s);
void strFree(string *s);
void strClear(string *s);
int strAppendChar(string *s1, char toAppend);
int strCopyString(string *s1, string *s2);
int strCopyConstant(string *s1, char *c);
int strCompareString(string *s1, string *s2);
int strCompareConstant(string *s1, char *s2);
char *strGetString(string *s);
int strGetLength(string *s);
int strGetCapacity(string *s);
