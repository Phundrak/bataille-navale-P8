#include <darray.h>
#include <game_state.h>
#include <stdio.h>

int main(int argc, char *argv[argc]) {
  (void)argv;
  (void)argc;

  /* game_state_t *game = newGame(); */

  // Cas d'un jeu deux joueurs local
  /* { */
  /*   // J1 */
  /*   game->camp_allocator->put_in_camp(game->camp_allocator, game, */
  /*                                     newLocalPlayer()); */
  /*   // J2 */
  /*   game->camp_allocator->put_in_camp(game->camp_allocator, game, */
  /*                                     newLocalPlayer()); */
  /* } */

    /* test darray_t
     * ***********************************************************/


  char *str1 = "Hello World!", *str2 = "How are you?",
       *str3 = "This is a wonderful morning!";
  darray_t *char_array_test;
  char_array_test = darrayNew(sizeof(char *));
  darrayPushBack(char_array_test, &str1);
  darrayPushBack(char_array_test, &str2);
  darrayPushBack(char_array_test, &str3);
  printf("String: %s\n", *(char **)darrayGet(char_array_test, 1));
  size_t i;
  for(i = 0; i < darraySize(char_array_test); ++i) {
    printf("String: %s\n", *(char **)darrayGet(char_array_test, i));
  }
  darrayErase(char_array_test, darrayGet(char_array_test, 1));
  for(i = 0; i < darraySize(char_array_test); ++i) {
    printf("String: %s\n", *(char **)darrayGet(char_array_test, i));
  }
  darrayDelete(char_array_test);

  /* do { */
  /*   for (unsigned i = 0; i < darraySize(game->camps); ++i) { */
  /*     camp_t *camp = *(camp_t **)darrayGet(game->camps, i); */
  /*     player_t *players = darrayGet(camp->players, 0); */
  /*     size_t n = darraySize(camp->players); */
  /*     while (n--) { */
  /*       result_t r; */
  /*       point_t coordinates; */
  /*       do { */
  /*         coordinates = players->get_action(players, game); */
  /*       } while ((r = doAction(game, players, coordinates)) == REDO); */
  /*       ++players; */
  /*     } */
  /*   } */
  /* } while (turnEndUpdate(game)); */
  return 0;
}
