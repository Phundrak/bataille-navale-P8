#include <player.h>
#include <game_state.h>

extern void blitToGrid(piece_t *p, point_t pos, game_state_t *game, unsigned char id);

typedef struct {
	player_t base; /*!< Instance parente */
	point_t last;
} dumb_player_t;

point_t dumbPlayerAction(player_t *p, game_state_t *gs) {
	dumb_player_t *self = (void*)p;
	++self->last.x;
	if (self->last.x >= gs->width) {
		self->last.x -= gs->width;
		++self->last.y;
	}
	return self->last;
}

point_t localToGlobal(player_t *self, point_t p) {
	return (point_t) {
		p.x + self->owned_rect[0].x,
		p.y + self->owned_rect[0].y
	};
}

void dumbplayerSetBoats(player_t *self, game_state_t *gs) {
	int all = 1;
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j) {
			point_t pos = localToGlobal(self, (point_t){i * 5, j * 5});
			blitToGrid(&Pieces[all - 1], pos, gs, all);
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
