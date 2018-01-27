#include <darray.h>
#include <camp.h>

camp_t *newCamp() {
	camp_t *ret = malloc(sizeof(*ret));
	ret->players = darrayNew(sizeof(player_t*));
	ret->is_alive = 1;
	return ret;
}
