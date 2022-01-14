/* Dudu Matei-Ioan 313CB */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "bmp_header.h"

#define MAXLEN 100
#define DEC 10

// Structura care contine culoarea unui pixel
typedef struct {
    unsigned char B, G, R;
} pixel;

// Structura care contine matricea unei imagini, inaltimea si latimea acesteia
typedef struct {
    pixel **pix;
    int height, width;
} Image;

// Structura care contine atat imaginea, cat si header-ele acesteia
typedef struct {
    Image *img;
    bmp_fileheader *fileH;
    bmp_infoheader *infoH;
} BMP;

// Structura care contine latimea si culoarea creionului pentru desenat
typedef struct {
    pixel color;
    int width;
} Pen;

// Structura care contine coordonatele unui pixel
typedef struct {
    int x, y;
} Point;

void addNull(char *);

void swapNr(int *, int *);

BMP * editCommand(char *, BMP *);

int saveCommand(char *, BMP *);

void freeBMP(BMP *);

int insertCommand(BMP *, char *, int, int);

int charToInt(char *);

void drawLine(BMP *, Pen *, int, int, int, int);

void drawRectangle(BMP *, Pen *, int, int, int, int);

void drawTriangle(BMP *, Pen *, Point, Point, Point);

void fill(BMP *, Pen *, pixel *, int, int);
