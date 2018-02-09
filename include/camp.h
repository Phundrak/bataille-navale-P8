/**
 * \file camp.h
 * \brief Définition du type représentant une équipe
 */

#ifndef CAMP_H
#define CAMP_H

#include "player.h"
#include "darray.h"

/**
 * \struct camp_t
 * \brief Représente une équipe
 */
typedef struct camp_s {
	darray_t *players;	/*!< Tableau de pointeur vers les joueurs de l'équipe */
	int is_alive;		/*!< Non nul si l'équipe peut encore jouer */
	char *team_string;	/*!< Représentation textuelle des joueurs de l'équipe */
} camp_t;

/// \brief Créé un nouveau camp
camp_t *newCamp();

/// \brief Renvoie une réprésentation des membres de l'équipe sous forme de chaine
char *campTeamString(camp_t *);

/// \brief Libère les ressources associées avec un camp
void deleteCamp(camp_t *);

#endif /* CAMP_H */
