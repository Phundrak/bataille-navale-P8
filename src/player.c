#include <player.h>
#include <stdlib.h>
#include <memory.h>

#ifdef Debug
#include <stdio.h>
#endif

typedef struct {
	player_t base;
} local_player_t;

static point_t playerLocalAction(player_t *self, game_state_t *game) {
	/*
		 TODO:
		 Afficher l'état du jeu
		 demander entrée coordonnées de tir
	*/
	return (point_t){0, 0};
}

static void playerLocalSetBoats(player_t *self, game_state_t *game) {
	local_player_t *p = (void *)self;
}

player_t *newLocalPlayer() {
	local_player_t *ret = malloc(sizeof(*ret));
	ret->base.get_action = playerLocalAction;
	ret->base.setup_boats = playerLocalSetBoats;
	return &ret->base;
}

char pieces[][PIECE_SIZE][PIECE_SIZE] = {{{1, 1, 0, 0, 0},
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

void cyclicRoll(char *a, char *b, char *c, char *d) {
	char temp = *a;
	*a = *b;
	*b = *c;
	*c = *d;
	*d = temp;
}

int emptyLine(char piece[PIECE_SIZE][PIECE_SIZE], int line) {
	int loop;
	for (loop = 0; loop < PIECE_SIZE; ++loop)
		if (piece[line][loop] != 0)
			return 0;
	return 1;
}

int emptyColumn(char piece[PIECE_SIZE][PIECE_SIZE], int column) {
	int loop;
	for (loop = 0; loop < PIECE_SIZE; ++loop)
		if (piece[loop][column] != 0)
			return 0;
	return 1;
}

void shiftColumnLeft(char piece[PIECE_SIZE][PIECE_SIZE]) {
	int column;
	for (column = 0; column < PIECE_SIZE; ++column) {
		memmove(&piece[column][0], &piece[column][1], PIECE_SIZE - 1);
		piece[column][PIECE_SIZE - 1] = 0;
	}
}

void shiftLineUp(char piece[PIECE_SIZE][PIECE_SIZE]) {
	memmove(&piece[0][0], &piece[1][0], sizeof(piece[0]) * (PIECE_SIZE - 1));
	memset(&piece[PIECE_SIZE-1], 0, PIECE_SIZE);
}

void realignPiece(char piece[PIECE_SIZE][PIECE_SIZE]) {
	while (emptyColumn(piece, 0))
		shiftColumnLeft(piece);
	while (emptyLine(piece, 0))
		shiftLineUp(piece);
}

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

/* debug */
void printPiece(char piece[PIECE_SIZE][PIECE_SIZE]) {
#ifdef Debug
	int inner_loop, outer_loop;
	for (outer_loop = 0; outer_loop < PIECE_SIZE; ++outer_loop) {
		for (inner_loop = 0; inner_loop < PIECE_SIZE; ++inner_loop)
			printf("%d ", piece[outer_loop][inner_loop]);
		printf("\n");
	}
#endif
}
