#include <stdio.h>
#include <string.h>
#include <darray.h>

#define PTR_ERROR 2

darray_t *darrayNew(size_t element_size) {
  darray_t *ret;
  ret = (darray_t*)malloc(sizeof(darray_t));
  ret->begin = calloc(1, element_size);
  ret->end = ret->begin + 1;
  ret->element_size = element_size;
  ret->nb_elem = 0;
  return ret;
}

void darrayInsert(darray_t *self, void *pos, void *elem) {
  void *itr;

  if(!realloc(self, sizeof(self->element_size) * (self->nb_elem + 1))) {
    printf("Failed memory reallocation at %s:%d\nAborting...", __FILE__, __LINE__ - 1);
    exit(PTR_ERROR);
  }
  for(itr = self->end; itr != pos; --itr)
    memcpy(itr, itr - 1, self->element_size);
  memcpy(elem, itr, self->element_size);
}

void  darrayErase(darray_t *self, void *pos) {
  void* itr;
  free(pos);
  for(itr = pos; itr != self->end - 1; ++itr)
    memcpy(itr + 1, itr, self->element_size);
  if(!realloc(self, sizeof(self->element_size) * (self->nb_elem - 1))) {
    printf("Failed memory reallocation at %s:%d\nAborting...", __FILE__, __LINE__ - 1);
    exit(PTR_ERROR);
  }
}

size_t darraySize(darray_t *self) {
  return self->element_size;
}

size_t darrayNbElem(darray_t *self) {
  return self->nb_elem;
}

void *darrayGet(darray_t *self, size_t idx) {
  void *itr;
  int i;
  for(itr = self->begin, i = 0; i < idx; ++i, ++itr)
    ;
  return itr;
}

void darrayPushBack(darray_t *self, void* elem) {
  if(!realloc(self, sizeof(self->element_size) * (self->nb_elem + 1))) {
    printf("Failed memory reallocation at %s:%d\nAborting...", __FILE__, __LINE__ - 1);
    exit(PTR_ERROR);
  }
  ++self->nb_elem;
  self->end = elem;
  ++self->end;
}

void darrayPopBack(darray_t *self) {
  free(self->end - 1);
  if(!realloc(self, sizeof(self->element_size) * (self->nb_elem - 1))) {
    printf("Failed memory reallocation at %s:%d\nAborting...", __FILE__, __LINE__ - 1);
    exit(PTR_ERROR);
  }
  --self->end;
}

void darrayDelete(darray_t *self) {
  for(; self->begin != self->end; ++self)
    free(self->begin);
}