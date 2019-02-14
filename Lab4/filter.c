//
// filter.c - filter input image
//
#include <stdio.h>
#include <stdlib.h>
#include "clock.h"
#include "bmp.h"
#define CMIN 1000000.0

byte bmp_hdr[BMP_HDR_SIZE];
byte a[MAX_ROW][MAX_COL][NUM_COLORS]; // input
byte b[MAX_ROW][MAX_COL][NUM_COLORS]; // naive output
byte c[MAX_ROW][MAX_COL][NUM_COLORS]; // optimized output
byte threshold[2][NUM_COLORS];
byte color[3][NUM_COLORS];

//
// read threshold file, filter.txt
//
int read_threshold(char *filename, byte threshold[2][NUM_COLORS], byte color[3][NUM_COLORS])
{
  FILE *fp;
  byte *ptr = &threshold[0][0];
  int i, x;

  fp = fopen(filename,"r+");
  for (i=0; i<6; i++)
    if (!feof(fp))
    {
      fscanf(fp, "%d", &x);
      *ptr = (byte) x;
      ptr++;
    }
  ptr = &color[0][0];
  for (i=0; i<9; i++)
    if (!feof(fp))
    {
      fscanf(fp, "%d", &x);
      *ptr = (byte) x;
      ptr++;
    }
  fclose(fp);
  printf("Low Threshold: Blue = %d, Green = %d, Red = %d\n", threshold[0][0],threshold[0][1],threshold[0][2]);
  printf("High Threshold: Blue = %d, Green = %d, Red = %d\n", threshold[1][0],threshold[1][1],threshold[1][2]);
  printf("Below Filter Color: Blue = %d, Green = %d, Red = %d\n", color[0][0],color[0][1],color[0][2]);
  printf("Filter Color: Blue = %d, Green = %d, Red = %d\n", color[1][0],color[1][1],color[1][2]);
  printf("Above Filter Color: Blue = %d, Green = %d, Red = %d\n", color[2][0],color[2][1],color[2][2]);
}

//
// naive version -- base version, leave alone
//
void filter_naive(int width, int height)
{
  int k, x, y, z;
  int inRange[3];

  for (z=0; z<3; z++)
    for (x=0; x<height; x++)
      for (y=0; y<width; y++)
      {
        inRange[0] = 0;
        inRange[1] = 0;
        inRange[2] = 0;
        for (k=0; k<3; k++)
        {
          if(a[x][y][k]<threshold[0][k])
            inRange[0] = 1;
        }
        if (inRange[0]==0)
        {
          for (k=0; k<3; k++)
          {
            if(a[x][y][k]>threshold[1][k])
              inRange[2] = 1;
          }
        }
        if (inRange[0]==0 && inRange[2]==0)
          inRange[1]=1;
        if (inRange[0])
            b[x][y][z]=color[0][z];
        else if (inRange[1])
            b[x][y][z]=color[1][z];
        else if (inRange[2])
            b[x][y][z]=color[2][z];
      }
  return;
}

//
// optimized version - modify this function
//
void filter_optimized(int width, int height)
{
  int k, x, y, z;
  int inRange[3];

for (x=0; x<height; x++)
  for (y=0; y<width; y++)
  {
        inRange[0] = 0;
        inRange[1] = 0;
        inRange[2] = 0;
        /*
        for (k=0; k<3; k++)
        {
          if(a[x][y][k]<threshold[0][k])
            inRange[0] = 1;
        }
        */
        if( a[x][y][0] < threshold[0][0] || a[x][y][1] < threshold[0][1] || a[x][y][2] < threshold[0][2])
        {
          inRange[0] = 1;
        }
        
        /*
        if (inRange[0]==0)
        {
          for (k=0; k<3; k++)
          {
            if(a[x][y][k]>threshold[1][k])
              inRange[2] = 1;
          }
        }
        */
        if(inRange[0] == 0 && (a[x][y][0]>threshold[1][0] || a[x][y][1]>threshold[1][1] || a[x][y][2]>threshold[1][2]))
        {
          inRange[2] = 1;
        }
        
        if (inRange[0]==0 && inRange[2]==0)
          inRange[1]=1;
        if (inRange[0])
        {
            c[x][y][0]=color[0][0];
            c[x][y][1]=color[0][1];
            c[x][y][2]=color[0][2];
        }
        else if (inRange[1])
        {
            c[x][y][0]=color[1][0];
            c[x][y][1]=color[1][1];
            c[x][y][2]=color[1][2];
        }    
        else if (inRange[2])
        { 
            c[x][y][0]=color[2][0];
            c[x][y][1]=color[2][1];
            c[x][y][2]=color[2][2];
        }
  }
  return;
}

int main(int argc, char *argv[])
{
  int i=0;
  int x=0, y=0, z=0;
  int width, height, size;
  double cycles, Mhz;
  int cnt = 1;
  double cmeas = 0;
  double bmeas = 0;
  double cycles_naive, cycles_optimized;

  Mhz = mhz(1);

  if (argc<4)
  {
    fprintf(stderr, "Usage: filter <filter.txt> <input.bmp> <base output.bmp> [optimized output.bmp]\n");
    exit(1);
  }
  read_threshold(argv[1], threshold, color);
  read_bmp(argv[2], bmp_hdr, a, &size, &width, &height);
  start_counter();
  do {
    int c = cnt;
    printf("cnt = %d\n", cnt);
    // Warm up cache
    filter_naive(width, height);

    bmeas = get_counter();
    while (c-- > 0)
    {
      // filter
      filter_naive(width, height);
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
      int c = cnt;
      printf("cnt = %d\n", cnt);
      // Warm up cache
      filter_optimized(width, height);

      bmeas = get_counter();
      while (c-- > 0)
      {
        // filter using optimized version
        filter_optimized(width, height);
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
    // Compute CPE - cycles per element (pixel) = cycles_optimized/(width*height)
    printf("Optimized CPE = %f cycles \n", cycles_optimized/((double)width*height));
    // Compute speedup
    printf("Speedup = %f\n", cycles_naive/cycles_optimized);
  }
  return 0;
}
