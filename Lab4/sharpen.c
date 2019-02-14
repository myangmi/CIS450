//
// sharpen.c - apply convolution mask to interior points to sharpen image
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
// initialize mask
//
void init_mask()
{
  mask[0][0]=0;
  mask[0][1]=-1;
  mask[0][2]=0;
  mask[1][0]=-1;
  mask[1][1]=5;
  mask[1][2]=-1;
  mask[2][0]=0;
  mask[2][1]=-1;
  mask[2][2]=0;

  return;
}

//
// sharpen_naive - the naive baseline version
//
void sharpen_naive(int width, int height)
{
  int i, j, k;
  int tmp;

  init_mask();

  for (k = 0; k < 3; k++)
    for (j = 1; j < width-1; j++)
      for (i = 1; i < height-1; i++)
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
        b[i][j][k] = tmp;
      }
  return;
}

//
// optimized version
//
void sharpen_optimized(int width, int height)
{
  int i, j;
  int tmp, tmp2, tmp3;

  //int mask10 = -1;
  //int mask01 = -1;
  //int mask11 = 5;
  //int mask21 = -1;
  //int mask12 = -1;
  
  byte* center;
  byte* left;
  byte* right;
  byte* topcenter;
  byte* bottomcenter;
  height--;
  width--;


  
    for (i = 1; i < height; i++)
      for (j = 1; j < width; j++)
      {
        
        left = a[i][j-1];
        center = a[i][j];
        right = a[i][j+1];
        topcenter = a[i-1][j];
        bottomcenter = a[i+1][j];
        
        
        tmp = 5**center -
              *left -
              *topcenter -
              *bottomcenter -
              *right;
       
        
        
        tmp2 = 5**(center + 1) -
              *(left + 1) -
              *(topcenter + 1) -
              *(bottomcenter + 1) -
              *(right + 1);
        
        
        
        tmp3 = 5**(center + 2) -
              *(left + 2) -
              *(topcenter + 2) -
              *(bottomcenter + 2) -
              *(right + 2);
        
         if (tmp<1)
              c[i][j][0] = 0;
        else if (tmp>254)
              c[i][j][0] = 255;
        else c[i][j][0] = tmp;
        
              
              
        if (tmp2<1)
              c[i][j][1] = 0;
        else if (tmp2>254)
              c[i][j][1] = 255;
        else c[i][j][1] = tmp2;
       
              
        if (tmp3<1)
              c[i][j][2] = 0;
        else if (tmp3>254)
              c[i][j][2] = 255;
        else c[i][j][2] = tmp3;
              
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
    fprintf(stderr, "Usage: sharpen <input> <base output> [optimized output]\n");
    exit(1);
  }
  read_bmp(argv[1], bmp_hdr, a, &size, &width, &height);
  start_counter();
  do {
    int n = cnt;
    printf("cnt = %d\n", cnt);
    // Warm up cache
    sharpen_naive(width, height);

    bmeas = get_counter();
    while (n-- > 0)
    {
      // Smooth
      sharpen_naive(width, height);
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
  write_bmp(argv[2], bmp_hdr, b, width, height);

  if (argc > 3)
  {
    cnt = 1;
    do {
      int n = cnt;
      printf("cnt = %d\n", cnt);
      // Warm up cache
      sharpen_naive(width, height);

      bmeas = get_counter();
      while (n-- > 0)
      {
        // sharpen edges using optimized version
        sharpen_optimized(width, height);
      }
      cmeas = get_counter();
      cycles = (cmeas-bmeas);
      cnt += cnt;
    } while ((cmeas-bmeas) < CMIN);  /* Make sure have enough */
    printf("Optimized cnt = %d\n", cnt);
    printf("Optimized cycles = %f, MHz = %f, cycles/Mhz = %f\n", cycles, Mhz, cycles/Mhz);
    printf("Optimized elapsed time per image = %f seconds \n", (cycles/cnt)/(1.0e6*Mhz));
    cycles_optimized = cycles/cnt;
    write_bmp(argv[3], bmp_hdr, c, width, height);
    // Compute speedup
    printf("Speedup = %f\n", cycles_naive/cycles_optimized);
    // Compute CPE - cycles per element (pixel) = cycles_optimized/(width*height)
    printf("Optimized CPE = %f cycles \n", cycles_optimized/((double)width*height));
  }
  return 0;
}

