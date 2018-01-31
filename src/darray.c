#include <darray.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>

#define _GNU_SOURCE
#define PTR_ERROR 2

darray_t *darrayNew(size_t element_size) {
  darray_t *ret;
  ret = (darray_t *)malloc(sizeof(darray_t));
  ret->begin = calloc(1, element_size);
  ret->end = ret->begin;
  ret->element_size = element_size;
  ret->nb_elem = 0;
  return ret;
}

void darrayInsert(darray_t *self, void *pos, void *elem) {
  void *itr, *new_array;

  new_array = reallocarray(self->begin, self->nb_elem + 1, self->element_size);
  if (!new_array) {
    printf("Failed memory reallocation at %s:%d\nAborting...", __FILE__,
           __LINE__ - 2);
    exit(PTR_ERROR);
  }
  self->begin = new_array;
  for (itr = self->end; itr != pos; itr = (char *)itr - self->element_size)
    memcpy(itr, (char *)itr - self->element_size, self->element_size);
  memcpy(elem, itr, self->element_size);
}

void darrayErase(darray_t *self, void *pos) {
  void *itr;
  for (itr = pos; itr < self->end; itr = (char *)+self->element_size)
    memcpy((char *)itr + self->element_size, itr, self->element_size);
  self->end = (char *)self->end - self->element_size;
}

size_t darraySize(darray_t *self) { return self->element_size; }

size_t darrayNbElem(darray_t *self) { return self->nb_elem; }

void *darrayGet(darray_t *self, size_t idx) {
  void *itr;
  size_t i;
  for (itr = self->begin, i = 0; i < idx;
       ++i, itr = (char *)itr + self->element_size)
    ;
  return itr;
  //  return &self->begin[idx];
}

void darrayPushBack(darray_t *self, void *elem) {
  void *new_array;
  new_array = reallocarray(self->begin, self->nb_elem + 1, self->element_size);
  if (!new_array) {
    printf("Failed memory reallocation at %s:%d\nAborting...", __FILE__,
           __LINE__ - 2);
    exit(PTR_ERROR);
  }
  self->begin = new_array;
  ++self->nb_elem;
  memcpy(self->end, elem, self->element_size);
  //  self->end = elem;
  self->end = (char *)self->end + self->element_size;
}

void darrayPopBack(darray_t *self) {
  void *new_array;
  free((char *)self->end - self->element_size);
  new_array = reallocarray(self->begin, self->nb_elem - 1, self->element_size);
  if (!new_array) {
    printf("Failed memory reallocation at %s:%d\nAborting...", __FILE__,
           __LINE__ - 2);
    exit(PTR_ERROR);
  }
  self->begin = new_array;
  self->end = (char *)self->end - self->element_size;
}

void darrayDelete(darray_t *self) { free(self->begin); }
