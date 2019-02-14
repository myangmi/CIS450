//
// emboss.c - apply convolution mask to interior points to emboss
//          - an image set edge pixels to black = (0,0,0)
//
#include <stdio.h>
#include <stdlib.h>
#include "clock.h"
#include "bmp.h"
#define CMIN 100000.0

byte bmp_hdr[BMP_HDR_SIZE];
byte a[MAX_ROW][MAX_COL][NUM_COLORS];
byte b[MAX_ROW][MAX_COL][NUM_COLORS];
byte c[MAX_ROW][MAX_COL][NUM_COLORS];
int  mask[3][3];

//
// read mask file
//
int read_mask(char *filename, int mask[3][3])
{
  FILE *fp;
  int *mask_ptr = &mask[0][0];
  int i;

  fp = fopen(filename,"r+");
  for (i=0; i<9; i++)
    if (!feof(fp))
      fscanf(fp, "%d", mask_ptr++);
  fclose(fp);
}

//
// emboss_naive - the naive baseline version
//
void emboss_naive(int width, int height)
{
  int i, j, k;
  int tmp;

  for (i = 1; i < height-1; i++)
    for (j = 1; j < width-1; j++)
      for (k = 0; k < 3; k++)
      {
        tmp = mask[0][0]*a[i-1][j-1][k]+
              mask[1][0]*a[i][j-1][k]+
              mask[2][0]*a[i+1][j-1][k]+
              mask[0][1]*a[i-1][j][k]+
              mask[1][1]*a[i][j][k]+
              mask[2][1]*a[i+1][j][k]+
              mask[0][2]*a[i-1][j+1][k]+
              mask[1][2]*a[i][j+1][k]+
              mask[2][2]*a[i+1][j+1][k];
	    if (tmp>255)
              tmp = 255;
        if (tmp<0)
              tmp = 0;
        b[i][j][k] = 255-tmp;
      }
  return;
}

//
// optimized version
//
// Hint: the optimized version only needs to work for mask emboss.txt
//
void emboss_optimized(int width, int height)
{
  int i, j, k;
  int tmp;

  for (i = 1; i < height-1; i++)
    for (j = 1; j < width-1; j++)
      for (k = 0; k < 3; k++)
      {
        tmp = mask[0][0]*a[i-1][j-1][k]+
              mask[1][0]*a[i][j-1][k]+
              mask[2][0]*a[i+1][j-1][k]+
              mask[0][1]*a[i-1][j][k]+
              mask[1][1]*a[i][j][k]+
              mask[2][1]*a[i+1][j][k]+
              mask[0][2]*a[i-1][j+1][k]+
              mask[1][2]*a[i][j+1][k]+
              mask[2][2]*a[i+1][j+1][k];
	    if (tmp>255)
              tmp = 255;
        if (tmp<0)
              tmp = 0;
        c[i][j][k] = 255-tmp;
      }
  return;
}

int main(int argc, char *argv[])
{
  int i=0;
  int width, height, size;
  double cycles, Mhz;
  int cnt = 1;
  double cmeas = 0;
  double bmeas = 0;
  double cycles_naive, cycles_optimized;
  char filename[256];

  Mhz = mhz(1);

  if (argc<4)
  {
    fprintf(stderr, "Usage: emboss <mask> <input> <base output> [optimized output]\n");
    exit(1);
  }
  read_mask(argv[1], mask);
  read_bmp(argv[2], bmp_hdr, a, &size, &width, &height);
  start_counter();
  do {
    int n = cnt;
    printf("cnt = %d\n", cnt);
    // Warm up cache
    emboss_naive(width, height);

    bmeas = get_counter();
    while (n-- > 0)
    {
      // Smooth
      emboss_naive(width, height);
    }
    cmeas = get_counter();
    cycles = (cmeas-bmeas);
    cnt += cnt;
  } while ((cmeas-bmeas) < CMIN);  /* Make sure have enough */
  printf("Naive cnt = %d\n", cnt);
  printf("Naive cycles = %f, MHz = %f, cycles/Mhz = %f\n", cycles, Mhz, cycles/Mhz);
  printf("Naive elapsed time per image = %f seconds \n", (cycles/cnt)/(1.0e6*Mhz));
  cycles_naive = cycles/cnt;
  // Compute CPE - cycles per element (pixel) = cycles_naive/(width*height)
  printf("Naive CPE = %f cycles \n", cycles_naive/((double)width*height));
  write_bmp(argv[3], bmp_hdr, b, width, height);

  if (argc > 4)
  {
    cnt = 1;
    do {
      int n = cnt;
      printf("cnt = %d\n", cnt);
      // Warm up cache
      emboss_naive(width, height);

      bmeas = get_counter();
      while (n-- > 0)
      {
        // emboss edges using optimized version
        emboss_optimized(width, height);
      }
      cmeas = get_counter();
      cycles = (cmeas-bmeas);
      cnt += cnt;
    } while ((cmeas-bmeas) < CMIN);  /* Make sure have enough */
    printf("Optimized cnt = %d\n", cnt);
    printf("Optimized cycles = %f, MHz = %f, cycles/Mhz = %f\n", cycles, Mhz, cycles/Mhz);
    printf("Optimized elapsed time per image = %f seconds \n", (cycles/cnt)/(1.0e6*Mhz));
    cycles_optimized = cycles/cnt;
    write_bmp(argv[4], bmp_hdr, c, width, height);
    // Compute speedup
    printf("Speedup = %f\n", cycles_naive/cycles_optimized);
    // Compute CPE - cycles per element (pixel) = cycles_optimized/(width*height)
    printf("Optimized CPE = %f cycles \n", cycles_optimized/((double)width*height));
  }
  return 0;
}

