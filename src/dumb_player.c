#include <player.h>
#include <game_state.h>

extern void blitToGrid(piece_t *p, point_t pos, game_state_t *game, unsigned char id);

typedef struct {
	player_t base; /*!< Instance parente */
	point_t last;
} dumb_player_t;

point_t dumbPlayerAction(player_t *player, game_state_t *game_state) {
	dumb_player_t *self = (void*)player;
	++self->last.x;
	if (self->last.x >= game_state->width) {
		self->last.x -= game_state->width;
		++self->last.y;
	}
	return self->last;
}

point_t localToGlobal(player_t *self, point_t point) {
	return (point_t) {
		point.x + self->owned_rect[0].x,
		point.y + self->owned_rect[0].y
	};
}

void dumbplayerSetBoats(player_t *self, game_state_t *gs) {
	int all = 1;
	for (int outer_loop = 0; outer_loop < 3; ++outer_loop)
		for (int inner_loop = 0; inner_loop < 3; ++inner_loop) {
			point_t pos = localToGlobal(self, (point_t){(size_t) (outer_loop * 5), (size_t) (inner_loop * 5)});
			blitToGrid(&Pieces[all - 1], pos, gs, (unsigned char) all);
			all++;
			if (all > NBBOATS) {
				self->n_boats = all - 1;
				return;
			}
		}
}

player_t *newDumbPlayer() {
	dumb_player_t *ret = calloc(1, sizeof(*ret));
	ret->last = (point_t) {-1, 0};
	ret->base.get_action = dumbPlayerAction;
	ret->base.setup_boats = dumbplayerSetBoats;
	return &ret->base;
}
