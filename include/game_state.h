/**
 * \file game_state.h
 * \brief Définition de l'état du jeu et déclaration des fonctions manipulant cet état
 */
#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "cell.h"
#include "result.h"
#include "camp.h"
#include "darray.h"
#include "camp_allocator.h"

/**
 * \struct game_state_t
 * \brief État interne du jeu
 *
 * Cet objet maintient l'état interne du jeu, comme la grille,
 * les équipes et autres.
 */
typedef struct game_state_s {
	cell_t *grid;						/*!< Grille de jeu de width * height éléments */
	int width;							/*!< Largeur de la grille */
	int height;							/*!< Hauteur de la grille */
	int cheat;							/*!< Indique si le mode rapide est activé. Correspond à l'id d'un seul bateau à placer si non négatif */
	int alloc_id;						/*!< L'ID du dernier bateau placé, utilisé pour donner un ID unique à tout les bateaux du plateau */
	camp_allocator_t *camp_allocator;	/*!< Pointeur vers un objet qui décrit la logique d'allocation des équipes et partition de la grille */
	darray_t *camps;					/*!< Tableau des équipes du jeu */
	camp_t *winning;					/*!< Pointe vers une équipe vivante, garantie d'être l'équipe victorieuse en fin de partie */
} game_state_t;

/// \brief Créé un nouvel objet \ref game_state_t vide
game_state_t *newGame();

/// \brief Effectue une action d'un joueur dans le jeu
result_t doAction(game_state_t *self, player_t *player, point_t coord);

/// \brief Vérifie si le jeu est fini
int turnEndUpdate(game_state_t *game);

/// \brief Détermine si un point est dans un rectangle
int isPointInsideRect(point_t p, point_t rect[2]);

/// \brief Restaure le terminal et met fin au jeu
void interruptHandler(int s);

/// \brief Donne un pointeur vers une cellule selon ses coordonnées
cell_t *getCell(game_state_t *game, point_t co);

/// \brief Donne les coordonnées d'une cellule selon son pointeur
point_t getCoordinates(game_state_t *game, cell_t *c);

#endif /* GAME_STATE_H */
