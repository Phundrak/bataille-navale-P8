#include <game_state.h>
#include <stdlib.h>

/* TODO: 
   Ajouter la possibilité de passer des options, notamment
   nombre de joueurs
   nombre d'équipes
   type de partie
 */
game_state_t *new_game(/*Options*/) {
	game_state_t *ret = calloc(sizeof(*ret), 1);

	ret->width = 17 * 2;
	ret->height = 17;
	ret->grid = calloc(sizeof(cell_t), ret->width * ret->height);
	ret->camps = darray_new(sizeof(camp_t*));

	// Chacun pour soi
	ret->camp_allocator = new_single_allocator();
	return ret;
}
