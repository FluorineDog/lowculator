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




//Number fetcher

#ifdef _MSC_VER
const char* symbols = "0123456789ABCDEF";
int get_digit(char ch) {
	int i;
	for (i = 0; i < 16; ++i) {
		if (symbols[i] == ch)
			break;
	}
	//printf("%dFUCKYOU",i);////////////////////////////////////// 
	return i; // i == base for errors
}
#else
int get_digit(char ch) {
	int i;
	switch (ch)
	{
	case '0'...'9':
		i = ch - '0';
	case 'A'...'F':
		i = ch - 'A' + 10;
	default:
		i = 16;
	}

	return i; // i == base for errors
}
#endif // MSC


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
			if (digit < 16)
				error_dealing(str, "invalid digit");
			break;
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
T strToLD (char * str, char** endsRef){
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
		case 'b':
			++str;
			base = 2;
			break;
		case 'x':
			++str;
			base = 16;
			break;
		case 'o':
			++str;
			base = 8;
			break;
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
		tmp /= base;
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
	int rank2stay;
}op_t;
#include "stack4opt.h"
#include "stack4num.h"
const op_t opTable[]={
	{(void *)&plus,2,30,29},
	{(void *)&minus,2,30,29},
	{(void *)&multiply,2,20,19},
	{(void *)&divide,2,20,19},
	{(void *)&mod,2,20,19},
	{(void *)&power,2,10,11},
	{(void *)&nothing,1,0,100}, //left  bracket
	{(void *)&nothing,1,99,100}, //right  bracket
};
typedef T (*pf0)(void);
typedef T (*pf1)(T);
typedef T (*pf2)(T,T);

static stack4opt sop;
static stack4num snum;


char* str4debug;
void opPush(const op_t* ptr) {
	for(const op_t* pold=peeko(&sop);	
			pold->rank2stay < ptr->rank2push;
			pold=peeko(&sop) ){	
		popo(&sop);
		T x1,x2;
		switch(pold->nVariable){
			case 0:
				pushn(&snum,((pf0)(pold->func))() );
			case 1:
				x1=peekn(&snum);
				popn(&snum);
				pushn(&snum,((pf1)(pold->func))(x1) );					
			case 2:
				x1=peekn(&snum);
				popn(&snum);
				x2=peekn(&snum);
				popn(&snum);
				pushn(&snum,((pf2)(pold->func))(x1,x2) );				
		}
	}
	pusho(&sop,*ptr);
}
void midOpPush(int x){
	const op_t* ptr = &(opTable[x]);
	opPush(ptr);
}
const int NUMFLG = 1;
const int OPFLG = 2;
//const int OPFLG = 2;
T calculator(char* str){
	raw_str = str;
	inito(&sop);
	initn(&snum);
	pusho(&sop,opTable[6]);
	int flag = OPFLG; //record the previous data type
	while(true){
		str4debug=str;

		switch(*str){
			case '\t':
			case ' ':
			case '\"':
			case '\\':
				++str;
				continue;
			case '+':
			case '-':
				if(flag == OPFLG)
					break;
				//do as the following intensively
			case '0'...'9':
			case '.':
				pushn(&snum,strToLD(str, &str));
				flag = NUMFLG;
				continue;
			case '(':midOpPush(6);++str;flag = NUMFLG;continue;
			case ')':midOpPush(6);++str;flag = NUMFLG;continue;
			case '\0':midOpPush(6);goto end;
		}
		
		flag = OPFLG;
		switch(*str){
			case '+':midOpPush(0); ++str;continue;
			case '-':midOpPush(1); ++str;continue;
			case '*':midOpPush(2); ++str;continue;
			case '/':midOpPush(3); ++str;continue;
			case '%':midOpPush(4); ++str;continue;
			case '^':midOpPush(5); ++str;continue;
		}
		
		++str;
	}
end:
	if(sop.size==1&&snum.size==1)
	{
			T result = peekn(&snum);
			free_all();
			return result;
	}
	else 
	error_dealing(str,"BUG? WRONG INPUT? GUESS WHAT.");
	return 0;
}













































