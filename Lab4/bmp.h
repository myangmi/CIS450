//
// bmp.h
//
#define MAX_ROW 1600
#define MAX_COL 1600
#define NUM_COLORS 3
#define BMP_HDR_SIZE 54

typedef unsigned char byte;

int read_bmp(char*, byte*, byte a[][MAX_COL][NUM_COLORS], int*, int*, int*);
int write_bmp(char*, byte*, byte b[][MAX_COL][NUM_COLORS], int, int);
