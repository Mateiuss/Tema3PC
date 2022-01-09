#include "headers.h"

void addNull(char *p)
{
    if (p[strlen(p) - 1] == '\n') {
        p[strlen(p) - 1] = 0;
    }  
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