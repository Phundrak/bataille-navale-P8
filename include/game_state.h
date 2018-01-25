#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "cell.h"
#include "result.h"
#include "camp.h"
#include "darray.h"
#include "camp_allocator.h"

typedef struct game_state_s {
	cell_t *grid;
	int width;
	int height;

	camp_allocator_t *camp_allocator;
	darray_t *camps;
} game_state_t;

game_state_t *newGame();
result_t doAction(game_state_t *, player_t *, point_t);
int turnEndUpdate(game_state_t *);
#endif /* GAME_STATE_H */