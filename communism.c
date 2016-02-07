#include "communism.h"
#include "alloc.h"
#include "sandersboard.h"
#include "syscall.h"
#include "sand_rand.h"
#include "video.h"
#include "halt.h"

extern int init_vga(int blah);

extern unsigned char * vidmem;

unsigned char *grid1;
int player_pos_x1;
int player_pos_y1;
int player_x1[4];
int player_y1[4];
unsigned char player_color1;

unsigned char *grid2;
int player_pos_x2;
int player_pos_y2;
int player_x2[4];
int player_y2[4];
unsigned char player_color2;


unsigned char *grid3;
int player_pos_x3;
int player_pos_y3;
int player_x3[4];
int player_y3[4];
unsigned char player_color3;

int spin_lock;

void communism_draw_cell(int x, int y, unsigned char c) {
	int i, j;
	for (j = 1; j < CELL_SIZE-1; j++) {
		for (i = 1; i < CELL_SIZE-1; i++) {
			*(vidmem + x + i + 320* (j + y)) = c;
		}
	}
	for (i = 0; i < CELL_SIZE; i++) {
		*(vidmem + x + i + 320* y) = 2;
		*(vidmem + x + i + 320* (CELL_SIZE + y)) = 2;
		*(vidmem + x + 320* (i + y)) = 2;
		*(vidmem + x + CELL_SIZE + 320* (i + y)) = 2;
	}
}

void render_grid(int player) {
	int pos_x, pos_y;
	int *x, *y;
	unsigned char color;
	unsigned char* grid;
	if (player == 1) {
		pos_x = player_pos_x1;
		pos_y = player_pos_y1;
		x = player_x1;
		y = player_y1;
		color = player_color1;
		grid = grid1;
	} else  if (player == 2) {
		pos_x = player_pos_x2;
		pos_y = player_pos_y2;
		x = player_x2;
		y = player_y2;
		color = player_color2;
		grid = grid2;
	} else {
		pos_x = player_pos_x3;
		pos_y = player_pos_y3;
		x = player_x3;
		y = player_y3;
		color = player_color3;
		grid = grid3;
	}
	int i, j;
	for (i = 0; i < 4; i++) {
		*(grid + pos_x + x[i] + COMMUNISM_WIDTH * (pos_y + y[i])) = color;
	}
	
	for (j = 0; j < COMMUNISM_HEIGHT; j++) {
		for (i = 0; i < COMMUNISM_WIDTH; i++) {
			if (player == 1) {
				communism_draw_cell(i * CELL_SIZE, j * CELL_SIZE, *(grid + i + j * COMMUNISM_WIDTH));
			} else if (player == 2) {
				communism_draw_cell(i * CELL_SIZE + (COMMUNISM_WIDTH) * CELL_SIZE + COMMUNISM_WIDTH/2, (COMMUNISM_HEIGHT - j - 1) * CELL_SIZE, *(grid + i + j * COMMUNISM_WIDTH));
			} else {
				communism_draw_cell(j * CELL_SIZE + (COMMUNISM_WIDTH) * CELL_SIZE * 2 + COMMUNISM_WIDTH, i * CELL_SIZE, *(grid + i + j * COMMUNISM_WIDTH));
			}
		}
	}
}

void generate_piece(int player, int *pos_x, int *pos_y, unsigned char *color, int x[4], int y[4]) {
	unsigned int piece = sand_rand() % 7;
	*pos_x = 4;
	*pos_y = 0;
	switch(piece) {
		case 0: // Line
			x[0] = -1;
			y[0] = 0;
			x[1] = 0;
			y[1] = 0;
			x[2] = 1;
			y[2] = 0;
			x[3] = 2;
			y[3] = 0;
			*color = 33;
			break;
		case 1: // L
			x[0] = -1;
			y[0] = 0;
			x[1] = 0;
			y[1] = 0;
			x[2] = 1;
			y[2] = 0;
			x[3] = 1;
			y[3] = 1;
			*color = 38;
			break;
		case 2: // Backwards L
			x[0] = -1;
			y[0] = 0;
			x[1] = 0;
			y[1] = 0;
			x[2] = 1;
			y[2] = 0;
			x[3] = -1;
			y[3] = 1;
			*color = 18;
			break;
		case 3: // S
			x[0] = 0;
			y[0] = 0;
			x[1] = 1;
			y[1] = 0;
			x[2] = -1;
			y[2] = 1;
			x[3] = 0;
			y[3] = 1;
			*color = 13;
			break;
		case 4: // Z
			x[0] = -1;
			y[0] = 0;
			x[1] = 0;
			y[1] = 0;
			x[2] = 0;
			y[2] = 1;
			x[3] = 1;
			y[3] = 1;
			*color = 8;
			break;
		case 5: // T
			x[0] = -1;
			y[0] = 0;
			x[1] = 0;
			y[1] = 0;
			x[2] = 1;
			y[2] = 0;
			x[3] = 0;
			y[3] = 1;
			*color = 28;
			break;
		case 6: // Square
			x[0] = 0;
			y[0] = 0;
			x[1] = 1;
			y[1] = 0;
			x[2] = 0;
			y[2] = 1;
			x[3] = 1;
			y[3] = 1;
			*color = 23;
			break;
	}

	render_grid(player);
}

void check_grid(int player) {
	unsigned char* grid;
	if (player == 1) {
		grid = grid1;
	} else if (player == 2) {
		grid = grid2;
	} else {
		grid = grid3;
	}
	int i, j, k;
	i = COMMUNISM_HEIGHT-1;

	while(i >= 0) {
		int cleared = 1;
		for (j = 0; j < COMMUNISM_WIDTH; j++) {
			if (*(grid + j + COMMUNISM_WIDTH * i) == 0) {
				cleared = 0;
			}
		}
		if (cleared == 0) {
			i--;
		} else {
			if (spin_lock > 10000000) {
				spin_lock -= 1000000;
			}
			for (j = i; j >= 1; j--) {
				for (k = 0; k < COMMUNISM_WIDTH; k++) {
					*(grid + k + COMMUNISM_WIDTH * j) = *(grid + k + COMMUNISM_WIDTH * (j - 1));
				}
			}
			for (k = 0; k < COMMUNISM_WIDTH; k++) {
				*(grid + k) = 0;
			}
		}
	}
}

int down_piece(int player) {
	int pos_x, pos_y;
	int *x, *y;
	unsigned char color;
	unsigned char *grid;
	if (player == 1) {
		pos_x = player_pos_x1;
		pos_y = player_pos_y1;
		x = player_x1;
		y = player_y1;
		color = player_color1;
		grid = grid1;
	} else if (player == 2) {
		pos_x = player_pos_x2;
		pos_y = player_pos_y2;
		x = player_x2;
		y = player_y2;
		color = player_color2;
		grid = grid2;
	} else {
		pos_x = player_pos_x3;
		pos_y = player_pos_y3;
		x = player_x3;
		y = player_y3;
		color = player_color3;
		grid = grid3;
	}

	int i;
	int lock = 0;
	for (i = 0; i < 4; i++) {
		*(grid + pos_x + x[i] + COMMUNISM_WIDTH * (pos_y + y[i])) = 0;
	}
	for (i = 0; i < 4; i++) {
		if (pos_y + y[i] + 1 >= COMMUNISM_HEIGHT) {
			lock = 1;
			break;
		}
		if (*(grid + pos_x + x[i] + COMMUNISM_WIDTH * (pos_y + y[i] + 1)) != 0) {
			lock = 1;
			break;
		}
	}
	if (lock) {
		for (i = 0; i < 4; i++) {
			*(grid + pos_x + x[i] + COMMUNISM_WIDTH * (pos_y + y[i])) = color-2;
		}
		check_grid(player);
		if (player == 1) {
			generate_piece(player, &player_pos_x1, &player_pos_y1, &player_color1, player_x1, player_y1);
		} else if (player == 2) {
			generate_piece(player, &player_pos_x2, &player_pos_y2, &player_color2, player_x2, player_y2);
		} else {
			generate_piece(player, &player_pos_x3, &player_pos_y3, &player_color3, player_x3, player_y3);
		}
		return 1;
	} else {
		if (player == 1) {
			player_pos_y1 ++;
		} else if (player == 2) {
			player_pos_y2 ++;
		} else {
			player_pos_y3 ++;	
		}
		render_grid(player);
		return 0;
	}
}

void move_piece(int move, int player) {
	int pos_x, pos_y;
	int *x, *y;
	unsigned char *grid;
	if (player == 1) {
		pos_x = player_pos_x1;
		pos_y = player_pos_y1;
		x = player_x1;
		y = player_y1;
		grid = grid1;
	} else if (player == 2) {
		pos_x = player_pos_x2;
		pos_y = player_pos_y2;
		x = player_x2;
		y = player_y2;
		grid = grid2;
	} else {
		pos_x = player_pos_x3;
		pos_y = player_pos_y3;
		x = player_x3;
		y = player_y3;
		grid = grid3;	
	}

	int i;
	int lock = 0;
	for (i = 0; i < 4; i++) {
		*(grid + pos_x + x[i] + COMMUNISM_WIDTH * (pos_y + y[i])) = 0;
	}
	for (i = 0; i < 4; i++) {
		if (pos_x + x[i] + move >= COMMUNISM_WIDTH || pos_x + x[i] + move < 0) {
			lock = 1;
			break;
		}
		if (*(grid + pos_x + x[i] + move + COMMUNISM_WIDTH * (pos_y + y[i])) != 0) {
			lock = 1;
			break;
		}
	}
	if (lock) {
		return;
	} else {
		if (player == 1) {
			player_pos_x1 += move;
		} else if (player == 2) {
			player_pos_x2 += move;
		} else {
			player_pos_x3 += move;
		}
		render_grid(player);
	}
}

void rotate_piece(int player) {
	int pos_x, pos_y;
	int *x, *y;
	unsigned char *grid;
	if (player == 1) {
		pos_x = player_pos_x1;
		pos_y = player_pos_y1;
		x = player_x1;
		y = player_y1;
		grid = grid1;
	} else if (player == 2) {
		pos_x = player_pos_x2;
		pos_y = player_pos_y2;
		x = player_x2;
		y = player_y2;
		grid = grid2;
	} else {
		pos_x = player_pos_x3;
		pos_y = player_pos_y3;
		x = player_x3;
		y = player_y3;
		grid = grid3;
	}

	int i;
	int lock = 0;
	for (i = 0; i < 4; i++) {
		*(grid + pos_x + x[i] + COMMUNISM_WIDTH * (pos_y + y[i])) = 0;
	}
	for (i = 0; i < 4; i++) {
		if (pos_x + y[i] >= COMMUNISM_WIDTH || pos_x + y[i] < 0) {
			lock = 1;
			break;
		}
		if (pos_y - x[i] >= COMMUNISM_HEIGHT || pos_y - x[i] < 0) {
			lock = 1;
			break;
		}
		if (*(grid + pos_x + y[i] + COMMUNISM_WIDTH * (pos_y - x[i])) != 0) {
			lock = 1;
			break;
		}
	}
	if (lock) {
		return;
	} else {
		int temp;
		if (player == 1) {
			for (i = 0; i < 4; i++) {
				temp = player_x1[i];
				player_x1[i] = player_y1[i];
				player_y1[i] = -temp;
			}
		} else if (player == 2) {
			for (i = 0; i < 4; i++) {
				temp = player_x2[i];
				player_x2[i] = player_y2[i];
				player_y2[i] = -temp;
			}
		} else {
			for (i = 0; i < 4; i++) {
				temp = player_x3[i];
				player_x3[i] = player_y3[i];
				player_y3[i] = -temp;
			}
		}
		render_grid(player);
	}
}

void drop_piece(int player) {
	while(!down_piece(player)) {

	}
}


int communism(int argc, char *argv[]) {
	int i, j;
	init_vga(0);

	spin_lock = 100000000;

	grid1 = mm_alloc(COMMUNISM_WIDTH * COMMUNISM_HEIGHT);
	grid2 = mm_alloc(COMMUNISM_WIDTH * COMMUNISM_HEIGHT);
	grid3 = mm_alloc(COMMUNISM_WIDTH * COMMUNISM_HEIGHT);

	for (j = 0; j < COMMUNISM_HEIGHT; j++) {
		for (i = 0; i < COMMUNISM_WIDTH; i++) {
			*(grid1 + i + j * COMMUNISM_WIDTH) = 0;
			*(grid2 + i + j * COMMUNISM_WIDTH) = 0;
			*(grid3 + i + j * COMMUNISM_WIDTH) = 0;
		}
	}

	set_keyboard_handler(&communism_keyboard_handler);

	generate_piece(1, &player_pos_x1, &player_pos_y1, &player_color1, player_x1, player_y1);
	generate_piece(2, &player_pos_x2, &player_pos_y2, &player_color2, player_x2, player_y2);
	generate_piece(3, &player_pos_x3, &player_pos_y3, &player_color3, player_x3, player_y3);
	while (1) {
		i = 0; 
		while (i++ < spin_lock) {
		}

		down_piece(1);
		down_piece(2);
		down_piece(3);
	}

	return 0;
}

void communism_keyboard_handler(char keycode) {
	if (keycode <= 0) {
		return;
	}

	if (key_status[CTRL_KEY_CODE] == 1) {
  		if (keyboard_map[(unsigned char) keycode] == 'c') {
  			communism_exit();
    	}
    	if (keyboard_map[(unsigned char) keycode] == 'z') {
  			communism_exit();
    	}
    	return;
    }

    switch(keycode) {
    	case UP_KEY_CODE:
    		rotate_piece(3);
    		return;
    	case DOWN_KEY_CODE:
    		down_piece(3);
    		return;
    	case LEFT_KEY_CODE:
    		move_piece(-1, 3);
    		return;
    	case RIGHT_KEY_CODE:
    		move_piece(1, 3);
    		return;
    	case ENTER_KEY_CODE:
    		drop_piece(3);
    }

    int ascii_key = keyboard_map[(unsigned char) keycode];

    switch(ascii_key) {
    	case 'q':
    		drop_piece(1);
    		return;
    	case 'w':
    		rotate_piece(1);
    		return;
    	case 'a':
    		move_piece(-1, 1);
    		return;
    	case 's':
    		down_piece(1);
    		return;
    	case 'd':
    		move_piece(1, 1);
    		return;

    	case 'b':
    		drop_piece(2);
    		return;
    	case 'j':
    		rotate_piece(2);
    		return;
    	case 'h':
    		move_piece(-1, 2);
    		return;
    	case 'k':
    		down_piece(2);
    		return;
    	case 'l':
    		move_piece(1, 2);
    		return;

    	case ' ':
    		rotate_piece(1);
    		rotate_piece(2);
    		rotate_piece(3);
    		return;

    }
}

extern void video_mode(void);
int communism_exit() {
	mm_free(grid1);
	mm_free(grid2);
	mm_free(grid3);


//	video_mode();
	halt();
	return 0;
}
