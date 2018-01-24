### Structures de données necessaires

Tableau dynamique avec 
`new_array`
`insert`
`erase`
`size`
`push_back`
`pop_back`
`destroy`

État de jeu:

```
/* 
	Contient l'état de la grille à plat.
	Pour une paires de coordonnées (x, y)
	où y = 0 désigne la ligne du haut et x = 0 
	désigne la colonne de gauche.
	On obtient la cellule d'indice (x, y) avec l'expression
	grid[width * y + x]
*/
cellule *grid
width, height

/* Objet qui prend en parametre un joueur et lui affecte un camp */
camp_allocator
camps[]
```

cellule :

```
int,
int,
bateau_*
```

L'état indique si la cellule est occupé par un bateau ou non, et
si un canon a tirer dessus.

camp_allocator:

```
camp_t *(*get_camp)(camp_allocator *, game_state *, joueur_t);
```

camps:

```
/* Tableau terminé par pointeur null */
joueur_t (*get_players)(camp_t *);
is_alive
```

joueur_t:

```
point_t (*get_action)(joueur_t *, game_state *);
name;
is_alive;
```

point_t:

```
x, y
```

result:

```
enum result {
	REDO = -1,
	MISS,
	SUCCESS
}

```

### Fonctions

```
/* Explose une case. Si la case appartient au joueur, renvoyer -1,
	si il y a un bateau, renvoyer 1 
	si il y a pas de  bateaux, renvoyer 0 */
result do_action(game_state *, joueur_t *, point_t *)
```

```
/*
	Verifie si les joueur vivants le sont toujours
	Verifie si les camps vivants le sont toujours
	renvoie si le jeu est fini
*/
bool turn_end_update(game_state *)
```
