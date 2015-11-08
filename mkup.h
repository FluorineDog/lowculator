#ifndef MKUP_H_
#define MKUP_H_
#include <stdio.h>
#include <stdlib.h>
#if !defined __cplusplus
#define nullptr 0
typedef int bool;
const bool false = 0;
const bool true = 1;
#endif // !defined __cplusplus 

typedef long double T;
void free_all(void);
extern char* raw_str;
void error_dealing(char* str, char* message)
{
	int loc = str - raw_str;
	printf("\n%s\n", raw_str);
	for (int i = 0; i < loc; i++)
		putchar(' ');
	putchar('^');
	printf("\n%s\n", message);
    free_all();
    //	PAUSE;
	exit(-1);
	//something to do///////////////////////////////////////////////
}
#endif // !MKUP_H_
