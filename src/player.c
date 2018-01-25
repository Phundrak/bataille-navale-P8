#include <player.h>
#include <stdlib.h>

typedef struct {
	player_t base;
} local_player_t;

point_t playerLocalAction(player_t *self, game_state_t *game) {
	/* 
	   TODO: 
	   Afficher l'état du jeu
	   demander entrée coordonnées de tir
	*/
	return (point_t){0, 0};
}

player_t *newLocalPlayer() {
	local_player_t *ret = malloc(sizeof(*ret));
	ret->base.get_action = playerLocalAction;
	return &ret->base;
}
