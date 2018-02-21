/**
 * \file player.c
 * \brief Implémentation des fonctions pour la famille de type \ref player_t
 */

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

extern char Pieces[][PIECE_SIZE][PIECE_SIZE];

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
	for (int i = 0; i < (int)game->height; ++i) {
		for (int j = 0; j < (int)game->width; ++j) {
			move(i, j * 2);
			color_t c = arr[game->width * i + j];
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
void blitToGrid(char (*piece)[5][5], point_t pos, game_state_t *game, unsigned char id) {
	for (int i = 0; i < 5; ++i)
		for (int j = 0; j < 5; ++j) {
			char v = (*piece)[i][j];
			if (!v)
				continue;
			if (j + pos.x >= game->width) {
				continue;
			}
			if (i + pos.y >= game->height) {
				continue;
			}
			cell_t *c = &game->grid[game->width * (i + pos.y) + (j + pos.x)];
			c->has_boat |= 1;
			c->boat_id = id;
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
int blitBoat(char (*piece)[5][5], color_t *arr, point_t pos, game_state_t *game, int *add) {
	int acc = 0;
	for (int i = 0; i < 5; ++i)
		for (int j = 0; j < 5; ++j) {
			char v = (*piece)[i][j];
			if (!v)
				continue;
			if (j + pos.x >= game->width) {
				acc += 1 + game->width - (j + pos.x);
				continue;
			}
			if (i + pos.y >= game->height) {
				acc += 1 + game->height - (i + pos.y);
				continue;
			}
			color_t *c = &arr[game->width * (i + pos.y) + (j + pos.x)];
			acc += (*c != CYAN);
			add[0] += 1;
			*c = WHITE;
		}
	return acc;
}

/**
 * `cursorMovement` gère une itération du mouvement d'un curseur aux 
 * coordonnées pointées par `r` dans un état du jeu `game`
 * \param r pointeur vers les coordonnées du curseur
 * \param game L'état du jeu
 */
static void cursorMovement(point_t *r, game_state_t *game) {
	switch(getch()) {
	case '[':
		switch(getch()) {
		case 'A':
			if (r->y > 0)
				--r->y;
			break;
		case 'B':
			if (r->y < game->height - 1)
				++r->y;
			break;
		case 'C':
			if (r->x < game->width - 1)
				++r->x;
			break;
		case 'D':
			if (r->x > 0)
				--r->x;
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
	point_t r = {game->width / 2, game->height / 2};
	while (1) {
		color_t *arr = stateToView(game, self);
		arr[game->width * r.y + r.x] = BLACK;
		printColorArray(game, arr);
		printw("Tour du joueur %s\n\r", self->name);
		refresh();
		free(arr);
		while (1) {
			int c = getch();
			if (c == ' ')
				return r;
			else if (c == '\033') {
				cursorMovement(&r, game);
				break;
			}
			else if (c == 4) {
				interruptHandler(0);
				break;
			}
		}
	}
	// unreachable
	return r;
}

/**
 * `playerLocalSetBoats` gère le placement des bateaux d'un joueur local dans le jeu `game`
 * \param self Pointeur vers l'instance parente du joueur
 * \param game L'état du jeu
 */
static void playerLocalSetBoats(player_t *self, game_state_t *game) {
	point_t prev = {0, 0};
	point_t r = {
		(self->owned_rect[0].x + self->owned_rect[1].x) / 2,
		(self->owned_rect[0].y + self->owned_rect[1].y) / 2
	};
	for(int i = 0; i < 7;) {
		if (game->cheat > -1)
			i = game->cheat;
		char boat_mem[5][5];
		char (*boat)[5][5] = &boat_mem;
		memcpy(boat, Pieces[i], sizeof(boat_mem));
		while (1) {
			color_t *arr = stateToView(game, self);
			int k = 0;
			int coll = blitBoat(boat, arr, r, game, &k);
			printColorArray(game, arr);
			printw("[%s] Fleches pour deplacer, r pour tourner, espace pour valider\n\r", self->name);
			// C'était soit switch et goto, soit une série de if-else
			int c = getch();
			if (c == ' ') {
				if (!coll) {
					prev = r;
					++i;
					blitToGrid(boat, r, game, ++game->alloc_id);
					r = (point_t) {
						(self->owned_rect[0].x + self->owned_rect[1].x) / 2,
						(self->owned_rect[0].y + self->owned_rect[1].y) / 2
					};
					if (game->cheat > -1)
						i = 10;
				}
				else
					r = prev;
				free(arr);
				break;
			}
			else if (c == 'r') 
				rotate(*boat, 1);
			else if (c == '\033') 
				cursorMovement(&r, game);
			else if (c == 4) 
				interruptHandler(0);
			free(arr);
		}
	}
	self->n_boats = 7;
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

	for (size_t i = 0; i < game->height; ++i)
		for (size_t j = 0; j < game->width; ++j) {
			int state = game->grid[game->width * i + j].state & 0x7;
			const color_t *values = isPointInsideRect((point_t){j, i}, filter->owned_rect) ?
															(const color_t*)player_colors : foe_colors;
			arr[game->width * i + j] = values[state];
		}
	return arr;
}

/**
 * `cyclicRoll` fait une rotation dans un sens des quatres caractères pointés par a, b, c, et d.
 * \param a Un pointeur vers un caractère.
 * \param b Un pointeur vers un caractère.
 * \param c Un pointeur vers un caractère.
 * \param d Un pointeur vers un caractère.
 * \return Tableau de couleur représentant le bitmap de l'état du jeu
 */
void cyclicRoll(char *a, char *b, char *c, char *d) {
	char temp = *a;
	*a = *b;
	*b = *c;
	*c = *d;
	*d = temp;
}

/**
 * `emptyLine` détermine si la ligne `line` est vide.
 * \param piece Pièce à vérifier.
 * \param line Indice de la ligne à vérifier.
 * \return 0 si la ligne contient au moins un élément, ou 1 dans le cas contraire
 */
int emptyLine(char piece[PIECE_SIZE][PIECE_SIZE], int line) {
	int loop;
	for (loop = 0; loop < PIECE_SIZE; ++loop)
		if (piece[line][loop] != 0)
			return 0;
	return 1;
}

/**
 * `emptyColumn` détermine si la colonne `column` est vide.
 * \param piece Pièce à vérifier.
 * \param column Indice de la ligne à vérifier.
 * \return 0 si la colonne contient au moins un élément, ou 1 dans le cas contraire
 */
int emptyColumn(char piece[PIECE_SIZE][PIECE_SIZE], int column) {
	int loop;
	for (loop = 0; loop < PIECE_SIZE; ++loop)
		if (piece[loop][column] != 0)
			return 0;
	return 1;
}

/**
 * `shiftColumnLeft` déplace tout les éléments d'une pièce d'une colonne vers la gauche.
 * \param piece Pièce à modifier.
 */
void shiftColumnLeft(char piece[PIECE_SIZE][PIECE_SIZE]) {
	int column;
	for (column = 0; column < PIECE_SIZE; ++column) {
		memmove(&piece[column][0], &piece[column][1], PIECE_SIZE - 1);
		piece[column][PIECE_SIZE - 1] = 0;
	}
}

/**
 * `shiftLineUp` déplace tout les éléments d'une pièce d'une ligne vers le haut.
 * \param piece Pièce à modifier.
 */
void shiftLineUp(char piece[PIECE_SIZE][PIECE_SIZE]) {
	memmove(&piece[0][0], &piece[1][0], sizeof(piece[0]) * (PIECE_SIZE - 1));
	memset(&piece[PIECE_SIZE-1], 0, PIECE_SIZE);
}

/**
 * `realignPiece` élimine les lignes et colonnes vides d'une pièces
 * \param piece Pièce à modifier.
 */
void realignPiece(char piece[PIECE_SIZE][PIECE_SIZE]) {
	while (emptyColumn(piece, 0))
		shiftColumnLeft(piece);
	while (emptyLine(piece, 0))
		shiftLineUp(piece);
}

/**
 * `rotate` effectue un certain nombre de rotation à 90° d'une pièce dans le sens horaire
 * \param piece Pièce à modifier.
 * \param rotation_nb Nombre de rotations à effectuer.
 */
void rotate(char piece[5][5], int rotation_nb) {
	int outer_loop, inner_loop, rotations;
	for (rotations = 0; rotations < rotation_nb; ++rotations) {
		for (outer_loop = 0; outer_loop < PIECE_SIZE / 2; ++outer_loop)
			for (inner_loop = 0; inner_loop < (PIECE_SIZE + 1) / 2; ++inner_loop)
				cyclicRoll(
						&piece[outer_loop][inner_loop],
						&piece[PIECE_SIZE - 1 - inner_loop][outer_loop],
						&piece[PIECE_SIZE - 1 - outer_loop][PIECE_SIZE - 1 - inner_loop],
						&piece[inner_loop][PIECE_SIZE - 1 - outer_loop]);
	}
	realignPiece(piece);
}

/// \brief Tableau des pièces représentant un type de bateau
char Pieces[][PIECE_SIZE][PIECE_SIZE] = {{{1, 1, 0, 0, 0},
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
