#include <player.h>
#include <stdlib.h>
#ifdef Debug
#include <memory.h>
#include <stdio.h>

#endif

#define PIECE_SIZE 5

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

void cyclic_roll(char *a, char *b, char *c, char *d) {
  char temp = *a;
  *a = *b;
  *b = *c;
  *c = *d;
  *d = temp;
}

int empty_line(char piece[PIECE_SIZE][PIECE_SIZE], int line) {
  int loop;
  for (loop = 0; loop < PIECE_SIZE; ++loop)
    if (piece[line][loop] != 0)
      return 0;
  return 1;
}

int empty_collumn(char piece[PIECE_SIZE][PIECE_SIZE], int collumn) {
  int loop;
  for (loop = 0; loop < PIECE_SIZE; ++loop)
    if (piece[loop][collumn] != 0)
      return 0;
  return 1;
}

void shift_collumn_left(char piece[PIECE_SIZE][PIECE_SIZE]) {
  int collumn;
  for (collumn = 0; collumn < PIECE_SIZE; ++collumn) {
    memmove(&piece[collumn][0], &piece[collumn][1], 4);
    piece[collumn][PIECE_SIZE - 1] = 0;
  }
}

void shift_line_up(char piece[PIECE_SIZE][PIECE_SIZE]) {
  int collumn, line;
  for (line = 0; line < PIECE_SIZE - 1; ++line) {
    memmove(&piece[line], &piece[line + 1], sizeof(char *));
  }
  for (collumn = 0; collumn < PIECE_SIZE; ++collumn)
    piece[PIECE_SIZE - 1][collumn] = 0;
  //  for(line = 0; line < PIECE_SIZE - 1; ++line)
  //    for(collumn = 0; collumn < PIECE_SIZE; ++collumn)
  //      piece[collumn][line] = piece[collumn][line + 1];
  //  for(collumn = 0; collumn < PIECE_SIZE; ++collumn)
  //    piece[collumn][PIECE_SIZE - 1] = 0;
}

void realign_piece(char piece[PIECE_SIZE][PIECE_SIZE]) {
  while (empty_collumn(piece, 0))
    shift_collumn_left(piece);
  while (empty_line(piece, 0))
    shift_line_up(piece);
}

void rotate(char piece[5][5], int rotation_nb) {
  int outer_loop, inner_loop, rotations;
  for (rotations = 0; rotations < rotation_nb; ++rotations) {
    for (outer_loop = 0; outer_loop < PIECE_SIZE / 2; ++outer_loop)
      for (inner_loop = 0; inner_loop < (PIECE_SIZE + 1) / 2; ++inner_loop)
        cyclic_roll(
            &piece[outer_loop][inner_loop],
            &piece[PIECE_SIZE - 1 - inner_loop][outer_loop],
            &piece[PIECE_SIZE - 1 - outer_loop][PIECE_SIZE - 1 - inner_loop],
            &piece[inner_loop][PIECE_SIZE - 1 - outer_loop]);
  }
  realign_piece(piece);
}

/* debug */
void print_piece(char piece[PIECE_SIZE][PIECE_SIZE]) {
#ifdef Debug
  int inner_loop, outer_loop;
  for (outer_loop = 0; outer_loop < PIECE_SIZE; ++outer_loop) {
    for (inner_loop = 0; inner_loop < PIECE_SIZE; ++inner_loop)
      printf("%d ", piece[outer_loop][inner_loop]);
    printf("\n");
  }
#endif
}
