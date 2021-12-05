/**
 *  Project: Implementation of imperative language compiler IFJ21
 *  @file str.c
 *
 *  @brief Library that enables working with dynamic strings
 *
 *  @author <xvasin11> Ladislav Vasina
 */

#include "str.h"
#include <string.h>
#include <malloc.h>

#define STR_OK 1
#define STR_ERR 0

#define STR_LEN_INC 8

/**
 * @brief string initilization
 */
int strInit(string *s){
    s->data = (char*) malloc(STR_LEN_INC); // String allocation 
    if(s->data != NULL){ // String allocation OK
        s->data[0] = '\0';
        s->length = 0;
        s->capacity = STR_LEN_INC;
        return STR_OK;
    }else{ // String allocation FAILED
        return STR_ERR;
    }
}

/**
 * @brief clearing string from the memory
 */
void strFree(string *s){
    free(s->data);
}

/**
 * @brief deleting (clearing) whole string
 */
void strClear(string *s) {
  s->data[0] = '\0';
  s->length = 0;
}

/**
 * @brief appends char at the end of the string 's1'
 */
int strAppendChar(string *s1, char toAppend){
    if((s1->length + 1) < s1->capacity){ // If there is enough of the allocated space
        s1->data[s1->length] = toAppend;
        s1->length++;
        s1->data[s1->length] = '\0';
        return STR_OK;
    }else{ // There is not enough of the allocated memory --> increase
        s1->data = (char *) realloc(s1->data, sizeof(char) * (s1->length + STR_LEN_INC));
        if(s1->data == NULL){
            return STR_ERR; // Allocation failed
        }
        s1->capacity = s1->capacity + STR_LEN_INC; 
        s1->data[s1->length] = toAppend;
        s1->length++;
        s1->data[s1->length] = '\0';
        return STR_OK;
    }
}
/**
 * @brief appends string at the end of the string 's1'
 */
int strAppendStr(string *s1, const char *toAppendStr){
    int const_string_length = (int) strlen(toAppendStr);

    if(s1->length + const_string_length + 1 >= s1->capacity){ // If we can't fit needed string to the string that is already made
        int new_capacity = s1->length + const_string_length + 1;
        if(s1->data != (char *) realloc(s1->data, sizeof(char) * new_capacity)){ // Realloc not successful
            return STR_ERR;
        }
        s1->capacity = new_capacity; // Allocate new capacity
    }
    s1->length = s1->length + const_string_length; // Increase string length
    strcat(s1->data, toAppendStr); // Append string
    s1->data[s1->length] = '\0';
    return STR_OK;
}

/**
 * @brief copies string 's2' to the string 's1'
 */
int strCopyString(string *s1, string *s2){
    if(s2->length >= s1->capacity){ // If there is NOT enough memory allocated
        s1->data = (char *) realloc(s1->data, s2->length + 1); // Alloce enough space for s2
            if(s1->data == NULL){
                return STR_ERR; // Allocation failed
            }
        s1->capacity = s2->length + 1; // Change capacity parametr of 's1'(destination string where we copy second string)
    }
    strcpy(s1->data, s2->data); // Copy 's2' to 's1'
    s1->length = s2->length;
    return STR_OK;
}

/**
 * @brief copy constant 'c' to 's1'
 */
int strCopyConstant(string *s1, char *c){
    int newLength = strlen(c);
    if(newLength >= s1->capacity){ // If there is NOT enough memory allocated
        s1->data = (char *) realloc(s1->data, newLength + 1); // Alloce enough space for 's2'
            if(s1->data == NULL){
                return STR_ERR; // Allocation failed
            }
        s1->capacity = newLength + 1; // Change capacity parametr of 's1'(destination string where we copy second string)
    }
    strcpy(s1->data, c); // Copy 'c' to 's1'
    s1->length = newLength;
    return STR_OK;
}

/**
 * @brief compares string 's1' and 's2'
 */
int strCompareString(string *s1, string *s2){
    return strcmp(s1->data, s2->data);
}

/**
 * @brief compares string 's1' and constant 's2'
 */
int strCompareConstant(string *s1, char *s2){
    return strcmp(s1->data, s2);
}

/**
 * @brief returns value of string 's'
 */
char *strGetString(string *s){
    return s->data;
}

/**
 * @brief returns lenght of string 's'
 */
int strGetLength(string *s){
    return s->length;
}

/**
 * @brief returns data size (capacity) of string 's'
 */
int strGetCapacity(string *s){
    return s->capacity;
}
