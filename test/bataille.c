/**
 * \file bataille.c
 * \brief Implémentation des tests du jeu de bataille navale
 */

#include <test_framework.h>
#include <game_state.h>
#include <cell.h>
#include <player.h>
#include <stdlib.h>
#include <darray.h>
#include <camp.h>
#include <camp_allocator.h>

void blitToGrid(piece_t *piece, point_t pos, game_state_t *game, unsigned char id);

/// \brief Fait littéralement rien.
void nothing() {}

/**
 * Test si tout les bateaux sont coulables
 * return 1 si le test a passé, sinon 0
 */
TEST(sink_test) {
	game_state_t game_state = {0};

	int maxd = 0;
	for (int loop = 0; loop < NBBOATS; ++loop) {
		if (Pieces[loop].width > maxd)
			maxd = Pieces[loop].width;
		if (Pieces[loop].height > maxd)
			maxd = Pieces[loop].height;
	}

	game_state.height = game_state.width = (size_t) maxd;
	player_t admin = {.owned_rect = {{-1, -1}, {-1, -1}}, .setup_boats = nothing};
	player_t victime = (player_t) {.setup_boats = nothing};
	game_state.camps = darrayNew(sizeof(camp_t *));
	game_state.camp_allocator = newSingleAllocator();
	admin.name = "Administrateur";
	game_state.camp_allocator->put_in_camp(game_state.camp_allocator, &game_state, &victime);
	game_state.camp_allocator->put_in_camp(game_state.camp_allocator, &game_state, &admin);
	admin.owned_rect[0] = (point_t) {-1, -1};
	admin.owned_rect[1] = (point_t) {-1, -1};
	victime.owned_rect[0] = (point_t) {0, 0};
	victime.owned_rect[1] = (point_t) {(size_t) maxd, (size_t) maxd};
	game_state.grid = calloc(sizeof(cell_t), maxd * maxd);
	for (int outer_loop = 0; outer_loop < NBBOATS; ++outer_loop) {
		memset(game_state.grid, 0, sizeof(cell_t) * maxd * maxd);
		blitToGrid(&Pieces[outer_loop], (point_t){0, 0}, &game_state, 1);
		result_t result = MISS;
		for (size_t coords_x = 0; result != SUNK && coords_x < maxd; ++coords_x) {
			for (size_t coords_y = 0; result != SUNK && coords_y < maxd; ++coords_y) {
				result = doAction(&game_state, &admin, (point_t){coords_y, coords_x});
			}
		}
		if (result != SUNK)
			return 0;
	}
	return 1;
}

/**
 * Test si une partie finie bien.
 * return 1 si le test a passé, sinon 0
 */
TEST(end_game) {
	game_state_t *game = newGame((option_t) {15, 15});

	{
		player_t *player = newDumbPlayer();
		player->name = "Bot 1";
		game->camp_allocator->put_in_camp(game->camp_allocator, game, player);
		player = newDumbPlayer();
		player->name = "Bot 2";
		game->camp_allocator->put_in_camp(game->camp_allocator, game, player);
	}

	while (1) {
		for(unsigned nr_camps = 0; nr_camps < darraySize(game->camps); ++nr_camps) {
			camp_t *camp = *(camp_t **)darrayGet(game->camps, nr_camps);
			player_t *players = *(player_t**)darrayGet(camp->players, 0);
			size_t nr_players = darraySize(camp->players);
			while (nr_players--) {
				if (players->n_boats == 0) {
					++players;
					continue;
				}
				point_t coordinates;
				result_t result;
				do {
					coordinates = players->get_action(players, game);
					result = doAction(game, players, coordinates);
				} while(result == REDO);
				if (!turnEndUpdate(game))
					goto end;
				++players;
			}
		}
	}

end:;

	// Fin du jeu, nettoyage
	for (unsigned nr_camps = 0; nr_camps < darraySize(game->camps); ++nr_camps) {
		camp_t *camp = *(camp_t**)darrayGet(game->camps, nr_camps);
		deleteCamp(camp);
	}
	darrayDelete(game->camps);
	free(game->grid);
	free(game->camp_allocator);
	free(game);
	return 1;
}
