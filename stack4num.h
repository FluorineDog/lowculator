#ifndef STACK4NUM_H_
#define STACK4NUM_H_
#include "mkup.h"
#include "gc.h"
typedef T mytype1;
typedef struct {
	mytype1* data;
	int size;
	int capacity;
}stack4num;
void initn(stack4num* s) {// CHAR n for number
	s->data = nullptr;
	s->size = 0;
	s->capacity = 0;
}
void pushn(stack4num* s, mytype1 x) {
	if (s->size >= s->capacity) {
		s->capacity += 20 + s->capacity / 2;
		mytype1* old = s->data;
		s->data = (mytype1*)malloc_smart(sizeof(mytype1) * s->capacity);
		for (int i = 0; i < s->size; i++){
			s->data[i] = old[i];
		}
		free_smart(s->data);
	}
	++s->size;
	s->data[s->size-1]=x;
}
extern char* str4debug;
void error_dealing(char* str, char* message);
void popn(stack4num* s) {
	if (!s->size)
		error_dealing(str4debug,"syntax error, believe it or not");
	-- s->size;

}
mytype1 peekn(stack4num* s) {
	if (!s->size)
		error_dealing(str4debug,"syntax error, believe it or not");
	return s->data[s->size - 1];
}
#endif // STACK4NUM_H_