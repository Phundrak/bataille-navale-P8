#include <darray.h>
#include <camp.h>

camp_t *new_camp() {
	camp_t *ret = malloc(sizeof(*ret));
	ret->players = darray_new(sizeof(player_t*));
	ret->is_alive = 1;
	return ret;
}