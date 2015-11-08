#ifndef FUNCTIONTABLE_H_
#define FUNCTIONTABLE_H_
#include "mkup.h"
#include <math.h>
#include <ctype.h>
#include <string.h>
typedef struct {
	char* name;
	int nVariable;
	void * function;
}funID;

typedef T unaryFun(T);
typedef T binaryFun(T, T);



const T pi()  {return 3.14159265358979323846264338327974944L;}
const T e_natural()  {return 2.718281828459045235360287471352662497757247L;}
funID functions[] =
{
	{ "sinh",1, (void *)sinhl },  
	{ "sh"	,1, (void *)sinhl },
	{ "cosh",1, (void *)coshl },
	{ "ch"	,1, (void *)coshl },
	{ "tanh",1, (void *)tanhl },
	{ "th"	,1, (void *)tanhl },
	//{ "mod"	,2, (void *)fun_mod }, to be lazy...
	{ "sin"	,1, (void *)sinl },
	{ "cos"	,1, (void *)cosl },
	{ "tan"	,1, (void *)tanl },
	{ "pi"	,0, (void *)&pi },	//regard constants as 0-variable functions for simplification.
	{ "e"	,0,	(void *)&e_natural },
};

funID* funMatch(char* str, char** end) {
	char funbuf[10] = {'\0'};
	int i = 0;
	do {
		funbuf[i] = *str;
	}
	while (++i < 9 && isalpha(*++str));
	funbuf[i] = 0;
	for (int index = 0; index < sizeof(functions) / sizeof(funID); ++index) {
		if (!strcmp(functions[index].name, funbuf)) {
			return &functions[index];
		}
	}
	error_dealing(str, "function or constant unmatched");
	return nullptr;
};

T plus(T a,T b){
    return a+b;
}
T minus(T a,T b){
    return a-b;
}
T multiply(T a,T b){
    return a*b;
}
T divide(T a,T b){
    return a/b;
}
T mod(T a, T b){
	return a - (int)(a/b)*b;
}
T power(T a,T b){
	return powl(a,b);
}
T nothing(T a){
	return a;
}
#endif