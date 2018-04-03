/**
 * \file player.c
 * \brief Implémentation des fonctions pour la famille de type \ref player_t
 */

#include <cell.h>
#include <player.h>
#include <game_state.h>

#include <unistd.h>
#include <curses.h>
#include <string.h>

/**
 * \struct local_player_t
 * \brief Définition d'un joueur local
 */
typedef struct {
	player_t base; /*!< Instance parente */
} local_player_t;

/**
 * `printColorArray` nettoie le terminal et affiche un tableau
 * de couleur représentant un état possible du jeu
 * \param game L'état du jeu
 * \param arr Tableau de couleurs
 */
void printColorArray(game_state_t *game, const color_t *arr) {
	clear();
	refresh();
	color_t current = BLACK;
	attron(COLOR_PAIR(current));
	for (int outer_loop = 0; outer_loop < (int)game->height; ++outer_loop) {
		for (int inner_loop = 0; inner_loop < (int)game->width; ++inner_loop) {
			move(outer_loop, inner_loop * 2);
			color_t c = arr[game->width * outer_loop + inner_loop];
			if (c != current) {
				attroff(COLOR_PAIR(current));
				// current = c;
			}
			attron(COLOR_PAIR(c));
			printw("  ");
			current = c;
		}
		attron(COLOR_PAIR(BLACK));
	}
	move((int)game->height + 1, 0);
	attroff(COLOR_PAIR(BLACK));
	refresh();
}

/**
 * `blitToGrid` écrit le bitmap `piece` d'un bateau d'identifiant `id`
 * aux coordonnées `pos` dans la grille de jeu `game`
 * \param piece la pièce à placer
 * \param pos les coordonnées d'où placer la pièce
 * \param game L'état du jeu
 * \param id L'id du bateau qui sera placé
 */
void blitToGrid(piece_t *piece, point_t pos, game_state_t *game, unsigned char id) {
	for (int outer_loop = 0; outer_loop < piece->height; ++outer_loop)
		for (int inner_loop = 0; inner_loop < piece->width; ++inner_loop) {
			char v = piece->cells[piece->width * outer_loop + inner_loop] != ' ';
			if (!v)
				continue;
			if (inner_loop + pos.x >= game->width) {
				continue;
			}
			if (outer_loop + pos.y >= game->height) {
				continue;
			}
			cell_t *cell = &game->grid[game->width * (outer_loop + pos.y) + (inner_loop + pos.x)];
			cell->has_boat |= 1;
			cell->boat_id = id;
		}
}

/**
 * `blitBoat` écrit le bitmap `piece` d'un bateau aux coordonnées `pos` dans la
 * un tableau de couleur `arr` pouvant représenter un état du jeu `game`
 * \param piece la pièce à placer
 * \param arr le tableau où dessiner la pièce
 * \param pos les coordonnées d'où placer la pièce
 * \param game L'état du jeu
 * \param add pointeur vers le nombre de fragments du bateau qui ont pu être placés
 * \return le nombre de fragments du bateau sont superposé avec des éléments où un frament ne
 * peut pas être placé. Utilisé pour la gestion de collision.
 */
int blitBoat(piece_t *piece, color_t *arr, point_t pos, game_state_t *game, int *add) {
	int acc = 0;
	for (int outer_loop = 0; outer_loop < piece->height; ++outer_loop)
		for (int inner_loop = 0; inner_loop < piece->width; ++inner_loop) {
			char has_boat_piece = piece->cells[piece->width * outer_loop + inner_loop] != ' ';
			if (!has_boat_piece)
				continue;
			if (inner_loop + pos.x >= game->width) {
				acc += 1 + game->width - (inner_loop + pos.x);
				continue;
			}
			if (outer_loop + pos.y >= game->height) {
				acc += 1 + game->height - (outer_loop + pos.y);
				continue;
			}
			color_t *color = &arr[game->width * (outer_loop + pos.y) + (inner_loop + pos.x)];
			acc += (*color != CYAN);
			add[0] += 1;
			*color = WHITE;
		}
	return acc;
}

/**
 * `cursorMovement` gère une itération du mouvement d'un curseur aux
 * coordonnées pointées par `r` dans un état du jeu `game`
 * \param coords_curseur pointeur vers les coordonnées du curseur
 * \param game L'état du jeu
 */
static void cursorMovement(point_t *coords_curseur, game_state_t *game) {
	switch(getch()) {
	case '[':
		switch(getch()) {
		case 'A':
			if (coords_curseur->y > 0)
				--coords_curseur->y;
			break;
		case 'B':
			if (coords_curseur->y < game->height - 1)
				++coords_curseur->y;
			break;
		case 'C':
			if (coords_curseur->x < game->width - 1)
				++coords_curseur->x;
			break;
		case 'D':
			if (coords_curseur->x > 0)
				--coords_curseur->x;
			break;
		default:
			break;
		}
		break;
		default:break;
	}
}

/**
 * `playerLocalAction` gère un tour d'un joueur local dans le jeu `game`
 * \param self Pointeur vers l'instance parente du joueur
 * \param game L'état du jeu
 */
static point_t playerLocalAction(player_t *self, game_state_t *game) {
	point_t centre_jeu = {game->width / 2, game->height / 2};
	while (1) {
		color_t *arr = stateToView(game, self);
		arr[game->width * centre_jeu.y + centre_jeu.x] = BLACK;
		printColorArray(game, arr);
		printw("Tour du joueur %s\n\r", self->name);
		refresh();
		free(arr);
		while (1) {
			int key = getch();
			if (key == ' ')
				return centre_jeu;
			else if (key == '\033') {
				cursorMovement(&centre_jeu, game);
				break;
			}
			else if (key == 4) {
				interruptHandler(0);
				break;
			}
		}
	}
	// unreachable
	return centre_jeu;
}

/**
 * `playerLocalSetBoats` gère le placement des bateaux d'un joueur local dans le jeu `game`
 * \param self Pointeur vers l'instance parente du joueur
 * \param game L'état du jeu
 */
static void playerLocalSetBoats(player_t *self, game_state_t *game) {
	point_t prev = {0, 0};
	point_t centre_jeu = {
		(self->owned_rect[0].x + self->owned_rect[1].x) / 2,
		(self->owned_rect[0].y + self->owned_rect[1].y) / 2
	};
	for(int outer_loop = 0; outer_loop < NBBOATS;) {
		if (game->cheat > -1)
			outer_loop = game->cheat;
		piece_t boat;
		memcpy(&boat, &Pieces[outer_loop], sizeof(boat));
		boat.cells = strdup(boat.cells);
		while (1) {
			color_t *arr = stateToView(game, self);
			int k = 0;
			int coll = blitBoat(&boat, arr, centre_jeu, game, &k);
			printColorArray(game, arr);
			printw("[%s] Fleches pour deplacer, centre_jeu pour tourner, espace pour valider\n\r", self->name);
			// C'était soit switch et goto, soit une série de if-else
			int key = getch();
			if (key == ' ') {
				if (!coll) {
					prev = centre_jeu;
					++outer_loop;
					blitToGrid(&boat, centre_jeu, game, ++game->alloc_id);
					centre_jeu = (point_t) {
						(self->owned_rect[0].x + self->owned_rect[1].x) / 2,
						(self->owned_rect[0].y + self->owned_rect[1].y) / 2
					};
					if (game->cheat > -1)
						outer_loop = 10;
				}
				else
					centre_jeu = prev;
				free(arr);
				free(boat.cells);
				break;
			}
			else if (key == 'r')
				rotate(&boat);
			else if (key == '\033')
				cursorMovement(&centre_jeu, game);
			else if (key == 4)
				interruptHandler(0);
			free(arr);
		}
	}
	self->n_boats = NBBOATS;
	if (game->cheat > -1)
		self->n_boats = 1;
}

/**
 * `newLocalPlayer` crée une instance d'un joueur local sur terminal
 * \return Pointeur vers la nouvelle instance
 */
player_t *newLocalPlayer() {
	local_player_t *ret = calloc(1, sizeof(*ret));
	ret->base.get_action = playerLocalAction;
	ret->base.setup_boats = playerLocalSetBoats;
	return &ret->base;
}

/**
 * `stateToView` crée un tableau de couleur de même dimension que la grille de
 * jeu représentant le bitmap de l'état du jeu `game` du point de vue du joueur `filter`
 * \param game L'état du jeu
 * \param filter Le joueur selon le point de vue duquel on génère la vue
 * \return Tableau de couleur représentant le bitmap de l'état du jeu
 */
color_t *stateToView(game_state_t *game, player_t *filter) {
	/* TODO: Ne pas allouer ici pour éviter des allocations inutiles */
	color_t *arr = calloc(game->width * game->height, sizeof(color_t));

	static const color_t player_colors[] = {CYAN, WHITE, YELLOW, MAGENTA, BLACK, BLACK, BLACK, RED};
	static const color_t foe_colors[] = {BLUE, BLUE, CYAN, MAGENTA, BLACK, BLACK, BLACK, RED};

	for (size_t outer_loop = 0; outer_loop < game->height; ++outer_loop)
		for (size_t inner_loop = 0; inner_loop < game->width; ++inner_loop) {
			int state = game->grid[game->width * outer_loop + inner_loop].state & 0x7;
			const color_t *values = isPointInsideRect((point_t){inner_loop, outer_loop}, filter->owned_rect) ?
															(const color_t*)player_colors : foe_colors;
			arr[game->width * outer_loop + inner_loop] = values[state];
		}
	return arr;
}

void transpose(piece_t *piece) {
	char tmp[piece->width * piece->height];
	memset(tmp, 0, sizeof(tmp));
	char *iter = tmp;
	for (int outer_loop = 0; outer_loop < piece->width; ++outer_loop)
		for (int inner_loop = piece->height - 1; inner_loop >= 0; --inner_loop)
			*iter++ = piece->cells[piece->width * inner_loop + outer_loop];
	memcpy(piece->cells, tmp, sizeof(tmp));
	int width = piece->width;
	piece->width = piece->height;
	piece->height = (char) width;
}

char *strrev(char *str) {
	char *character = str;
	while (*character) ++character;
	--character;
	char *s = str;
	while (s < character) {
		char t = *s;
		*s = *character;
		*character = t;
		++s;
		--character;
	}
	return str;
}


/**
 * `rotate` effectue un certain nombre de rotation à 90° d'une pièce dans le sens horaire
 * \param piece Pièce à modifier.
 * \param rotation_nb Nombre de rotations à effectuer.
 */
void rotate(piece_t *piece) {
	transpose(piece);
}

/// \brief Tableau des pièces représentant un type de bateau
piece_t Pieces[NBBOATS] = {
	{2, 2,
	 "xx"
	 "xx"},
	{4, 2,
	 "xxxx"
	 "xxxx"},
	{2, 1,
	 "xx"},
	{3, 1,
	 "xxx"},
	{4, 1,
	 "xxxx"},
	{5, 1,
	 "xxxxx"},
	{4, 3,
	 "xxxx"
	 "   x"
	 "   x"},
};
