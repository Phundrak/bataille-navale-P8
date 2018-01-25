#ifndef CAMP_H
#define CAMP_H

#include "player.h"
#include "darray.h"

typedef struct camp_s {
	darray_t *players;
	int is_alive;
} camp_t;

camp_t *newCamp();

#endif /* CAMP_H */
