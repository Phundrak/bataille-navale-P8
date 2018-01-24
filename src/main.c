#include <game_state.h>

int main(int argc, char *argv[argc])
{
	(void)argv;
	(void)argc;
	
	game_state_t *game = new_game();

	// Cas d'un jeu deux joueurs local
	{
		// J1
		game->camp_allocator->put_in_camp(game->camp_allocator, game, new_local_player());
		// J2
		game->camp_allocator->put_in_camp(game->camp_allocator, game, new_local_player());
	}

	do {
		for(unsigned i = 0; i < darray_size(game->camps); ++i) {
			camp_t *camp = *(camp_t **)darray_get(game->camps, i);
			player_t *players = darray_get(camp->players, 0);
			unsigned n = darray_size(camp->players);
			while (n--) {
				result_t r;
				point_t coordinates;
				do {
					coordinates = players->get_action(players, game);
				} while ((r = do_action(game, players, coordinates)) == REDO);
				++players;
			}
		}
	} while (turn_end_update(game));
    return 0;
}
