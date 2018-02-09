/**
 * \file camp_allocator.c
 * \brief Implémentation des fonctions pour la famille de type \ref camp_allocator_t
 */

#include <camp.h>
#include <camp_allocator.h>
#include <game_state.h>
#include <stdlib.h>

/**
 * \struct single_camp_allocator_t
 * \brief Objet héritant de \ref camp_allocator_t.
 * Encapsule la logique d'un jeu en mode "Chacun pour soi" ou chaque joueur est seul
 * dans son équipe.
 */
typedef struct {
	camp_allocator_t base;	/*!< Instance parente */
	int alloc_number;		/*!< Numéro d'allocation permettant de garder trace de combien de joueur ont été alloué par cette instance */
} single_camp_allocator_t;

/**
 * `singleAllocateCamp` affecte à un joueur une zone de la grille de jeu
 * de taille 17 * 17 et le place seul dans une équipe. Le joueur doit ensuite placer
 * ses bateaux dans la zone qui lui est alloué
 * \param base Pointeur vers l'instance parente
 * \param game Pointeur vers l'état de jeu
 * \param player Pointeur vers le joueur à placer
 */
static void singleAllocateCamp(camp_allocator_t *base, game_state_t *game, player_t *p) {
	single_camp_allocator_t *self = (void*) base;
	// 1 camp par joueur
	// 1 joueur par camp
	camp_t *camp = newCamp();
	darrayPushBack(camp->players, &p);
	darrayPushBack(game->camps, &camp);

	int bidx = 17 * self->alloc_number;
	p->owned_rect[0] = (point_t) {
			bidx % game->width,
			bidx / game->width * 17
	};

	p->owned_rect[1] = (point_t) {
		p->owned_rect[0].x + 17,
		p->owned_rect[0].y + 17,
	};

	++self->alloc_number;
	p->setup_boats(p, game);
}

/**
 * `newSingleAllocator` permet de créer un nouvel objet de type \ref camp_allocator_t
 * qui encapsule la logique d'un jeu en mode "Chacun pour soi" où chaque joueur est seul
 * dans son équipe.
 * \return Pointeur sur le nouvel objet \ref camp_allocator_t
 */
camp_allocator_t *newSingleAllocator() {
	single_camp_allocator_t *ret = calloc(1, sizeof(*ret));
	ret->base.put_in_camp = singleAllocateCamp;
	return &ret->base;
}
