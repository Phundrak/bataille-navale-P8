/**
 * \file camp_allocator.h
 * \brief Logique d'allocation de joueur
 */

#ifndef CAMP_ALLOCATOR_H
#define CAMP_ALLOCATOR_H

typedef struct game_state_s game_state_t;
typedef struct player_s player_t;

/**
 * \struct camp_allocator_t
 * \brief Objet de base encapsulant la logique d'allocation d'espace
 * et d'équipe aux joueurs
 */
typedef struct camp_allocator_s {
	void (*put_in_camp)(struct camp_allocator_s *, game_state_t *, player_t *); ///< Pointeur vers une fonction qui gère l'allocation, voir \ref `singleAllocateCamp`
} camp_allocator_t;

/**
 * \brief renvoie un allocateur qui place tout les joueurs dans la 
 * même équipe et leur affecte une zone de 17 * 17
 */
camp_allocator_t *newSingleAllocator();

#endif /* CAMP_ALLOCATOR_H */
