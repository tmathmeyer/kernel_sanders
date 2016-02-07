#include "communism.h"


#include "alloc.h"
#include "sandersboard.h"
#include "syscall.h"
#include "sand_rand.h"
#include "video.h"
#include "halt.h"

extern int init_vga(int blah);

extern unsigned char * vidmem;

unsigned char *grid;

int player_pos_x;
int player_pos_y;
int player_x[4];
int player_y[4];
unsigned char player_color;

int spin_lock;

void communism_draw_cell(int x, int y, unsigned char c) {
	int i, j;
	for (j = 1; j < CELL_SIZE-1; j++) {
		for (i = 1; i < CELL_SIZE-1; i++) {
			*(vidmem + x + i + 320* (j + y) + 115) = c;
		}
	}
	for (i = 0; i < CELL_SIZE; i++) {
		*(vidmem + x + i + 320* y + 115) = 2;
		*(vidmem + x + i + 320* (CELL_SIZE + y) + 115) = 2;
		*(vidmem + x + 320* (i + y) + 115) = 2;
		*(vidmem + x + CELL_SIZE + 320* (i + y) + 115) = 2;
	}
}

void render_grid() {
	int i, j;
	for (i = 0; i < 4; i++) {
		*(grid + player_pos_x + player_x[i] + COMMUNISM_WIDTH * (player_pos_y + player_y[i])) = player_color;
	}
	
	for (j = 0; j < COMMUNISM_HEIGHT; j++) {
		for (i = 0; i < COMMUNISM_WIDTH; i++) {
			communism_draw_cell(i * CELL_SIZE, j * CELL_SIZE, *(grid + i + j * COMMUNISM_WIDTH));
		}
	}
}

void generate_piece() {
	unsigned int piece = sand_rand() % 7;
	player_pos_x = 4;
	player_pos_y = 0;
	switch(piece) {
		case 0: // Line
			player_x[0] = -1;
			player_y[0] = 0;
			player_x[1] = 0;
			player_y[1] = 0;
			player_x[2] = 1;
			player_y[2] = 0;
			player_x[3] = 2;
			player_y[3] = 0;
			player_color = 33;
			break;
		case 1: // L
			player_x[0] = -1;
			player_y[0] = 0;
			player_x[1] = 0;
			player_y[1] = 0;
			player_x[2] = 1;
			player_y[2] = 0;
			player_x[3] = 1;
			player_y[3] = 1;
			player_color = 38;
			break;
		case 2: // Backwards L
			player_x[0] = -1;
			player_y[0] = 0;
			player_x[1] = 0;
			player_y[1] = 0;
			player_x[2] = 1;
			player_y[2] = 0;
			player_x[3] = -1;
			player_y[3] = 1;
			player_color = 18;
			break;
		case 3: // S
			player_x[0] = 0;
			player_y[0] = 0;
			player_x[1] = 1;
			player_y[1] = 0;
			player_x[2] = -1;
			player_y[2] = 1;
			player_x[3] = 0;
			player_y[3] = 1;
			player_color = 13;
			break;
		case 4: // Z
			player_x[0] = -1;
			player_y[0] = 0;
			player_x[1] = 0;
			player_y[1] = 0;
			player_x[2] = 0;
			player_y[2] = 1;
			player_x[3] = 1;
			player_y[3] = 1;
			player_color = 8;
			break;
		case 5: // T
			player_x[0] = -1;
			player_y[0] = 0;
			player_x[1] = 0;
			player_y[1] = 0;
			player_x[2] = 1;
			player_y[2] = 0;
			player_x[3] = 0;
			player_y[3] = 1;
			player_color = 28;
			break;
		case 6: // Square
			player_x[0] = 0;
			player_y[0] = 0;
			player_x[1] = 1;
			player_y[1] = 0;
			player_x[2] = 0;
			player_y[2] = 1;
			player_x[3] = 1;
			player_y[3] = 1;
			player_color = 23;
			break;
	}

	render_grid();
}

void check_grid() {
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

int down_piece() {
	int i;
	int lock = 0;
	for (i = 0; i < 4; i++) {
		*(grid + player_pos_x + player_x[i] + COMMUNISM_WIDTH * (player_pos_y + player_y[i])) = 0;
	}
	for (i = 0; i < 4; i++) {
		if (player_pos_y + player_y[i] + 1 >= COMMUNISM_HEIGHT) {
			lock = 1;
			break;
		}
		if (*(grid + player_pos_x + player_x[i] + COMMUNISM_WIDTH * (player_pos_y + player_y[i] + 1)) != 0) {
			lock = 1;
			break;
		}
	}
	if (lock) {
		for (i = 0; i < 4; i++) {
			*(grid + player_pos_x + player_x[i] + COMMUNISM_WIDTH * (player_pos_y + player_y[i])) = player_color-2;
		}
		check_grid();
		generate_piece();
		return 1;
	} else {
		player_pos_y ++;
		render_grid();
		return 0;
	}
}

void move_piece(int x) {
	int i;
	int lock = 0;
	for (i = 0; i < 4; i++) {
		*(grid + player_pos_x + player_x[i] + COMMUNISM_WIDTH * (player_pos_y + player_y[i])) = 0;
	}
	for (i = 0; i < 4; i++) {
		if (player_pos_x + player_x[i] + x >= COMMUNISM_WIDTH || player_pos_x + player_x[i] + x < 0) {
			lock = 1;
			break;
		}
		if (*(grid + player_pos_x + player_x[i] + x + COMMUNISM_WIDTH * (player_pos_y + player_y[i])) != 0) {
			lock = 1;
			break;
		}
	}
	if (lock) {
		return;
	} else {
		player_pos_x += x;
		render_grid();
	}
}

void rotate_piece() {
	int i;
	int lock = 0;
	for (i = 0; i < 4; i++) {
		*(grid + player_pos_x + player_x[i] + COMMUNISM_WIDTH * (player_pos_y + player_y[i])) = 0;
	}
	for (i = 0; i < 4; i++) {
		if (player_pos_x + player_y[i] >= COMMUNISM_WIDTH || player_pos_x + player_y[i] < 0) {
			lock = 1;
			break;
		}
		if (player_pos_y - player_x[i] >= COMMUNISM_HEIGHT || player_pos_y - player_x[i] < 0) {
			lock = 1;
			break;
		}
		if (*(grid + player_pos_x + player_y[i] + COMMUNISM_WIDTH * (player_pos_y - player_x[i])) != 0) {
			lock = 1;
			break;
		}
	}
	if (lock) {
		return;
	} else {
		int temp;
		for (i = 0; i < 4; i++) {
			temp = player_x[i];
			player_x[i] = player_y[i];
			player_y[i] = -temp;
		}
		render_grid();
	}
}

void drop_piece() {
	while(!down_piece()) {

	}
}


int communism(int argc, char *argv[]) {
	int i, j;
	init_vga(0);

	spin_lock = 100000000;

	grid = mm_alloc(COMMUNISM_WIDTH * COMMUNISM_HEIGHT);


	for (j = 0; j < COMMUNISM_HEIGHT; j++) {
		for (i = 0; i < COMMUNISM_WIDTH; i++) {
			*(grid + i + j * COMMUNISM_WIDTH) = 0;
		}
	}

	set_keyboard_handler(&communism_keyboard_handler);

	generate_piece();
	while (1) {
		i = 0; 
		while (i++ < spin_lock) {
		}

		down_piece();
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
    		rotate_piece();
    		return;
    	case DOWN_KEY_CODE:
    		down_piece();
    		return;
    	case LEFT_KEY_CODE:
    		move_piece(-1);
    		return;
    	case RIGHT_KEY_CODE:
    		move_piece(1);
    		return;
    	case ENTER_KEY_CODE:
    		drop_piece();
    }

    int ascii_key = keyboard_map[(unsigned char) keycode];

    switch(ascii_key) {
    	case ' ':
    		rotate_piece();
    		return;
    }
}

extern void video_mode(void);
int communism_exit() {
	mm_free(grid);

	video_mode();
	halt();
	return 0;
}
