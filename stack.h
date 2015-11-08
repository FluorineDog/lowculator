#ifndef STACK_H_
#define STACK_H_
#include "mkup.h"
#include "gc.h"
typedef int mytype;
typedef struct {
	mytype* data;
	int size;
	int capacity;
}stack;
void init(stack* s) {
	s->data = nullptr;
	s->size = 0;
	s->capacity = 0;
}
void push(stack* s, mytype x) {
	if (s->size >= s->capacity) {
		s->capacity += 20 + s->capacity / 2;
		mytype* old = s->data;
		s->data = (mytype*)malloc_smart(sizeof(mytype) * s->capacity);
		for (int i = 0; i < s->size; i++){
			s->data[i] = old[i];
		}
		free_smart(s->data);
	}
	++s->size;
	s->data[s->size-1]=x;
}
extern char* str4debug;
void pop(stack* s) {
	if (!s->size)
		error_dealing(str4debug,"syntax error, believe it or not");
	-- s->size;

}
mytype peek(stack* s) {
	if (!s->size)
		error_dealing(str4debug,"syntax error, believe it or not");
	return s->data[s->size - 1];
}
#endif // STACK_H_