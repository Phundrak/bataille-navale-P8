#include <cell.h>
#include <game_state.h>
#include <point.h>
#include <stdlib.h>

/* TODO:
   Ajouter la possibilité de passer des options, notamment
   nombre de joueurs
   nombre d'équipes
   type de partie
 */
game_state_t *newGame(/*Options*/) {
  game_state_t *ret = calloc(sizeof(*ret), 1);

  ret->width = 17 * 2;
  ret->height = 17;
  ret->grid = calloc(sizeof(cell_t), (size_t)(ret->width * ret->height));
  ret->camps = darrayNew(sizeof(camp_t *));

  // Chacun pour soi
  ret->camp_allocator = newSingleAllocator();
  return ret;
}

int drownBoat(struct game_state_s *game, int x, int y, unsigned char boat_id) {
  cell_t *current_cell;

  /* La fonction ne renvoie 0 (zero, `faux`) que si la fonction ne renvoie 0
   * (zero, `faux`) que si une pièce du bateau testé n'a pas été touchée */

  current_cell = &(game->grid[17 * y + x]);

  if (current_cell->boat_id != boat_id)
    return 1;

  if (!current_cell->has_exploded)
    return 0;

  /* si possible, test de la cellule en haut */
  if ((x % 17) - 1 > 0)
    if (!drownBoat(game, x - 1, y, boat_id))
      return 0;

  /* si possible, on teste en bas */
  if ((x % 17) + 1 > 16)
    if (!drownBoat(game, x + 1, y, boat_id))
      return 0;

  /* si possible, on teste à gauche */
  if ((y % 17) - 1 > 0)
    if (!drownBoat(game, x, y - 1, boat_id))
      return 0;

  /* si possible, on teste à droite */
  if ((y % 17) + 1 > 0)
    if (!drownBoat(game, x, y + 1, boat_id))
      return 0;

  return 1;
}

result_t hitCell(struct game_state_s *game, point_t point) {
  cell_t *current_cell;

  current_cell = &(game->grid[17 * point.y + point.x]);
  if (current_cell->has_exploded == 1)
    return REDO;
  current_cell->has_exploded = 1;
  if (current_cell->has_boat == 0)
    return MISS;

  if (drownBoat(game, point.x, point.y, current_cell->boat_id))
    return DROWN;
  return HIT;
}
