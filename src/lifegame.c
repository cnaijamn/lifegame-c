/*-
 * Copyright (c) 2014 Chez Naijamn
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer
 *    in this position and unchanged.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR(S) ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR(S) BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <curses.h>
#include <err.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define CELL_BORN_CHAR	'.'
#define CELL_ALIVE_CHAR	'*'
#define CELL_DEAD_CHAR	' '

#define CYCLES		(1000)
#define START_NUM_RATE	(0.3)
#define DELAY		(50000)

enum cell_state {
	CELL_BORN = 0,
	CELL_ALIVE,
	CELL_DEAD,
};

static int alive_num;
static int board_width;
static int board_height;

typedef struct {
	enum cell_state state;
	enum cell_state next;
} cell_t;

static cell_t *cells;

static void
usage(void)
{
	(void)fprintf(stderr, "Usage: lifegame [options]\n"
	    "Options\n"
	    "  -n n         Number of cycles.\n"
	    "               0 <= n <= 10000 (default is %d).\n"
	    "  -r rate      Rate of first cells number.\n"
	    "               0.0 <= r <= 1.0 (default is %.1f).\n"
	    "  -h           Display this message.\n",
	    CYCLES, START_NUM_RATE
	);
	exit(1);
}

static enum cell_state
cell_init_status(float rate)
{
	return (arc4random_uniform(10000) < rate * 10000 ?
		    CELL_BORN : CELL_DEAD);
}

static void
print_cell(cell_t *cell, int i, int j)
{
	(void)mvaddch(i, j, cell->state == CELL_BORN ?
		    CELL_BORN_CHAR :
		    (cell->state == CELL_ALIVE ?
		        CELL_ALIVE_CHAR :
		        CELL_DEAD_CHAR));
}

static void
init_board(float rate)
{
	int i, j;
	enum cell_state s;

	alive_num = 0;
	for (i = 0; i < board_height; i++) {
		for (j = 0; j < board_width; j++) {
			s = cell_init_status(rate);
			if (s != CELL_DEAD)
				++alive_num;
			cells[i * board_width + j].state = s;
		}
	}
}

static int
count_no_dead_around(int i, int j)
{
	int ii, jj, c;

	c = 0;
	for (ii = i - 1; ii <= i + 1; ii++)
		for (jj = j - 1; jj <= j + 1; jj++)
			if (ii >= 0 && ii < board_height &&
			    jj >= 0 && jj < board_width &&
			    !(ii == i && jj == j) &&
			    cells[ii * board_width + jj].state != CELL_DEAD)
				++c;

	return (c);
}

static void
update_board(void)
{
	int i, j, dead;
	cell_t *c;
	enum cell_state s;

	for (i = 0; i < board_height; i++)
		for (j = 0; j < board_width; j++) {
			s = cells[i * board_width + j].state;
			dead = count_no_dead_around(i, j);

			if (dead == 3 && s == CELL_DEAD)
				s = CELL_BORN;
			else if ((dead == 2 || dead == 3) && s != CELL_DEAD)
				s = CELL_ALIVE;
			else
				s = CELL_DEAD;

			cells[i * board_width + j].next = s;
		}

	alive_num = 0;
	for (i = 0; i < board_height; i++)
		for (j = 0; j < board_width; j++) {
			c = &cells[i * board_width + j];
			c->state = c->next;
			if (c->state != CELL_DEAD)
				++alive_num;
		}
}

static void
print_board(void)
{
	int i, j;

	for (i = 0; i < board_height; i++)
		for (j = 0; j < board_width; j++)
			print_cell(&cells[i * board_width + j], i, j);
}

static void
print_summary(int i, int n, int alive_num)
{
	(void)mvprintw(board_height + 1, 0, "GENERATION: %d/%d ALIVE: %d", i, n,
	    alive_num);
}

static void
init_screen(void)
{
	if (!initscr()) {
		(void)fprintf(stderr, "Error: initscr failure\n");
		exit(EXIT_FAILURE);
	}
	if (LINES < 3) {
		endwin();
		(void)fprintf(stderr, "Error: terminal size is too small\n");
		exit(EXIT_FAILURE);
	}

	board_width  = COLS;
	board_height = LINES - 2;
}

static void
end_screen(void)
{
	(void)endwin();
}

int
main(int argc, char *argv[])
{
	int c, n, i;
	float r;

	(void)setlocale(LC_ALL, "");

	n = CYCLES;
	r = START_NUM_RATE;

	while ((c = getopt(argc, argv, "n:r:h")) != -1)
		switch (c) {
		case 'n':
			n = atoi(optarg);
			if (n < 0 || n > 10000)
				errx(1, "Illegal n option value.");
			break;
		case 'r':
			r = atof(optarg);
			if (r < 0.0 || r > 1.0)
				errx(1, "Illegal r option value.");
			break;
		case 'h':
		case '?':
		default:
			usage();
		}

	init_screen();
	cells = malloc(sizeof(cell_t) * board_width * board_height);
	init_board(r);
	print_board();
	print_summary(0, n, alive_num);

	for (i = 0; i < n; i++) {
		(void)usleep(DELAY);
		update_board();
		print_board();
		print_summary(i + 1, n, alive_num);
		(void)refresh();
	}

	(void)getch();

	end_screen();
	free(cells);
	return (0);
}
