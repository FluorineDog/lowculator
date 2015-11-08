#ifndef STACK4OPT_H_
#define STACK4OPT_H_
#include "mkup.h"
#include "gc.h"
typedef op_t mytype2;
typedef struct {
	mytype2* data;
	int size;
	int capacity;
}stack4opt;
void inito(stack4opt* s) {
	s->data = nullptr;
	s->size = 0;
	s->capacity = 0;
}
void pusho(stack4opt* s, mytype2 x) {
	if (s->size >= s->capacity) {
		s->capacity += 20 + s->capacity / 2;
		mytype2* old = s->data;
		s->data = (mytype2*)malloc_smart(sizeof(mytype2) * s->capacity);
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
void popo(stack4opt* s) {
	if (!s->size)
		error_dealing(str4debug,"syntax error, believe it or not");
	-- s->size;

}
mytype2 peeko(stack4opt* s) {
	if (!s->size)
		error_dealing(str4debug,"syntax error, believe it or not");
	return s->data[s->size - 1];
}
#endif // STACK4OPT_H_