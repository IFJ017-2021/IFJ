/**
 *  Project: Implementation of imperative language compiler IFJ21
 *  @file str.h
 *
 *  @brief Header file for str.c (Header file that enables working with dynamic strings)
 *
 *  @author <xvasin11> Ladislav Vasina
 */


#ifndef STRING_H
#define STRING_H


/**
 * @brief string structure
 */

typedef struct{
    char* data;    // Var to store the string
    int length;   // String length
    int capacity; // Memory allocated
} string;
/**
 * @brief string initialization
 */
int strInit(string *s);

/**
 * @brief clearing string from the memory
 */
void strFree(string *s);

/**
 * @brief deleting (clearing) whole string
 */
void strClear(string *s);

/**
 * @brief appends char at the end of the string 's1'
 */
int strAppendChar(string *s1, char toAppend);

/**
 * @brief appends string at the end of the string 's1'
 */
int strAppendStr(string *s1, const char toAppendStr[]);

/**
 * @brief copies string 's2' to the string 's1'
 */
int strCopyString(string *s1, string *s2);

/**
 * @brief copy constant 'c' to 's1'
 */
int strCopyConstant(string *s1, char *c);

/**
 * @brief converts unprintable chars as ascii values
 */
void asciiConvert(string *s);

/**
 * @brief compares string 's1' and 's2'
 */
int strCompareString(string *s1, string *s2);

/**
 * @brief compares string 's1' and constant 's2'
 */
int strCompareConstant(string *s1, char *s2);

/**
 * @brief returns value of string 's'
 */
char *strGetString(string *s);

/**
 * @brief returns length of string 's'
 */
int strGetLength(string *s);

/**
 * @brief returns data size (capacity) of string 's'
 */
int strGetCapacity(string *s);


#endif