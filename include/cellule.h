#ifndef CELLULE_H
#define CELLULE_H

typedef struct {
	union {
		struct {
			int has_boat : 1;
			int has_exploded : 1;
		};
		unsigned char state;
	};
} cell_t;

#endif /* CELLULE_H */