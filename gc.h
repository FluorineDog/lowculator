#ifndef GC_H_
#define GC_H_
#include <stdlib.h>
#include "mkup.h"
int x = sizeof(T);
struct {
	void ** ptrList;
	int count;
	int capacity;
}mem; 

void initialize() {
	mem.ptrList = nullptr;
	mem.count = 0;
	mem.capacity = 0;
}

void mem_stack_add(void* newPtr) {
	if (mem.count >= mem.capacity) {
		mem.capacity += 20 + mem.capacity / 2;
		void ** old = mem.ptrList;
		mem.ptrList = (void **)malloc(sizeof(void*) * mem.capacity);
		for (int i = 0; i < mem.count; i++)
		{
			mem.ptrList[i] = old[i];
		}
		free(old);
	}
	++mem.count;
	mem.ptrList[mem.count - 1] = newPtr;
}


void* malloc_smart(int size) {
	void *tmp = malloc(size);
	mem_stack_add(tmp);
	return tmp;
}
void free_smart(void* p) {
	// do nothing for efficience
	// for (int i = 0; i < mem.count; i++)
	// {
	// 	if (mem.ptrList[i] == p) {
	// 		free(p);
	// 		mem.ptrList[i] = nullptr;
	// 		break
	// 	}
	// }
}
void free_all() {
	for (int i = 0; i < mem.count; i++)
	{
		free(mem.ptrList[i]);
	}
	free(mem.ptrList);
	mem.ptrList = nullptr;
	mem.count = 0;
	mem.capacity = 0;
}
#endif // !GC_H_
