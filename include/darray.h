/**
 * \file darray.h
 * \brief Implémentation de \ref darray_t et déclaration des fonctions pour ce type
 */

#ifndef DARRAY_H
#define DARRAY_H

#include <stdlib.h>

/**
 * \struct darray_t
 * \brief Tableau dynamique
 *
 * Les objets `darray_t` offrent la possibilité d’avoir des tableaux à taille variable en C, similairement aux objets
 * `vector` en C++.
 */
typedef struct {
	void *begin; /*!< Pointeur sur le premier élément du tableau */
	void *end; /*!< Pointeur sur l’élément situé immédiatement après le dernier élément du tableau */
	size_t element_size; /*!< Taille des éléments stockés dans le tableau */
	size_t capacity; /*!< Capacité maximale du tableau actuel (non destinée à l’utilisateur) */
} darray_t;

/// \brief Créé un nouvel objet \ref darray_t vide
darray_t *darrayNew(size_t element_size);

/// \brief Insère un élément à l’endroit pointé dans un \ref darray_t
void  darrayInsert(darray_t *self, void *pos, void *elem);

/// \brief Supprime l’élément pointé dans l’objet \ref darray_t
void  darrayErase(darray_t *self, void *pos);

/// \brief Retourne l’élément du \ref darray_t au idx-ème index
void *darrayGet(darray_t *self, size_t idx);

/// \brief Insère un élément à la fin de l’élément \ref darray_t
void  darrayPushBack(darray_t *self, void *elem);

/// \brief Supprime le dernier élément de l’élément \ref darray_t
void  darrayPopBack(darray_t *self);

/// \brief Détruit l’élément \ref darray_t
void  darrayDelete(darray_t *self);

size_t darraySize(darray_t *self);

/// \brief Renvoie la taille de l’élément \ref darray_t
size_t darrayElemSize(darray_t *self);

#endif /* DARRAY_H */
