#include "headers.h"

void addNull(char *p)
{
    if (p[strlen(p) - 1] == '\n') {
        p[strlen(p) - 1] = 0;
    }  
}

int min(int x, int y)
{
    if (x < y) {
        return x;
    }
    return y;
}

int charToInt(char *num)
{
    int res = 0;
    int pow = 1;
    for (int i = strlen(num) - 1; i >= 0; i--) {
        res = pow * (num[i] - '0') + res;
        pow *= 10;
    }
    return res;
}

BMP *editCommand(char *p, BMP *bmp)
{
    FILE *fp = fopen(p, "rb");
    if (fp == NULL) {
        return NULL;
    }

    bmp->fileH = malloc(sizeof(bmp_fileheader));
    if (bmp->fileH == NULL) {
        fclose(fp);
        return NULL;
    }
        
    bmp->infoH = malloc(sizeof(bmp_infoheader));
    if (bmp->infoH == NULL) {
        fclose(fp);
        return NULL;
    }

    bmp->img = malloc(sizeof(Image));
    if(bmp->img == NULL) {
        fclose(fp);
        return NULL;
    }

    fread(bmp->fileH, sizeof(bmp_fileheader), 1, fp);
    fread(bmp->infoH, sizeof(bmp_infoheader), 1, fp);

    int width = bmp->infoH->width;
    int height = bmp->infoH->height;
    int pixPerRow = 3 * width;
    int paddPerRow = 0;
    if (pixPerRow % 4 != 0) {
        paddPerRow = 4 - pixPerRow % 4;
    }

    // Declararea imaginii citite + alocarea memoriei
    // Si citirea imaginii in pointerul *img
    bmp->img->height = height;
    bmp->img->width = width;
    bmp->img->pix = (pixel **) malloc(sizeof(pixel *) * height);
    if (bmp->img->pix == NULL) {
        fclose(fp);
        return NULL;
    }
    for (int i = height - 1; i >= 0; i--) {
        bmp->img->pix[i] = (pixel *) malloc(sizeof(pixel) * width);
        if (bmp->img->pix[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(bmp->img->pix[j]);
            }
            free(bmp->img->pix);
            fclose(fp);
            return NULL;
        }
        fread(bmp->img->pix[i], width, sizeof(pixel), fp);
        for (int j = 1; j <= paddPerRow; j++) {
            getc(fp);
        }
    }

    fclose(fp);
    return bmp;
}

int saveCommand(char *p, BMP *bmp)
{
    FILE *fp = fopen(p, "wb");
    if (fp == NULL) {
        return 0;
    }

    int pixPerRow = 3 * bmp->img->width;
    int paddPerRow = 0;
    if (pixPerRow % 4 != 0) {
        paddPerRow = 4 - pixPerRow % 4;
    }

    fwrite(bmp->fileH, sizeof(bmp_fileheader), 1, fp);
    fwrite(bmp->infoH, sizeof(bmp_infoheader), 1, fp);

    for (int i = bmp->img->height -1; i >=0; i--) {
        fwrite(bmp->img->pix[i], sizeof(pixel), bmp->img->width, fp);
        for (int j = 1; j <= paddPerRow; j++) {
            putc(0, fp);
        }
    }

    fclose(fp);
    return 1;
}

void freeBMP(BMP *bmp)
{
    free(bmp->fileH);
    free(bmp->infoH);
    for (int i = 0; i < bmp->img->height; i++) {
        free(bmp->img->pix[i]);
    }
    free(bmp->img->pix);
    free(bmp->img);
    free(bmp);
}

int insertCommand(BMP *bmp, char *p, int x, int y)
{
    BMP *insBMP;
    insBMP = malloc(sizeof(BMP));
    if (insBMP == NULL) {
        return 0;
    }
    insBMP = editCommand(p, insBMP);
    if (insBMP == NULL) {
        return 0;
    }

    int w1 = bmp->img->width;
    int h1 = bmp->img->height;
    int w2 = insBMP->img->width;
    int h2 = insBMP->img->height;

    int x1 = h1 - x - 1;
    int x2 = h2 - 1;

    for ( ; x1 >= 0 && x2 >= 0; x1--, x2--) {
        int k1 = y, k2 = 0;
        for ( ; k1 < w1 && k2 < w2; k1++, k2++) {
            bmp->img->pix[x1][k1].R = insBMP->img->pix[x2][k2].R;
            bmp->img->pix[x1][k1].G = insBMP->img->pix[x2][k2].G;
            bmp->img->pix[x1][k1].B = insBMP->img->pix[x2][k2].B;
        }
    }

    freeBMP(insBMP);
    return 1;
}
