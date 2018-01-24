#ifndef DARRAY_H
#define DARRAY_H

#include <stdlib.h>

typedef struct {
	void *begin;
	void *end;
	size_t element_size;
} darray_t;

darray_t *darray_new(size_t element_size);

void  darray_insert(darray_t *self, void *pos, void *elem);
void  darray_erase(darray_t *self, void *pos);
void *darray_get(darray_t *self, size_t idx);
size_t darray_size(darray_t *self);
void  darray_push_back(darray_t *self, void *elem);
void  darray_pop_back(darray_t *self);
void  darray_delete(darray_t *self);

#endif /* DARRAY_H */