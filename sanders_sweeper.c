#include "sanders_sweeper.h"
#include "alloc.h"
#include "sandersboard.h"
#include "syscall.h"
#include "sand_rand.h"
#include "video.h"
#include "halt.h"

extern int init_vga(int blah);

char *minefield;
int minefield_x;
int minefield_y;
int mines;

int player_pos_x;
int player_pos_y;

unsigned char player_prev;

extern unsigned char * vidmem;

void draw_cell(int x, int y, char c) {
	int cell_index = x + y * 320;
	switch(c) {
		case '?':
			*(vidmem + cell_index) = 4;
			break;
		case '0':
			*(vidmem + cell_index) = 0;
			break;
		case '1':
			*(vidmem + cell_index) = 18;
			break;
		case '2':
			*(vidmem + cell_index) = 28;
			break;
		case '3':
			*(vidmem + cell_index) = 23;
			break;
		case '4':
			*(vidmem + cell_index) = 10;
			break;
		case '5':
			*(vidmem + cell_index) = 5;
			break;
		case '6':
			*(vidmem + cell_index) = 6;
			break;
		case '7':
			*(vidmem + cell_index) = 7;
			break;
		case '8':
			*(vidmem + cell_index) = 8;
			break;
	}
	player_prev = *(vidmem + cell_index);
}

char get_mine(int x, int y) {
	return *(minefield + x + y * minefield_x);
}

char get_mine_adj(int x, int y) {
	int i, j;
	int adj_return = 0;
	for (i = x-1; i < x+2; i++) {
		if (i >= 0 && i < minefield_x) {
			for (j = y-1; j < y+2; j++) {
				if (j >= 0 && j < minefield_y) {
					if (get_mine(i, j) == SSWEEPER_MINE) {
						adj_return++;
					}
				}
			}
		}
	}
	return adj_return;
}

void move_player(int x, int y) {
	if (x != 0) {
		x = player_pos_x + x;
		if (x >= 0 && x < minefield_x) {
			*(vidmem + player_pos_x + player_pos_y * 320) = player_prev;
			player_pos_x = x;
			player_prev = *(vidmem + player_pos_x + player_pos_y * 320);
			*(vidmem + player_pos_x + player_pos_y * 320) = 10;
		}
	}
	if (y != 0) {
		y = player_pos_y + y;
		if (y >= 0 && y < minefield_y) {
			*(vidmem + player_pos_x + player_pos_y * 320) = player_prev;
			player_pos_y = y;
			player_prev = *(vidmem + player_pos_x + player_pos_y * 320);
			*(vidmem + player_pos_x + player_pos_y * 320) = 10;
		}
	}
}

void ssweeper_reveal(int x, int y) {
	int i, j;
	char spot = get_mine(x, y);
	if (spot == ' ') {
		return;
	}
	if (spot == SSWEEPER_MINE) {
		sanders_sweeper_exit();
		return;
	}
	draw_cell(x, y, spot);
	*(minefield + x + y * minefield_x) = ' ';
	if (spot == '0') {
		for (i = x-1; i < x+2; i++) {
			if (i >= 0 && i < minefield_x) {
				for (j = y-1; j < y+2; j++) {
					if (j >= 0 && j < minefield_y) {
						ssweeper_reveal(i, j);
					}
				}
			}
		}
	}
}

int sanders_sweeper(int argc, char *argv[]) {
	int i, j;
	init_vga(0);

	set_keyboard_handler(&sanders_sweeper_keyboard_handler);

	minefield_x = SSWEEPER_DEFAULT_WIDTH; 
	minefield_y = SSWEEPER_DEFAULT_WIDTH;
	mines = SSWEEPER_DEFAULT_MINES;

	minefield = mm_alloc(minefield_x * minefield_y);

	for (j = 0; j < minefield_y; j++) {
		for (i = 0; i < minefield_x; i++) {
			*(minefield + i + j * minefield_x) = ' ';
		}
	}

	int mine_interval = (minefield_x * minefield_y) / mines;

	for (i = 0; i < mines; i++) {
		int rand_offset = sand_rand() % mine_interval;
		*(minefield + i * mine_interval + rand_offset) = SSWEEPER_MINE;
	}

	for (j = 0; j < minefield_y; j++) {
		for (i = 0; i < minefield_x; i++) {
			if (get_mine(i, j) != SSWEEPER_MINE) {
				*(minefield + i + j * minefield_x) = '0' + get_mine_adj(i, j);
			}
			draw_cell(i, j, '?');
		}
	}

	player_pos_x = 0;
	player_pos_y = 0;

	player_prev = *(vidmem);


	return 0;
}

void sanders_sweeper_keyboard_handler(char keycode) {
	if (keycode <= 0) {
		return;
	}

	if (key_status[CTRL_KEY_CODE] == 1) {
  		if (keyboard_map[(unsigned char) keycode] == 'c') {
  			sanders_sweeper_exit();
    	}
    	if (keyboard_map[(unsigned char) keycode] == 'z') {
  			sanders_sweeper_exit();
    	}
    	return;
    }

    switch(keycode) {
    	case UP_KEY_CODE:
    		move_player(0, -1);
    		return;
    	case DOWN_KEY_CODE:
    		move_player(0, 1);
    		return;
    	case LEFT_KEY_CODE:
    		move_player(-1, 0);
    		return;
    	case RIGHT_KEY_CODE:
    		move_player(1, 0);
    		return;
    }

    int ascii_key = keyboard_map[(unsigned char) keycode];

    switch(ascii_key) {
    	case ' ':
    		ssweeper_reveal(player_pos_x, player_pos_y);
			player_prev = *(vidmem + player_pos_x + player_pos_y * 320);
			return;
    }
}

extern void video_mode(void);
int sanders_sweeper_exit() {
	mm_free(minefield);

	video_mode();
	halt();
	return 0;
}

