/**
 * \file darray.c
 * \brief Implémentation des fonctions pour le type \ref darray_t
 */

#include <darray.h>
#include <malloc.h>
#include <string.h>

#define _GNU_SOURCE

/// Constante pour les erreurs liées à \ref darray_t
#define PTR_ERROR 2

/**
 * `darrayNew` permet de créer un nouvel objet de type \ref darray_t ne contenant aucun élément. Le seul paramètre,
 * `element_size`, est utilisé afin de connaître l’espace mémoire à allouer à chacun des éléments dans le tableau.
 * Cela implique qu’un objet \ref darray_t ne peut contenir que des éléments du même type.
 * \param element_size Taille des objets stockés
 * \return Pointeur sur le nouvel objet \ref darray_t
 */
darray_t *darrayNew(size_t element_size) {
	darray_t *ret;
	ret = (darray_t*)malloc(sizeof(darray_t));
	ret->begin = NULL;
	ret->end = ret->begin;
	ret->element_size = element_size;
	ret->capacity = 0;
	return ret;
}

/**
 * `darrayInsert` insère l’élément `elem` avant l'élément pointé par `pos` dans l’objet \ref darray_t. Cela décalera tous
 * les éléments stockés dans \a `self` pen d’un cran vers la fin du tableau et insérera à l’endroit pointé le nouvel élément.
 * Cette fonction modifie les membres `begin` et `end` et potentiellement `capacity` de `self`.
 * \param self Objet \ref darray_t dans lequel on souhaite insérer un nouvel élément
 * \param pos Position à laquelle on souhaite insérer un nouvel élément
 * \param elem Élément que l’on souhaite insérer
 */
void darrayInsert(darray_t *self, void *pos, void *elem) {
	void *new_array;
	char *itr;
	int pos_aux;
	size_t size;
	pos_aux = (char*)pos - (char*)self->begin;
	if(darraySize(self) >= self->capacity) {
		size = darraySize(self);
		new_array = realloc(self->begin, (darraySize(self) + 1) * self->element_size);
		if (!new_array) {
			fprintf(stderr, "Failed memory reallocation at %s:%d\nAborting...", __FILE__,
						 __LINE__ - 2);
			exit(PTR_ERROR);
		}
		self->begin = new_array;
		++self->capacity;
		self->end = (char*)self->begin + size * self->element_size;
	}

	itr = (char*)self->begin + pos_aux;
	memmove(itr + self->element_size, itr, (char*)self->end - (char*)itr);
	memcpy(itr, elem, self->element_size);
	self->end = (char*)self->end + self->element_size;
}

/**
 * `darrayErase` supprime l’élément de objet \ref darray_t `self` pointé par `pos`. Cela décalera tous les éléments
 * suivants dans le tableau d’un cran vers le début du tableau de manière à ce qu’il n’y aie pas d’élément vide entre
 * les membres `begin` et `end` de `self`. Par ailleurs, le membre `end` de `self` se retrouve modifié par la fonction.
 * \param self Objet \ref `darray_t` dont on souhaite supprimer un élément
 * \param pos Élément de `self` que l’on souhaite supprimer
 */
void darrayErase(darray_t *self, void *pos) {
	memmove(pos, (char *)pos + self->element_size,
					((char *)self->end - self->element_size) - (char *)pos);
	self->end = (char*)self->end - self->element_size;
}

/**
 * `darrayPushBack` ajoute un nouvel élément `elem` à l’objet `self` à la fin du tableau de ce dernier. Cette
 * fonction modifie le membre `end` de `self`.
 * \param self Objet \ref darray_t à la fin duquel on souhaite ajouter un nouvel élément
 * \param elem Élément que l’on souhaite ajouter à la fin de `self`
 */
void darrayPushBack(darray_t *self, void *elem) {
	darrayInsert(self, self->end, elem);
}

/**
 * `darrayPopBack` permet de supprimer le dernier élément de l’objet \ref darray_t passé en argument. Cette fonction
 * modifie le membre `end` de ce dernier objet.
 * \param self Objet dont on souhaite supprimer le dernier élément
 */
void darrayPopBack(darray_t *self) {
	darrayErase(self, (char*)self->end - self->element_size);
}

/**
 * `darrayDelete` supprime tous les éléments contenus par l’objet \ref darray_t passé en argument avant de libérer la
 * mémoire occupée par l’objet lui-même. L’objet passé en argument ne sera plus utilisable après utilisation de cette
 * fonction.
 * \param self Objet \ref darray_t à supprimer
 */
void darrayDelete(darray_t *self) {
	free(self->begin);
	free(self);
}

/**
 * `darraySize` renvoie le nombre d’éléments contenu dans le \ref darray_t `self` passé en arguments. Cette fonction
 * ne modifie pas l’élément passé en argument.
 * \param self Objet \ref darray_t dont on souhaite connaître le nombre d’éléments
 * \return Nombre d’éléments contenus dans `self`
 */
size_t darraySize(darray_t *self) {
	return ((char*)self->end - (char*)self->begin) / self->element_size;
}

/**
 * `darrayGet` permet de récupérer un élément d’un objet \ref darray_t grâce à son index dans le tableau de l’objet
 * `self`. Si l’index est trop grand, alors le pointeur `NULL` sera renvoyé, sinon un pointeur de type `void*`
 * pointant sur l’élément correspondant sera renvoyé. Cette fonction ne modifie pas l’objet `self`.
 * \param self Objet \ref darray_t duquel on souhaite obtenir un pointeur sur l’élément à l’index `idx`
 * \param idx Index de l’élément que l’on souhaite récupérer
 * \return Pointeur de type `void*` pointant sur l’élément si l’index est valide, sur NULL sinon.
 */
void *darrayGet(darray_t *self, size_t idx) {
	void *itr;
	itr = (char*)self->begin + idx * self->element_size;
	return itr;
}
