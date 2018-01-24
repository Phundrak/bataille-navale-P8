#include <camp_allocator.h>
#include <camp.h>
#include <game_state.h>
#include <stdlib.h>

typedef struct {
	camp_allocator_t base;
} single_camp_allocator_t;

static void single_allocate_camp(camp_allocator_t *self, game_state_t *game, player_t *p) {
	(void)self;
	// 1 camp par joueur
	// 1 joueur par camp
	camp_t *camp = new_camp();
	darray_push_back(camp->players, &p);
	darray_push_back(game->camps, &p);
	/* TODO: Demander au joueur de placer ses bateaux */
}

camp_allocator_t *new_single_allocator() {
	single_camp_allocator_t *ret = calloc(1, sizeof(*ret));
	ret->base.put_in_camp = single_allocate_camp;
	return &ret->base;
}
