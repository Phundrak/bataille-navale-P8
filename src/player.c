#include <player.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
	player_t base;
} local_player_t;

static point_t playerLocalAction(player_t *self, game_state_t *game) {
	/* 
	   TODO: 
	   Afficher l'état du jeu
	   demander entrée coordonnées de tir
	*/
	return (point_t){0, 0};
}

static void playerLocalSetBoats(player_t *self, game_state_t *game) {
	local_player_t *p = (void*)self;
	
}

player_t *newLocalPlayer() {
	local_player_t *ret = malloc(sizeof(*ret));
	ret->base.get_action = playerLocalAction;
	ret->base.setup_boats = playerLocalSetBoats;
	return &ret->base;
}

enum color {BLUE, CYAN, RED, LIGHT_RED, WHITE, BLACK};

bool in_zone (int pos, game_state_t *user){
	int userzone_begin = game->width * user->owned_rect[0]->y + user->owned_rect[0]->x
	int userzone_end = game->width * user->owned_rect[1]->y + user->owned_rect[1]->x
	if (pos >= userzone_begin && pos <= userzone_end)
		return TRUE;
	return FALSE;
}

int * tab_color (player_t *user, game_state_t *game){ // return tableau des couleurs du jeu.
	int lng = game.width * game.height;
	int grid_color[lng];
	enum color_enmy = {BLUE, BLUE, CYAN, RED, NO, NO, NO, LIGHT_RED}
	enum color_ally = {CYAN, WHITE, CYAN, RED, NO, NO, NO, LIGHT_RED}
	for (int i=0 ; i<lng ; ++i){
		if (in_zone(i, user) == FALSE)	
			grid_color[i] = color_enmy[game->grid.state]
		else
			grid_color[i] = color_ally[game->grid.state]
	}
	return grid_color;
}




char pieces[][5][5] = {
	{
		{1, 1, 0, 0, 0},
		{1, 1, 0, 0, 0},
		{0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0}
	},
	{
		{1, 1, 1, 1, 0},
		{1, 1, 1, 1, 0},
		{0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0}
	},
	{
		{1, 1, 0, 0, 0},
		{0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0}
	},
	{
		{1, 1, 1, 0, 0},
		{0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0}
	},
	{
		{1, 1, 1, 1, 0},
		{0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0}
	},
	{
		{1, 1, 1, 1, 1},
		{0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0}
	},
	{
		{1, 1, 1, 1, 0},
		{0, 0, 0, 1, 0},
		{0, 0, 0, 1, 0},
		{0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0}
	}
};
