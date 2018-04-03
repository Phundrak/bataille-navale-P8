/**
 * \file game_logic.c
 * \brief Implémentation des fonctions pour le type \ref game_state_t
 */

#include <cell.h>
#include <game_state.h>
#include <darray.h>
#include <camp.h>
#include <camp_allocator.h>
#include <player.h>

/* TODO:
	 Ajouter la possibilité de passer des options, notamment
	 nombre de joueurs
	 nombre d'équipes
	 type de partie
 */
/**
 * `newGame` créé un nouvel état de jeu avec une grille de 34*17
 * avec un allocateur d'équipe par défaut et la triche désactivé
 * \return Un nouvel état de jeu
 */
game_state_t *newGame(option_t opt) {
	game_state_t *ret = calloc(sizeof(*ret), 1);

	ret->width = opt.width * 2;
	ret->height = (size_t) opt.height;
	ret->grid = calloc(sizeof(cell_t), ret->width * ret->height);
	ret->camps = darrayNew(sizeof(camp_t *));
	ret->cheat = -1;

	// Chacun pour soi
	ret->camp_allocator = newSingleAllocator();
	return ret;
}

/**
 * `isSunk` teste si le bateau d'identifiant `id` est coulé en observant à
 * partir des coordonnées `cp` dans la grille du jeu `game`.
 * \param game Le jeu \ref game_state_t dans lequel se situe le test à faire
 * \param point Les coordonnées \ref point_t à partie desquelles commencer le test
 * \param id l'identifiant du bateau à tester
 * \return 0 si le bateau n’est pas coulé, 1 sinon
 */
int isSunk(game_state_t *game, point_t point, int id) {
	cell_t *neighbours[4] = {0};
	cell_t *cell = &game->grid[game->width * point.y + point.x];

	if (!cell->has_exploded)
		return 0;

	if (point.y != 0)
		neighbours[0] = cell - game->width;
	if (point.y != game->height - 1)
		neighbours[1] = cell + game->width;
	if (point.x != 0)
		neighbours[2] = cell - 1;
	if (point.x != game->width - 1)
		neighbours[3] = cell + 1;

	// considéré coulé si tout ses voisins existants du meme id
	// ont explosé
	for (int loop = 0; loop < 4; ++loop) {
		if (!neighbours[loop])
			continue;
		if (neighbours[loop]->boat_id != id)
			continue;
		if (!neighbours[loop]->has_exploded)
			return 0;
		if (!neighbours[loop]->marked) {
			cell->marked |= 1;
			if (!isSunk(game, getCoordinates(game, neighbours[loop]), id)) {
				cell->marked = 0;
				return 0;
			}
			cell->marked = 0;
		}
	}
	return 1;
}

/**
 * `setSunk` marque le bateau d'identifiant `id` comme coulé en observant à
 * partir des coordonnées `cp` dans la grille du jeu `game`.
 * \param game Le jeu \ref game_state_t dans lequel se situe le marquage à faire
 * \param point Les coordonnées \ref point_t à partie desquelles commencer le marquage
 * \param id l'identifiant du bateau à marquer
 */
void setSunk(game_state_t *game, point_t point, int id) {
	cell_t *neighbours[4] = {0};
	cell_t *cell = &game->grid[game->width * point.y + point.x];

	if (!cell->has_exploded)
		return;

	if (point.y != 0)
		neighbours[0] = cell - game->width;
	if (point.y != game->height - 1)
		neighbours[1] = cell + game->width;
	if (point.x != 0)
		neighbours[2] = cell - 1;
	if (point.x != game->width - 1)
		neighbours[3] = cell + 1;

	for (int loop = 0; loop < 4; ++loop) {
		if (!neighbours[loop])
			continue;
		if (neighbours[loop]->boat_id != id)
			continue;
		if (!neighbours[loop]->has_sunk) {
			cell->has_sunk |= 1;
			setSunk(game, getCoordinates(game, neighbours[loop]), id);
		}
	}
	cell->has_sunk |= 1;
}

/**
 * `findOwner` renvoie un pointeur vers un objet de type \ref player_t
 * correspondant au joueur à qui appartient la case aux coordonnées `p`, pour un jeu `game`
 * \param game Le jeu \ref game_state_t dans lequel se situe la recherche à faire
 * \param point Les coordonnées \ref point_t pour lequel on cherche le propriétaire
 * \return Un pointeur vers le joueur à qui appartient la case
 */
player_t *findOwner(game_state_t *game, point_t point) {
	for (unsigned outer_loop = 0; outer_loop < darraySize(game->camps); ++outer_loop) {
		camp_t *camp = *(camp_t**)darrayGet(game->camps, outer_loop);
		for (unsigned inner_loop = 0; inner_loop < darraySize(camp->players); ++inner_loop) {
			player_t *player = *(player_t**)darrayGet(camp->players, inner_loop);
			if (isPointInsideRect(point, player->owned_rect))
				return player;
		}
	}
	return 0;
}

/**
 * `doAction` fait exploser une case de la part d'un joueur `player` dans
 * le jeu `game`
 * \param game L'état de jeu à modifier
 * \param player Le joueur qui réalise l'action
 * \param point Le point sur lequel est réalisé l'action
 * \return L'état de l'action représenté par \ref result_t
 */
result_t doAction(game_state_t *game, player_t *player, point_t point) {
	cell_t *cell;

	cell = &game->grid[game->width * point.y + point.x];
	if (isPointInsideRect(point, player->owned_rect) || cell->has_exploded)
		return REDO;
	cell->has_exploded |= 1;
	if (cell->has_boat) {
		if (isSunk(game, point, cell->boat_id)) {
			setSunk(game, point, cell->boat_id);
			player_t *target = findOwner(game, point);
			--target->n_boats;
			return SUNK;
		}
		return HIT;
	}
	return MISS;
}

/**
 * Vérifie si le jeu est fini.
 * \param self L'état de jeu à modifier
 * \return 0 si le jeu n'est pas fini, ou une valeur non nulle dans le cas contraire.
 */
int turnEndUpdate(game_state_t *game) {
	int n_alive = 0;

	for (unsigned inner_loop = 0; inner_loop < darraySize(game->camps); ++inner_loop) {
		camp_t *camp = *(camp_t**)darrayGet(game->camps, inner_loop);
		camp->is_alive = 0;
		for (unsigned outer_loop = 0; outer_loop < darraySize(camp->players); ++outer_loop) {
			player_t *player = *(player_t**)darrayGet(camp->players, outer_loop);
			camp->is_alive = camp->is_alive || player->n_boats;
		}
		n_alive += camp->is_alive;
		if (camp->is_alive)
			game->winning = camp;
	}

	return n_alive > 1;
}

/**
 * Vérifie si un point est dans un rectangle
 * \param point Le point à tester
 * \param rect Le rectangle à tester
 * \return 0 si le point n'est pas dans le rectangle, ou une valeur non nulle dans le cas contraire.
 */
int isPointInsideRect(point_t point, point_t rect[2]) {
	return point.x >= rect[0].x && point.x < rect[1].x &&
		point.y >= rect[0].y && point.y < rect[1].y;
}

/**
 * Recupère un pointeur vers une cellule dans une grille de jeu selon ses coordonnées
 * déréférencer le pointeur renvoyé par cette fonction est un comportement indéfini
 * si \a co est en dehors de la grille de jeu
 * \param game Le jeu qui contient la grille dans laquelle chercher.
 * \param point Les coordonnées du point.
 * \return Un pointeur vers la cellule aux coordonnées `co` de type \ref cell_t
 */
cell_t *getCell(game_state_t *game, point_t point) {
	return &game->grid[game->width * point.y + point.x];
}

/**
 * Recupère les coordonnées d'une cellule dans une grille de jeu selon son pointeur
 * \param game Le jeu qui contient la grille dans laquelle chercher.
 * \param co Un pointeur vers la cellule.
 * \return Les coordonnées de la cellule pointée par `c`
 */
point_t getCoordinates(game_state_t *game, cell_t *cell) {
	unsigned long dist = cell - game->grid;
	return (point_t) {
		dist % game->width,
		dist / game->width
	};
}
