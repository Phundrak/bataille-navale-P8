/**
 * \file dumb_player.c
 * \brief Implémentation des fonctions pour un bot de jeu
 */

#include <player.h>
#include <game_state.h>

extern void blitToGrid(piece_t *p, point_t pos, game_state_t *game, unsigned char id);

/**
 * \struct dumb_player_t
 * \brief Définition d'un bot
 */
typedef struct {
	player_t base; /*!< Instance parente */
	point_t last; /*!< Position du dernier point testé*/
} dumb_player_t;

/**
 * `dumbPlayerAction` gère un tour d'un bot dans le jeu `game_state`
 * \param player Pointeur vers l'instance parente du joueur
 * \param game_state L'état du jeu
 * \return La coordonnée que le bot souhaite tester.
 */
point_t dumbPlayerAction(player_t *player, game_state_t *game_state) {
	dumb_player_t *self = (void*)player;
	++self->last.x;
	if (self->last.x >= game_state->width) {
		self->last.x -= game_state->width;
		++self->last.y;
	}
	return self->last;
}

/**
 * `localToGlobal` convertit des coordonnées locales dans le repère
 * d'un joueur en coordonnées globales dans le plateau de jeu.
 * \param self Joueur actif
 * \param point Coordonnées locales de \p self
 * \return Équivalent global de \p point
 */
point_t localToGlobal(player_t *self, point_t point) {
	return (point_t) {
		point.x + self->owned_rect[0].x,
		point.y + self->owned_rect[0].y
	};
}

/**
 * `dumbPlayerSetBoats` gère le placement des bateaux d'un bot dans le jeu `gs`
 * \param self Pointeur vers l'instance parente du joueur
 * \param gs L'état du jeu
 */
void dumbPlayerSetBoats(player_t *self, game_state_t *gs) {
	int all = 1;
	for (int outer_loop = 0; outer_loop < 3; ++outer_loop)
		for (int inner_loop = 0; inner_loop < 3; ++inner_loop) {
			point_t pos = localToGlobal(self, (point_t){(size_t) (outer_loop * 5), (size_t) (inner_loop * 5)});
			blitToGrid(&Pieces[all - 1], pos, gs, (unsigned char) all);
			all++;
			if (all > NBBOATS) {
				self->n_boats = all - 1;
				return;
			}
		}
}

/**
 * `newDumbPlayer` crée une instance d'un bot qui place automatiquement
 * ses bateaux et choisis toutes les cases dans l'ordre.
 * \return Pointeur vers la nouvelle instance
 */
player_t *newDumbPlayer() {
	dumb_player_t *ret = calloc(1, sizeof(*ret));
	ret->last = (point_t) {-1, 0};
	ret->base.get_action = dumbPlayerAction;
	ret->base.setup_boats = dumbPlayerSetBoats;
	return &ret->base;
}
