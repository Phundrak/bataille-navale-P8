#include <test_framework.h>
#include <game_state.h>
#include <cell.h>
#include <player.h>
#include <stdlib.h>
#include <darray.h>
#include <camp.h>
#include <camp_allocator.h>

extern char Pieces[][PIECE_SIZE][PIECE_SIZE];
void blitToGrid(char (*piece)[5][5], point_t pos, game_state_t *game, unsigned char id);

void nothing() {}

TEST(sink_test) {
	game_state_t gs = {0};
	gs.height = gs.width = PIECE_SIZE;
	player_t admin = {.owned_rect = {{-1, -1}, {-1, -1}}, .setup_boats = nothing};
	player_t victime = {.setup_boats = nothing};
	gs.camps = darrayNew(sizeof(camp_t *));
	gs.camp_allocator = newSingleAllocator();
	admin.name = "Administrateur";
	gs.camp_allocator->put_in_camp(gs.camp_allocator, &gs, &victime);
	gs.camp_allocator->put_in_camp(gs.camp_allocator, &gs, &admin);
	admin.owned_rect[0] = (point_t) {-1, -1};
	admin.owned_rect[0] = (point_t) {-1, -1};
	gs.grid = calloc(sizeof(cell_t), PIECE_SIZE * PIECE_SIZE);
	for (int i = 0; i < NBBOATS; ++i) {
		memset(gs.grid, 0, sizeof(cell_t) * PIECE_SIZE * PIECE_SIZE);
		blitToGrid(&Pieces[i], (point_t){0, 0}, &gs, 1);
		result_t r;
		for (int y = 0; r != SUNK && y < PIECE_SIZE; ++y) {
			for (int x = 0; r != SUNK && x < PIECE_SIZE; ++x) {
				r = doAction(&gs, &admin, (point_t){x, y});
			}
		}
		if (r != SUNK)
			return 0;
	}
	return 1;
}


TEST(end_game) {
	game_state_t *game = newGame();

	{
		player_t *p = newDumbPlayer();
		p->name = "Bot 1";
		game->camp_allocator->put_in_camp(game->camp_allocator, game, p);
		p = newDumbPlayer();
		p->name = "Bot 2";
		game->camp_allocator->put_in_camp(game->camp_allocator, game, p);
	}

	while (1) {
		for(unsigned i = 0; i < darraySize(game->camps); ++i) {
			camp_t *camp = *(camp_t **)darrayGet(game->camps, i);
			player_t *players = *(player_t**)darrayGet(camp->players, 0);
			size_t n = darraySize(camp->players);
			while (n--) {
				if (players->n_boats == 0) {
					++players;
					continue;
				}
				point_t coordinates;
				result_t r;
				do {
					coordinates = players->get_action(players, game);
					r = doAction(game, players, coordinates);
				} while(r == REDO);
				if (!turnEndUpdate(game))
					goto end;
				++players;
			}
		}
	}

end:;

	// Fin du jeu, nettoyage
	for (unsigned i = 0; i < darraySize(game->camps); ++i) {
		camp_t *c = *(camp_t**)darrayGet(game->camps, i);
		deleteCamp(c);
	}
	darrayDelete(game->camps);
	free(game->grid);
	free(game->camp_allocator);
	free(game);
	return 1;
}