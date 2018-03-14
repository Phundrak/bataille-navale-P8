/**
 * \file cell.h
 * \brief Définition d'un type représentant l'état d'une cellule de jeu
 */

#ifndef CELL_H
#define CELL_H

/**
 * \struct cell_t
 * \brief Représente l'état d'une cellule
 */
typedef struct cell_s {
	union {
		struct {
			int has_boat : 1;		/*!< 1 si la cellule contient un fragment de bateau */
			int has_exploded : 1;	/*!< 1 si la cellule a été explosée */
			int has_sunk : 1;		/*!< 1 si la cellule a été coulée */
			int marked : 1;			/*!< 1 si la cellule a été marquée */
		};
		unsigned char state;		/*!< Possède une valeur unique pour une combinaison des état possible de la cellule */
	};
	unsigned char boat_id;			/*!< 0 si vide, ou bien l'identifiant du bateau auquel le fragment appartient */
} cell_t;

#endif
