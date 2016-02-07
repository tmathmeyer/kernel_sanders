#include "sanders_sweeper.h"
#include "alloc.h"
#include "sandersboard.h"
#include "syscall.h"
#include "screentext.h"
#include "sand_rand.h"

char *minefield;
int minefield_x;
int minefield_y;
int mines;

int player_pos_x;
int player_pos_y;

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
	x = player_pos_x + x;
	y = player_pos_y + y;
	if (x >= 0 && x < minefield_x) {
		player_pos_x = x;
	}
	if (y >= 0 && y < minefield_y) {
		player_pos_y = y;
	}
	set_cursor_pos(player_pos_x + SSWEEPER_FIELD_OFFSET_X, player_pos_y + SSWEEPER_FIELD_OFFSET_Y);
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
	screentext_writecharplace(spot, x+SSWEEPER_FIELD_OFFSET_X, y+SSWEEPER_FIELD_OFFSET_Y);
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
	set_keyboard_handler(&sanders_sweeper_keyboard_handler);
	console_clear();

	minefield_x = SSWEEPER_DEFAULT_WIDTH; 
	minefield_y = SSWEEPER_DEFAULT_WIDTH;
	mines = SSWEEPER_DEFAULT_MINES;

	player_pos_x = 0;
	player_pos_y = 0;

	set_cursor_pos(player_pos_x + SSWEEPER_FIELD_OFFSET_X, player_pos_y + SSWEEPER_FIELD_OFFSET_Y);

	minefield = mm_alloc(minefield_x * minefield_y);

	int i, j;
	for (i = 0; i < minefield_x; i++) {
		for (j = 0; j < minefield_y; j++) {
			*(minefield + i + j * minefield_x) = ' ';
		}
	}

	int mine_interval = (minefield_x * minefield_y) / mines;

	for (i = 0; i < mines; i++) {
		int rand_offset = sand_rand() % mine_interval;
		*(minefield + i * mine_interval + rand_offset) = SSWEEPER_MINE;
	}

	for (i = 0; i < minefield_x; i++) {
		for (j = 0; j < minefield_y; j++) {
			if (get_mine(i, j) != SSWEEPER_MINE) {
				*(minefield + i + j * minefield_x) = '0' + get_mine_adj(i, j);
			}
			// screentext_writecharplace(get_mine(i, j), i+SSWEEPER_FIELD_OFFSET_X, j+SSWEEPER_FIELD_OFFSET_Y);
			screentext_writecharplace('?', i+SSWEEPER_FIELD_OFFSET_X, j+SSWEEPER_FIELD_OFFSET_Y);
		}
	}


	return 0;
}

void sanders_sweeper_keyboard_handler(char keycode) {
	// sanders_printf("%d", keycode);
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
    		return;
    }
}

int sanders_sweeper_exit() {
	mm_free(minefield);
	set_default_keyboard_handler();
	console_clear();
	console_print("Thanks for playing!\n");
	return 0;
}

