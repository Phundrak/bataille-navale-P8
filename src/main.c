#include <signal.h>
#include <darray.h>
#include <game_state.h>
#include <player.h>
#include <stdio.h>
#include <curses.h>

void interruptHandler(int s) {
	(void)s;
	resetty();
	reset_shell_mode();
	clear();
	echo();
	timeout(0);
	curs_set(1);
	endwin();
	exit(127);
}

int main(int argc, char *argv[argc])
{
	struct sigaction s = {0};

	s.sa_handler = interruptHandler;
	sigemptyset(&s.sa_mask);	
	sigaction(SIGINT, &s, 0);

	savetty();
	initscr();
	clear();
	noecho();
	timeout(-1);
	curs_set(0);
	refresh();
	(void)argv;
	(void)argc;
	
	game_state_t *game = newGame();

	// Cas d'un jeu deux joueurs local
	{
		player_t *p = newLocalPlayer();
		p->name = "Joueur 1";
		game->camp_allocator->put_in_camp(game->camp_allocator, game, p);
		p = newLocalPlayer();
		p->name = "Joueur 2";
		game->camp_allocator->put_in_camp(game->camp_allocator, game, p);
	}

	while (1) {
		for(unsigned i = 0; i < darraySize(game->camps); ++i) {
			camp_t *camp = *(camp_t **)darrayGet(game->camps, i);
			player_t *players = *(player_t**)darrayGet(camp->players, 0);
			unsigned n = darraySize(camp->players);
			while (n--) {
				if (players->n_boats == 0) {
					++players;
					continue;
				}
				result_t r = REDO;
				point_t coordinates;
				do {
					coordinates = players->get_action(players, game);
					r = doAction(game, players, coordinates);
					clear();
					refresh();
					switch (r) {
					case REDO:
						break;
					case HIT:
						puts("Touché!");
						break;
					case MISS:
						puts("Raté!");
						break;
					case SUNK:
						puts("Coulé!");
						break;
					}
					printf("\r");
					fflush(stdout);
				} while(r == REDO);
				if (!turnEndUpdate(game))
					goto end;
				printf("C'est le tour du joueur suivant, appuyez sur espace\n\r");
				while (getch() != 32);
				++players;
			}
		}
	}

end:;

	player_t admin = {.owned_rect = {{0, 0}, {game->width, game->height}}};
	void *bitmap = stateToView(game, &admin);
	printColorArray(game, bitmap);
	free(bitmap);
	printf("L'équipe gagnante est { %s }\n\r", campTeamString(game->winning));
	
	// Fin du jeu, nettoyage
	for (unsigned i = 0; i < darraySize(game->camps); ++i) {
		camp_t *c = *(camp_t**)darrayGet(game->camps, i);
		deleteCamp(c);
	}
	darrayDelete(game->camps);
	free(game->grid);
	free(game->camp_allocator);
	free(game);

	printf("Appuyez sur une touche pour terminer\n\r");
	refresh();
	
	getch();
	interruptHandler(0);
    return 0;
}
