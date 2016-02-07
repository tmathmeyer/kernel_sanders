#ifndef SANDERSSWEEPER_HEADER
#define SANDERSSWEEPER_HEADER

#define SSWEEPER_DEFAULT_WIDTH 16
#define SSWEEPER_DEFAULT_HEIGHT 12
#define SSWEEPER_MIN_WIDTH 8
#define SSWEEPER_MIN_HEIGHT 8
#define SSWEEPER_MAX_WIDTH 48
#define SSWEEPER_MAX_HEIGHT 20
#define SSWEEPER_DEFAULT_MINES 32

#define SSWEEPER_FIELD_OFFSET_X 4
#define SSWEEPER_FIELD_OFFSET_Y 4

#define SSWEEPER_MINE '*'

void sanders_sweeper_keyboard_handler(char keycode);

int sanders_sweeper_exit();

#endif // SANDERSSWEEPER_HEADER
