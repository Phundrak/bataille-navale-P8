#include <player.h>
#include <stdlib.h>

typedef struct {
	player_t base;
} local_player_t;

point_t player_local_action(player_t *self, game_state_t *game) {
	/* 
	   TODO: 
	   Afficher l'état du jeu
	   demander entrée coordonnées de tir
	*/
	return (point_t){0, 0};
}

player_t *new_local_player() {
	local_player_t *ret = malloc(sizeof(*ret));
	ret->base.get_action = player_local_action;
	return &ret->base;
}
