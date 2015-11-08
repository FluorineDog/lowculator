//#pragma once
#include <math.h>
#include <string.h>
#include "gc.h"
#include <ctype.h>
#include <stdio.h>
#include "mkup.h"
#include "functionTable.h"
int base;
char* raw_str;
// #ifndef _MSC_VER
const char* symbols = "0123456789ABCDEF";
int get_digit(char ch) {
	int i;
	for (i = 0; i < 16; ++i) {
		if (symbols[i] == ch)
			break;
	}
	return i; // i == base for errors
}
// #else
// int get_digit(char ch) {
// 	int i;
// 	printf("HERE %c IS OK?\n",ch);
// 	switch (ch)
// 	{
// 		case '0'...'9':
// 			i = ch - '0';
// 		case 'A'...'F':
// 			i = ch - 'A' + 10;
// 		case 'a'...'f':
// 			i = ch - 'a' + 10;
// 		default:
// 			i = 16;
// 	}
// 	printf("THERE %d IS OK?\n",i);
// 	return i; // i == base for errors
// }
// #endif // MSC


T get_int(char* str, char** endsRef , int* indexRef) {
	T num = 0;
	int index = 0;
	int digit = get_digit(*str);
	if (digit >= base)
		error_dealing(str , "unexpected character: should be a valid digit");
	num = digit;
	++index;
	++str;
	while (true) {
		digit = get_digit(*str);
		if (digit >= base) {
			if (digit < 16)	//funny input
				error_dealing(str, "invalid digit");
			else
				break;  //sign of stop
		}
		++str;
		++index;
		num *= base;
		num += digit;
	}
	if (endsRef) {
		*endsRef = str;
	}
	if (indexRef) {
		*indexRef = index;
	}
	//printf("fuck %lf\n", num);
	return num;

}
T strToLD (char * str, char** endsRef){ //string to long double
	//sign
	bool isNega = false;
	switch (*str) { //deal with sign
	case '+':
		++str;
		break;
	case '-':
		++str;
		isNega = !isNega;
		break;
	}
	//base and related symbols
	if (*str == '0') {
		++str;
		switch (*str) {
			case 'b':++str;base = 2;break;
			case 'x':++str;base = 16;break;
			case 'o':++str;base = 8;break;
			default:	//may be natural base 8, or just 0.192 in base 10 
				if ('0' <= *str&&*str <= '9') {	//if 8,9 occurs, an error will set off later
					base = 8;
				}
				else {
					base = 10; //to deal with 0.123 fv
					--str;
				}
		}
	}
	else {
		base = 10;
	}
	T num = 0;
	if (*str != '.') {
		num += get_int(str, &str, nullptr);
		if (*str != '.')
			goto end;
	}
	++str;
	int index;
	T tmp = get_int(str, &str, &index);
	while (index-- > 0) {
		tmp /= base;  //no worry about overflow, since it called
	}
	num += tmp;
end:
	if(endsRef)
		*endsRef = str;
	return num;
}
typedef struct{
	void * func;
	int nVariable;
	int rank2push;
	int rank2calc;
}op_t;
#include "stack4opt.h"
#include "stack4num.h"
const op_t opTable[]={
	{(void *)&plus,		2,30,29},		//0
	{(void *)&minus,	2,30,29},		//1
	{(void *)&multiply,	2,20,19},		//2
	{(void *)&divide,	2,20,19},		//3
	{(void *)&mod,		2,20,19},		//4
	{(void *)&power,	2,10,11},		//5
	{(void *)&nothing,	-1,1,100},		//6		left  bracket
	{(void *)&nothing,	-2,0,100},		//7		End.
};
typedef T (*pf0)(void);
typedef T (*pf1)(T);
typedef T (*pf2)(T,T);

static stack4opt sop;	
static stack4num snum;


char* str4debug;
void opBraEnd() {
	for (op_t prev = peeko(&sop) ;;prev = peeko(&sop)) {
		popo(&sop);
		T x1, x2;
		switch (prev.nVariable) {
			case 0:
				pushn(&snum, ((pf0)(prev.func))());
				continue;
			case 1:
				x1 = peekn(&snum);
				popn(&snum);
				pushn(&snum, ((pf1)(prev.func))(x1));
				continue;
			case 2:
				x1 = peekn(&snum);
				popn(&snum);
				x2 = peekn(&snum);
				popn(&snum);
				pushn(&snum, ((pf2)(prev.func))(x2, x1));
				continue;
			case -1:
			case -2://i'm lazy
				return;
		}
	}
	
}
void opPush( op_t op) {
	for( op_t prev=peeko(&sop);	
			prev.rank2calc < op.rank2push;    //rank 0 is the highest
			prev=peeko(&sop) ){	
		popo(&sop);
		T x1,x2;	
		switch(prev.nVariable){
			case 0:
				pushn(&snum,((pf0)(prev.func))() );
				break;
			case 1:
				x1=peekn(&snum);
				popn(&snum);
				pushn(&snum,((pf1)(prev.func))(x1) );	
				break;
			case 2:
				x1=peekn(&snum);
				popn(&snum);
				x2=peekn(&snum);
				popn(&snum);
				pushn(&snum,((pf2)(prev.func))(x2,x1) );	
				break;
		}
	}
	pusho(&sop,op);
}
void midOpPush(int x){
	opPush(opTable[x]);
}
const int NUMFLG = 1;	//isPreviousNumber
const int OPFLG = 2;	//isPreviousOperator
T calculator(char* str){
	raw_str = str;
	inito(&sop);  
	initn(&snum);
	pusho(&sop,opTable[6]);
	int flag = OPFLG; //record the previous data type
	str4debug = raw_str; 
	while (true) {
		if (islower(*str)) {		//functions or constants
			funID* id = funMatch(str, &str);
			op_t ins;
			switch (id->nVariable)
			{
			case 0:
				pushn(&snum, ((pf0)(id->function))());
				flag = NUMFLG;
				continue;
			case 1:
				ins.func = id->function;
				ins.nVariable = 1;	// unary function
				ins.rank2push = 5;	// High rank
				ins.rank2calc = 6;	// right associative
				opPush(ins);
				flag = OPFLG;
				continue;
			default:
				error_dealing(str, "NO!!!!!!!!");
			}
		}
		str4debug = str;
		switch (*str) {
		case '\t':case ' ':case '\n':
		case '\"':case '\\':
			++str; continue;		// blank
		case '+':				
		case '-':
			if (flag == NUMFLG)		// sign? checking...
				goto OPDL;			/////////////
			//else no break: goto the following on purpose
		case '0':case '1':case '2':case '3':case '4':
		case '5':case '6':case '7':case '8':case '9':
		case '.':
			pushn(&snum, strToLD(str, &str));
			flag = NUMFLG;
			continue;
		case '(':midOpPush(6); ++str; flag = NUMFLG; continue;
		case ')':opBraEnd(); ++str;  continue;
		case '\0':
			opBraEnd();
			goto end;	// avoid ambiguous break
		}

OPDL:		flag = OPFLG;			/////////////
		switch (*str) {
			case '+':midOpPush(0); ++str; continue;
			case '-':midOpPush(1); ++str; continue;
			case '*':midOpPush(2); ++str; continue;
			case '/':midOpPush(3); ++str; continue;
			case '%':midOpPush(4); ++str; continue;
			case '^':midOpPush(5); ++str; continue;
		}
		error_dealing(str, "tell me what a fucking charactor you are!!");
		++str;
	}
end:					//
	if(sop.size==0&&snum.size==1){
			T result = peekn(&snum);
			free_all();
			return result;
	}
	else 
		error_dealing(str,"WRONG INPUT?BUG?GUESS WHAT.");
	return 0;
}










































