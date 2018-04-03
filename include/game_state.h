/**
 * \file game_state.h
 * \brief Définition de l'état du jeu et déclaration des fonctions manipulant cet état
 */
#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "result.h"
#include "point.h"

typedef struct cell_s cell_t;
typedef struct camp_s camp_t;
typedef struct darray_s darray_t;
typedef struct player_s player_t;
typedef struct camp_allocator_s camp_allocator_t;

/**
 * \struct game_state_t
 * \brief État interne du jeu
 *
 * Cet objet maintient l'état interne du jeu, comme la grille,
 * les équipes et autres.
 */
typedef struct game_state_s {
	cell_t *grid;						/*!< Grille de jeu de width * height éléments */
	size_t width;							/*!< Largeur de la grille */
	size_t height;							/*!< Hauteur de la grille */
	int cheat;							/*!< Indique si le mode rapide est activé. Correspond à l'id d'un seul bateau à placer si non négatif */
	unsigned char alloc_id;						/*!< L'ID du dernier bateau placé, utilisé pour donner un ID unique à tout les bateaux du plateau */
	camp_allocator_t *camp_allocator;	/*!< Pointeur vers un objet qui décrit la logique d'allocation des équipes et partition de la grille */
	darray_t *camps;					/*!< Tableau des équipes du jeu */
	camp_t *winning;					/*!< Pointe vers une équipe vivante, garantie d'être l'équipe victorieuse en fin de partie */
} game_state_t;

typedef struct {
	int width;
	int height;
} option_t;

/// \brief Créé un nouvel objet \ref game_state_t vide
game_state_t *newGame(option_t);

/// \brief Effectue une action d'un joueur dans le jeu
result_t doAction(game_state_t *self, player_t *player, point_t point);

/// \brief Vérifie si le jeu est fini
int turnEndUpdate(game_state_t *game);

/// \brief Détermine si un point est dans un rectangle
int isPointInsideRect(point_t point, point_t rect[2]);

/// \brief Restaure le terminal et met fin au jeu
void interruptHandler(int s);

/// \brief Donne un pointeur vers une cellule selon ses coordonnées
cell_t *getCell(game_state_t *game, point_t point);

/// \brief Donne les coordonnées d'une cellule selon son pointeur
point_t getCoordinates(game_state_t *game, cell_t *cell);

#endif /* GAME_STATE_H */
