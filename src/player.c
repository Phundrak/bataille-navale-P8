#include <player.h>
#include <stdlib.h>

typedef struct {
  player_t base;
} local_player_t;

static point_t playerLocalAction(player_t *self, game_state_t *game) {
  /*
     TODO:
     Afficher l'état du jeu
     demander entrée coordonnées de tir
     Lucien : rotation des pièces
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
