Bataille navale
===============

Specifications
--------------

Tableau de 17 * 17

Tailles des bateaux:
-----------------
                 
                 
                 
                 
                 
                 
                 
                 
                 
                 
                 
                 
                 
                 
                 
                 
                 
-----------------

```
xx
xx

xxxx
xxxx

xx

xxx

xxxx

xxxxx

xxxx
   x
   x
```

Deux joueurs humains.

Tour par tour avec progression simultanée
ou
Tour par tour classique

Algorithme
----------

Initialisation:
	- Les joueurs déterminent leur camps
	- Les joueurs placent les bateaux, superposition interdites
Boucle de jeu:
	- Pour chaque camps:
	  - Un utilisateur est déterminé
	  - L'utilisateur désigne une case
	  - Touché/Raté avec les coordonnées sont annoncés :
		  - soit à l'utilisateur et au posesseur de la case ciblé
		  - soit à tout le monde
	- Si un utilisateur n'a plus de bateaux, il ne peut plus etre considéré pour faire un choix.
	- Si un camp n'a plus d'utilisateur, le camp est considéré mort.
	- Si il ne reste qu'un camp vivant, terminer.
Fin du jeu:
	- Annoncer la victoire pour le camp survivant.
	- Quitter.
