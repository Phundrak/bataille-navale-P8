#include <signal.h>
#include <stdio.h>
#include <curses.h>
#include <string.h>
#include <stdlib.h>

/**
 * \brief Restaure l'état du terminal et quitte le jeu.
 * \param s Le signal qui a causé l'appel de la fonction, ignoré si appelé manuellement
 */
void interruptHandler(int s) {
	(void)s;
	resetty();
	reset_shell_mode();
	clear();
	echo();
	timeout(0);
	curs_set(1);
	endwin();
	exit(127);
}
