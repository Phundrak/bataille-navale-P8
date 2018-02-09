/**
 * \file camp.c
 * \brief Implémentation des fonctions pour le type \ref camp_t
 */

#include <camp.h>
#include <darray.h>
#include <string.h>
#include <stdio.h>

/**
 * `newCamp` permet de créer un nouvel objet de type \ref camp_t ne contenant 
 * aucun joueur.
 * \return Pointeur sur le nouvel objet \ref camp_t
 */
camp_t *newCamp() {
	camp_t *ret = calloc(1, sizeof(*ret));
	ret->players = darrayNew(sizeof(player_t*));
	ret->is_alive = 1;
	return ret;
}

/**
 * `campTeamString` renvoie une représentation sous forme de chaine
 * de caractères des membres de l'équipe. Cette fonction ne doit être
 * une seule fois après la constitution finale de l'équipe, et la mémoire
 * de la chaine est libéré lors de l'appel à \ref deleteCamp.
 * \param self Pointeur vers le camp
 */
char *campTeamString(camp_t *self) {
	unsigned n = darraySize(self->players);
	player_t *players = *(player_t **)darrayGet(self->players, 0);
	int len = 0;
	for (unsigned i = 0; i < n; ++i) {
		len += players[i].name ? strlen(players[i].name) : 0;
	}
	self->team_string = malloc(len + (2 * (len - 1)) + 1);
	char *it = self->team_string;
	for (unsigned i = 0; i < n; ++i) {
		it += sprintf(it, "%s", players[i].name);
		if (i < n - 1)
			it += sprintf(it, ", ");
	}
	return self->team_string;
}

/**
 * `deleteCamp` libère les ressources associées à une équipe
 * \param self Pointeur vers le camp
 */
void deleteCamp(camp_t *self) {
	for (unsigned j = 0; j < darraySize(self->players); ++j) {
		player_t *player = *(player_t**)darrayGet(self->players, j);
		/* TODO: call proper deallocation routine */
		free(player);
	}
	darrayDelete(self->players);
	free(self->team_string);
	free(self);
}
