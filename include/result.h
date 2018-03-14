/**
 * \file result.h
 * \brief Définition d'un type représentant le résultat d'une action
 */

#ifndef RESULT_H
#define RESULT_H

/// \enum result_t Représente l'état d'une action
typedef enum result_e {
	REDO = -1,	/*!< Le joueur doit faire un autre choix */
	MISS,		/*!< Le joueur a manqué son coup */
	HIT,		/*!< Le joueur a touché un fragment de bateau */
	SUNK		/*!< Le joueur a coulé un bateau */
} result_t;

#endif /* RESULT_H */
