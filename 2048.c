#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <curses.h>

#define TRUE 1
#define FALSE 0

typedef char BOOL;

/* use the first 4 bits to restore which sides are empty */
/* and the last 4 bits to the highest index */
typedef unsigned char GLOBAL_STATUS;
GLOBAL_STATUS g_sts = 1;

enum option {UP, DOWN, LEFT, RIGHT, QUIT, VOID};

/* the numbers showed in the grids */
char* grid_num_str[12] = {"    ", "  2 ", "  4 ", "  8 ", " 16 ", " 32 ", " 64 ", " 128", \
	" 256", " 512", "1024", "2048"};

/* the index of the numbers in the grids (the base number is 2) */
enum grid_num_index {null = 0, idx1, idx2, idx3, idx4, idx5, idx6, idx7, idx8, idx9, idx10, \
	idx11};

/* the array to restore the numbers of the gribs */
enum grid_num_index grid[16] = {null};

int move_times = 0;

/* some numbers with a bit of magic */
/* I cannot make others understand those number */
const int start_array[] = {0, 12, 0, 3};
const int opp_start_array[] = {12, 0, 3, 0};
const int change_bit_order[] = {5, 4, 7, 6};
const int step_len_array[] = {1, 1, 4, 4};
const int stride[] = {4, -4, 1, -1};

void print_map()
{
	int i;
	printw("easy-2048\n");
	for (i = 0; i < 4; i++) {
		printw("|------|------|------|------|\n");
		printw("| %s | %s | %s | %s |\n", grid_num_str[grid[4 * i]], \
				grid_num_str[grid[4 * i + 1]], \
				grid_num_str[grid[4 * i + 2]], \
				grid_num_str[grid[4 * i + 3]]);
	}
	printw("|------|------|------|------|\n%d moves\t'q' for quit\n", move_times);
}

enum option get_input(char input_ch)
{
	switch (input_ch) {
	case 'w':
	case 'k':
		return UP;
	case 's':
	case 'j':
		return DOWN;
	case 'a':
	case 'h':
		return LEFT;
	case 'd':
	case 'l':
		return RIGHT;
	case 'q':
		return QUIT;
	default:
		return VOID;
	}
}

/* check whether the side is empty or not */
BOOL check_side(int start, int step_len)
{
	int i;
	BOOL empty_flag = FALSE;
	for (i = 0; i < 4 * step_len; i+= step_len) {
		if (*(grid + start + i) == null) {
			empty_flag = TRUE;
		}
	}
	return empty_flag;
}

/* check all the sides and update the global status */
void check_all()
{
	int i;
	for (i = 0; i < 4; i++) {
		if (check_side(start_array[i], step_len_array[i])) {
			g_sts |= (1 << (i + 4));
		} else {
			g_sts &= ~(1 << (i + 4));
		}
	}
}

/* add number in the map */
void add_num(enum option opt)
{
	enum grid_num_index idx; 
	idx = rand() % 10 == 0 ? idx2: idx1;
	int x = rand() % 4;
	for (;;) {
		if (*(grid + opp_start_array[opt] + x * step_len_array[opt]) == null) {
			*(grid + opp_start_array[opt] + x * step_len_array[opt]) = idx;		
			break;
		} else {
			x = rand() % 4;
		}
	}
	move_times++;
}

/* count the numbersand change the map according to the options */
void move_num(enum option opt)
{
	enum grid_num_index* set;
	int i, j, k;
	for (i = 0; i < 4; i++) {
		set = grid + start_array[opt] + step_len_array[opt] * i;
		for (j = 0; j < 3; j++) {
			enum grid_num_index* current = set + j * stride[opt];
			enum grid_num_index* next = current + stride[opt];
			if (*current == null) {
				for (k = 0; k + j < 3; k++) {
					if (*next != null) {
						*current = *next;
						*next = null;
						break;
					} else {
						next += stride[opt];
					}
				}
			}
			if (*current != null) {
				for (k = 0; k + j < 3; k++) {
					if (*next == *current) {
						(*current)++;
						if (*current > (g_sts & 15)) {
							g_sts++;
						}
						*next = null;
						break;
					} else if (*next == null) {
						next += stride[opt];
					} else {
						break;
					}
				}
			}
		}
	}

}

int main(void)
{
	BOOL quit_flag = FALSE;
	srand((unsigned)time(NULL));
	int x = rand() % 16;
	grid[x] = idx1;
	initscr();
	while (!quit_flag) {
		print_map();
		if ((g_sts & 15) == 11) {
			printw("WIN!\n");
			quit_flag = TRUE;
		}
		char input_ch = getch();
		enum option opt = get_input(input_ch);
		switch (opt) {
		case QUIT:
			quit_flag = TRUE;
                        break;
		case VOID:
			break;
		default:
			move_num(opt);
			check_all();
			if (g_sts & (1 << change_bit_order[opt])) {
				add_num(opt);
				break;
			}
		}
		clear();
	}
	printw("Input 'q' again for quit\n");
	for (;;) {
		if (getch() == 'q') {
			endwin();
			break;
		}
	}
	return 0;
}

