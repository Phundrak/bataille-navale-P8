#include <darray.h>
#include <game_state.h>
#include <player.h>
#include <stdio.h>

int main(int argc, char *argv[argc]) {
	(void)argv;
	(void)argc;

	/* game_state_t *game = newGame(); */

	/* // Cas d'un jeu deux joueurs local */
	/* { */
	/*   // J1 */
	/*   game->camp_allocator->put_in_camp(game->camp_allocator, game, */
	/*                                     newLocalPlayer()); */
	/*   // J2 */
	/*   game->camp_allocator->put_in_camp(game->camp_allocator, game, */
	/*                                     newLocalPlayer()); */
	/* } */

	char pieces[][5][5] = {{{1, 1, 0, 0, 0},
													{1, 1, 0, 0, 0},
													{0, 0, 0, 0, 0},
													{0, 0, 0, 0, 0},
													{0, 0, 0, 0, 0}},
												 {{1, 1, 1, 1, 0},
													{1, 1, 1, 1, 0},
													{0, 0, 0, 0, 0},
													{0, 0, 0, 0, 0},
													{0, 0, 0, 0, 0}},
												 {{1, 1, 0, 0, 0},
													{0, 0, 0, 0, 0},
													{0, 0, 0, 0, 0},
													{0, 0, 0, 0, 0},
													{0, 0, 0, 0, 0}},
												 {{1, 1, 1, 0, 0},
													{0, 0, 0, 0, 0},
													{0, 0, 0, 0, 0},
													{0, 0, 0, 0, 0},
													{0, 0, 0, 0, 0}},
												 {{1, 1, 1, 1, 0},
													{0, 0, 0, 0, 0},
													{0, 0, 0, 0, 0},
													{0, 0, 0, 0, 0},
													{0, 0, 0, 0, 0}},
												 {{1, 1, 1, 1, 1},
													{0, 0, 0, 0, 0},
													{0, 0, 0, 0, 0},
													{0, 0, 0, 0, 0},
													{0, 0, 0, 0, 0}},
												 {{1, 1, 1, 1, 0},
													{0, 0, 0, 1, 0},
													{0, 0, 0, 1, 0},
													{0, 0, 0, 0, 0},
													{0, 0, 0, 0, 0}}};

	int outer_loop;
	for (outer_loop = 0; outer_loop < 7; ++outer_loop) {
			printPiece(pieces[outer_loop]);
		printf("\n");
	}
	printf("\nRotation (from 0 times to 6 times):\n");
	for (outer_loop = 0; outer_loop < 7; ++outer_loop) {
		rotate(pieces[outer_loop], outer_loop);
			printPiece(pieces[outer_loop]);
		printf("\n");
	}

	/* do { */
	/*   for (unsigned i = 0; i < darraySize(game->camps); ++i) { */
	/*     camp_t *camp = *(camp_t **)darrayGet(game->camps, i); */
	/*     player_t *players = darrayGet(camp->players, 0); */
	/*     size_t n = darraySize(camp->players); */
	/*     while (n--) { */
	/*       result_t r; */
	/*       point_t coordinates; */
	/*       do { */
	/*         coordinates = players->get_action(players, game); */
	/*       } while ((r = doAction(game, players, coordinates)) == REDO); */
	/*       ++players; */
	/*     } */
	/*   } */
	/* } while (turnEndUpdate(game)); */
	return 0;
}
