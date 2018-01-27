#include <game_state.h>

int main(int argc, char *argv[argc])
{
	(void)argv;
	(void)argc;
	
	game_state_t *game = newGame();

	// Cas d'un jeu deux joueurs local
	{
		// J1
		game->camp_allocator->put_in_camp(game->camp_allocator, game, newLocalPlayer());
		// J2
		game->camp_allocator->put_in_camp(game->camp_allocator, game, newLocalPlayer());
	}

	do {
		for(unsigned i = 0; i < darraySize(game->camps); ++i) {
			camp_t *camp = *(camp_t **)darrayGet(game->camps, i);
			player_t *players = darrayGet(camp->players, 0);
			unsigned n = darraySize(camp->players);
			while (n--) {
				result_t r;
				point_t coordinates;
				do {
					coordinates = players->get_action(players, game);
				} while ((r = doAction(game, players, coordinates)) == REDO);
				++players;
			}
		}
	} while (turnEndUpdate(game));
    return 0;
}
