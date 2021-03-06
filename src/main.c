/**
 * \file main.c
 * \brief Implémentation de la boucle principale de jeu et gestion du terminal
 */

#include <darray.h>
#include <game_state.h>
#include <camp.h>
#include <camp_allocator.h>
#include <player.h>

#include <signal.h>
#include <stdio.h>
#include <curses.h>
#include <string.h>

/**
 * \brief Fonction principale du programme
 *
 * Point d'entrée du programme. Un argument active le mode rapide, où chaque
 * joueur ne doit placer qu'un bateau dont l'indice correspond à l'argument
 *
 * \param argc Le nombre d'arguments passé au programme.
 * \param argv Un tableau de taille argc terminé par un pointeur nul correspondant aux arguments sous forme de chaine passé au programme.
 * \return L'état de sortie du programme
 */
int main(int argc, char *argv[argc])
{
	struct sigaction s;
	memset(&s, 0, sizeof(s));
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
	start_color();

	init_pair(1, COLOR_BLACK, COLOR_BLACK);
	init_pair(2, COLOR_BLACK, COLOR_RED);
	init_pair(3, COLOR_BLACK, COLOR_GREEN);
	init_pair(4, COLOR_BLACK, COLOR_YELLOW);
	init_pair(5, COLOR_BLACK, COLOR_BLUE);
	init_pair(6, COLOR_BLACK, COLOR_MAGENTA);
	init_pair(7, COLOR_BLACK, COLOR_CYAN);
	init_pair(8, COLOR_BLACK, COLOR_WHITE);

	for (int loop = 0; loop < NBBOATS; ++loop)
		Pieces[loop].cells = strdup(Pieces[loop].cells);

	game_state_t *game = newGame((option_t) {17, 17});

	if (argc >= 2) {
		game->cheat = atoi(argv[1]); // NOLINT
		if (game->cheat < 0 || game->cheat >= NBBOATS)
			game->cheat = -1;
	}

	// Cas d'un jeu deux joueurs local
	{
		player_t *player = newLocalPlayer();
		player->name = "Joueur 1";
		game->camp_allocator->put_in_camp(game->camp_allocator, game, player);
		player = newLocalPlayer();
		player->name = "Joueur 2";
		game->camp_allocator->put_in_camp(game->camp_allocator, game, player);
	}

	while (1) {
		for(unsigned outer_loop = 0; outer_loop < darraySize(game->camps); ++outer_loop) {
			camp_t *camp = *(camp_t **)darrayGet(game->camps, outer_loop);
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
					clear();
					refresh();
					switch (result) {
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
				} while(result == REDO);
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
		camp_t *camp = *(camp_t**)darrayGet(game->camps, i);
		deleteCamp(camp);
	}
	darrayDelete(game->camps);
	free(game->grid);
	free(game->camp_allocator);
	free(game);

	printf("Appuyez sur une touche pour terminer\n\r");
	refresh();

	for (int i = 0; i < NBBOATS; ++i)
		free(Pieces[i].cells);

	getch();
	interruptHandler(0);
	return 0;
}
