/**
 * \file point.h
 * \brief Définition d'un type représentant un point dans l'espace
 */

#ifndef POINT_H
#define POINT_H

#include <stdlib.h>

/**
 * \struct point_t
 * \brief Défini un point dans un espace 2D
 */
typedef struct {
	size_t x; /*!< Coordonnées X du point */
	size_t y; /*!< Coordonnées Y du point */
} point_t;

#endif /* POINT_H */
