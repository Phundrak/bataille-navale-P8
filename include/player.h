/**
 * \file player.h
 * \brief Définition du joueur de base et déclaration des fonctions manipulant un joueur
 */

#ifndef PLAYER_H
#define PLAYER_H

#include "point.h"

#define NBBOATS 7

struct game_state_s;
typedef struct game_state_s game_state_t;

/**
 * \struct player_t
 * \brief Objet de base pour tout les type de joueurs
 */
typedef struct player_s {
	point_t (*get_action)(struct player_s *, game_state_t *);	/*!< Pointeur vers une fonction qui détermine la décision d'un joueur */
	void (*setup_boats)(struct player_s *, game_state_t *);		/*!< Pointeur vers une fonction qui place les bateaux du joueur sur sa zone allouée */
	char *name;													/*!< Nom du joueur */
	point_t owned_rect[2];										/*!< Zone de la grille de jeu allouée au joueur */
	int n_boats;												/*!< Nombre de bateaux restant pour ce joueur */
} player_t;

/// \brief Instancie un joueur local
player_t *newLocalPlayer();

/// \brief Instancie un joueur naif
player_t *newDumbPlayer();

/**
 * \enum color_t
 * \brief Couleur affichable dans un terminal
 */
typedef enum {
	BLACK = 1,
	RED,
	YELLOW = RED + 2,
	BLUE,
	MAGENTA,
	CYAN,
	WHITE,
} color_t;

/// \brief Structure représentant une pièce
typedef struct {
	char width, height;
	char *cells;
} piece_t;

extern piece_t Pieces[NBBOATS];

/// \brief Génère un tableau de couleur associé à un jeu du point de vue d'un joueur filtre
color_t *stateToView(game_state_t *game, player_t *filter);

/// \brief Affiche un tableau de couleur associé à un jeu
void printColorArray(game_state_t *game, const color_t *arr);

/// \brief Fait tourner une pièce 90° dans le sens des aiguilles d'une montre `rotation_nb` fois.
void rotate(piece_t *);

#endif /* PLAYER_H */
