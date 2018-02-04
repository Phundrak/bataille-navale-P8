#ifndef PLAYER_H
#define PLAYER_H

#include "point.h"

#define PIECE_SIZE 5

struct game_state_s;
typedef struct game_state_s game_state_t;

typedef struct player_s {
	point_t (*get_action)(struct player_s *, game_state_t *);
	void (*setup_boats)(struct player_s *, game_state_t *);
	point_t owned_rect[2];
	int is_alive;
} player_t;

player_t *newLocalPlayer();

/* rotation d'une pièce du joueur de 90° dans le sens des aiguilles d'une
   montre */
void rotate(char piece[5][5], int rotation_nb);

/* debug */
void printPiece(char piece[PIECE_SIZE][PIECE_SIZE]);

#endif /* PLAYER_H */
