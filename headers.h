#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "bmp_header.h"

#define MAXLEN 100
#define DEC 10

void addNull(char *);

void swapNr(int *, int *);

BMP * editCommand(char *, BMP *);

int saveCommand(char *, BMP *);

void freeBMP(BMP *);

int insertCommand(BMP *, char *, int, int);

int charToInt(char *);

void drawLine(BMP *, Pen *, int, int, int, int);

void drawRectangle(BMP *, Pen *, int, int, int, int);

void drawTriangle(BMP *, Pen *, int, int, int, int, int, int);

void fill(BMP *, Pen *, pixel *, int, int);
