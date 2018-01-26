#include <camp_allocator.h>
#include <camp.h>
#include <game_state.h>
#include <stdlib.h>

typedef struct {
	camp_allocator_t base;
	int alloc_number;
} single_camp_allocator_t;

static void single_allocate_camp(camp_allocator_t *base, game_state_t *game, player_t *p) {
	single_camp_allocator_t *self = (void*) base;
	// 1 camp par joueur
	// 1 joueur par camp
	camp_t *camp = newCamp();
	darrayPushBack(camp->players, &p);
	darrayPushBack(game->camps, &p);

	// Le plateau est un carré avec des lignes potentiellement vide.
	// La largeur en fonction du nombre de joueurs est
	// int width = ceil(sqrt(nplayers)) * 17;
	// remplacer 17 par la taille d'un plateau

	// Quand un joueur est ajouté, on lui donne
	// le prochain espace de 17 * 17 disponible
	// alloué de haut en bas et de gauche à droite

	int bidx = 17 * self->alloc_number;
	// ex, j0 -> [0, 0] - [17, 17]
	// ex, j1 -> [0, 17] - [17, 34]
	p->owned_rect[0] = (point_t) {
			bidx % game->width,
			bidx / game->width * 17
	};

	p->owned_rect[1] = (point_t) {
		p->owned_rect[0].x + 17,
		p->owned_rect[0].y + 17,
	};
	
	/*
	  Dans le cas d'une partie réseau, soit l'ennemi envoie la position
	  de ses bateau à notre client, qui a potentiellement été modifié pour
	  dévoiler la position des bateau, soit l'ennemi n'envoie pas ses bateaux et
	  peut potentiellement mentir lorsque le client envoie une case
	 */
	p->setup_boats(p, game);
	/* TODO: Demander au joueur de placer ses bateaux */
}

camp_allocator_t *new_single_allocator() {
	single_camp_allocator_t *ret = calloc(1, sizeof(*ret));
	ret->base.put_in_camp = single_allocate_camp;
	return &ret->base;
}
