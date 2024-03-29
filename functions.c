/* Dudu Matei-Ioan 313CB */

#include "headers.h"

// Functie care inlocuieste \n cu \0 in sirurile care se termina in \n\0
void addNull(char *p) {
    if (p[strlen(p) - 1] == '\n') {
        p[strlen(p) - 1] = 0;
    }
}

// Functie care inverseaza valorile a doua numere
void swapNr(int *x, int *y) {
    int aux = *x;
    *x = *y;
    *y = aux;
}

/* Functie care returneaza un numar de tip int care are cifrele memorate 
intr-un sir de caractere */
int charToInt(char *num) {
    int res = 0;
    int pow = 1;
    for (int i = (int)strlen(num) - 1; i >= 0; i--) {
        res = pow * (num[i] - '0') + res;
        pow *= DEC;
    }
    return res;
}

/* Functie care citeste o imagine de tip BMP si returneaza adresa
catre structura in care s-a memorat */
BMP *editCommand(char *p, BMP *bmp) {
    FILE *fp = fopen(p, "rb");
    if (fp == NULL) {
        return NULL;
    }

    // Aloc memorie pentru FileHeader
    bmp->fileH = malloc(sizeof(bmp_fileheader));
    if (bmp->fileH == NULL) {
        fclose(fp);
        return NULL;
    }

    // Aloc memorie pentru InfoHeader
    bmp->infoH = malloc(sizeof(bmp_infoheader));
    if (bmp->infoH == NULL) {
        fclose(fp);
        return NULL;
    }

    // Aloc memorie pentru matricea de pixeli
    bmp->img = malloc(sizeof(Image));
    if (bmp->img == NULL) {
        fclose(fp);
        return NULL;
    }

    // Citesc din imagine header-ele acesteia
    fread(bmp->fileH, sizeof(bmp_fileheader), 1, fp);
    fread(bmp->infoH, sizeof(bmp_infoheader), 1, fp);

    int width = bmp->infoH->width;
    int height = bmp->infoH->height;
    int pixPerRow = 3 * width;

    // Calculez cati octeti de padding am per linie
    int paddPerRow = 0;
    if (pixPerRow % 4 != 0) {
        paddPerRow = 4 - pixPerRow % 4;
    }

    // Aloc memorie pentru matricea de pixeli
    bmp->img->height = height;
    bmp->img->width = width;
    bmp->img->pix = (pixel **) malloc(sizeof(pixel *) * height);
    if (bmp->img->pix == NULL) {
        fclose(fp);
        return NULL;
    }
    for (int i = 0; i < height; i++) {
        bmp->img->pix[i] = (pixel *) malloc(sizeof(pixel) * width);
        if (bmp->img->pix[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(bmp->img->pix[j]);
            }
            free(bmp->img->pix);
            fclose(fp);
            return NULL;
        }
        // Citesc linie cu linie in matrice
        fread(bmp->img->pix[i], width, sizeof(pixel), fp);
        for (int j = 1; j <= paddPerRow; j++) {
            // Citesc octetii de padding
            getc(fp);
        }
    }

    fclose(fp);
    return bmp;
}

/* Functie care scrie imaginea memorata intr-o structura de tip BMP intr-un
fisier de la calea memorata in zona de memorie pointata de p */
int saveCommand(char *p, BMP *bmp) {
    FILE *fp = fopen(p, "wb");
    if (fp == NULL) {
        return 0;
    }

    // Calculez cati octeti de padding am per linie
    int pixPerRow = 3 * bmp->img->width;
    int paddPerRow = 0;
    if (pixPerRow % 4 != 0) {
        paddPerRow = 4 - pixPerRow % 4;
    }

    // Scriu header-ele imaginii in fisier
    fwrite(bmp->fileH, sizeof(bmp_fileheader), 1, fp);
    fwrite(bmp->infoH, sizeof(bmp_infoheader), 1, fp);

    for (int i = 0; i < bmp->img->height; i++) {
        // Scriu linie cu linie in fisier pixelii memorati in matrice
        fwrite(bmp->img->pix[i], sizeof(pixel), bmp->img->width, fp);

        for (int j = 1; j <= paddPerRow; j++) {
            // Scriu si octetii de padding la sfarsitul fiecarei linii
            putc(0, fp);
        }
    }

    fclose(fp);
    return 1;
}

// Functie care elibereaza memoria alocata pentru o variabila de tip BMP
void freeBMP(BMP *bmp) {
    free(bmp->fileH);
    free(bmp->infoH);
    for (int i = 0; i < bmp->img->height; i++) {
        free(bmp->img->pix[i]);
    }
    free(bmp->img->pix);
    free(bmp->img);
    free(bmp);
}

/* Functie care insereaza peste imaginea salvata in bmp, imaginea de la 
calea memorata in zona de memorie pointata de p */
int insertCommand(BMP *bmp, char *p, int x, int y) {
    // Aloc memorie pentru imaginea urmeaza sa fie inserata
    BMP *insBMP = malloc(sizeof(BMP));
    if (insBMP == NULL) {
        return 0;
    }

    // Citesc imaginea
    insBMP = editCommand(p, insBMP);
    if (insBMP == NULL) {
        return 0;
    }

    // Inaltimile si latimile imaginilor
    int w1 = bmp->img->width;
    int h1 = bmp->img->height;
    int w2 = insBMP->img->width;
    int h2 = insBMP->img->height;

    int x1 = x;
    int x2 = 0;

    /* Adaug peste imaginea initiala cea de-a doua imagine. In prima imagine
    pornesc de la pixelul dat ca parametru, iar in cea de-a doua pornesc
    de la pixelul din stanga-jos al imaginii. */
    for ( ; x1 < h1 && x2 < h2; x1++, x2++) {
        int k1 = y, k2 = 0;
        for ( ; k1 < w1 && k2 < w2; k1++, k2++) {
            bmp->img->pix[x1][k1].R = insBMP->img->pix[x2][k2].R;
            bmp->img->pix[x1][k1].G = insBMP->img->pix[x2][k2].G;
            bmp->img->pix[x1][k1].B = insBMP->img->pix[x2][k2].B;
        }
    }

    // Eliberez memoria alocata pentru imaginea folosita la inserat
    freeBMP(insBMP);
    return 1;
}

/* Functie care "ingroasa" pixelul de la coordonatele (x,y) cu grosimea
memorata in pen->width */
void fillPenStroke(BMP *bmp, Pen *pen, int x, int y) {
    int height = bmp->img->height;
    int width = bmp->img->width;
    int side = pen->width;

    // Pornesc din coltul stanga-sus al patratului ce urmeaza a fi ingrosat
    x = x - (side - 1) / 2;
    y = y - (side - 1) / 2;

    for (int i = x; i < x + side; i++) {
        for (int j = y; j < y + side; j++) {
            // Conditie care ingroasa pixelul doar daca (i,j) este in matrice
            if (i >= 0 && i < height && j >= 0 && j < width) {
                bmp->img->pix[i][j].R = pen->color.R;
                bmp->img->pix[i][j].G = pen->color.G;
                bmp->img->pix[i][j].B = pen->color.B;
            }
        }
    }
}

/* Functie care trage o linie pe imagine de la pixelul (x1,y1) la (x2,y2), de
culoare si latime memorate in zona de memorie pointata de pen */
void drawLine(BMP *bmp, Pen *pen, int x1, int y1, int x2, int y2) {
    int height = bmp->img->height;
    int width = bmp->img->width;

    fillPenStroke(bmp, pen, x1, y1);
    fillPenStroke(bmp, pen, x2, y2);

    if (x1 == x2) {  // Dreapta paralela cu Oy
        if (y2 < y1) {
            swapNr(&y1, &y2);
        }
        for (int i = y1; i <= y2; i++) {
            fillPenStroke(bmp, pen, x1, i);
        }
    } else if (y1 == y2) {  // Dreapta paralela cu Ox
        if (x2 < x1) {
            swapNr(&x1, &x2);
        }
        for (int i = x1; i <= x2; i++) {
            fillPenStroke(bmp, pen, i, y1);
        }
    } else {  // Dreapta care nu este paralela nici cu Ox, nici cu Oy
        // Lungimile intervalelor [x1,x2] si [y1,y2]
        int intervalX = abs(x2 - x1);
        int intervalY = abs(y2 - y1);

        /* Verific care dintre cele doua intervale este mai mare, pentru a 
        sti cum parcurg dreapta. Dupa care parcurg intervalul de distanta mai
        mare si generez cealalta coordonata cu ajutorul ecuatiei dreptei date
        de doua puncte cunoscute */
        if (intervalX > intervalY) {
            if (x2 < x1) {
                swapNr(&x2, &x1);
                swapNr(&y2, &y1);
            }
            for (int i = x1 + 1; i < x2; i++) {
                int j = ((y2 - y1) * (i - x1) + y1 * (x2 - x1)) / (x2 - x1);
                if (j >= 0 && j < width) {
                    fillPenStroke(bmp, pen, i, j);
                }
            }
        } else {
            if (y2 < y1) {
                swapNr(&y2, &y1);
                swapNr(&x2, &x1);
            }
            for (int i = y1 + 1; i < y2; i++) {
                int j = ((x2 - x1) * (i - y1) + x1 * (y2 - y1)) / (y2 - y1);
                if (j >= 0 && j < height) {
                    fillPenStroke(bmp, pen, j, i);
                }
            }
        }
    }
}

/* Functie care deseneaza pe imagine un dreptunghi de inaltime height si
de latime width, avand coltul din stanga jos la pixelul (x1,y1) si
culoarea si grosimea memorate in zona de memorie pointata de pen */
void drawRectangle(BMP *bmp, Pen *pen, int x1, int y1, int height, int width) {
    int x2 = x1 + height, y2 = y1;
    int x3 = x2, y3 = y1 + width;
    int x4 = x1, y4 = y3;

    drawLine(bmp, pen, x1, y1, x2, y2);
    drawLine(bmp, pen, x2, y2, x3, y3);
    drawLine(bmp, pen, x3, y3, x4, y4);
    drawLine(bmp, pen, x1, y1, x4, y4);
}

/* Functie care deseneaza pe imagine un triunghi care are colturile in pixelii
(x1,y1), (x2,y2) si (x3,y3) si culoarea si latimea memorate in zona de memorie
pointata de catre pointerul pen */
void drawTriangle(BMP *bmp, Pen *pen, Point p1, Point p2, Point p3) {
    drawLine(bmp, pen, p1.x, p1.y, p2.x, p2.y);
    drawLine(bmp, pen, p2.x, p2.y, p3.x, p3.y);
    drawLine(bmp, pen, p3.x, p3.y, p1.x, p1.y);
}

/* Functie care verifica daca pixelul de la coordonatele (x,y) are aceeasi
culoare cu cea memorata in zona de memorie pointata de color */
int samePixel(BMP *bmp, pixel *color, int x, int y) {
    if (bmp->img->pix[x][y].R != color->R)
        return 0;
    if (bmp->img->pix[x][y].G != color->G)
        return 0;
    if (bmp->img->pix[x][y].B != color->B)
        return 0;
    return 1;
}

/* Functie care face fill la imagine incepand de la pixelul (x,y) cu culoarea
memorata in zona de memorie pointata de catre color */
void fill(BMP *bmp, Pen *pen, pixel *color, int x, int y) {
    // Colorez pixelul de coordonate (x,y) cu culoarea salvata in pen->color
    bmp->img->pix[x][y].B = pen->color.B;
    bmp->img->pix[x][y].G = pen->color.G;
    bmp->img->pix[x][y].R = pen->color.R;

    // Inaltimea, respectiv latimea imaginii
    int height = bmp->img->height;
    int width = bmp->img->width;

    /* Verific daca pixelii din jur sunt de aceeasi culoare si daca acestia
    exista sau daca am iesit din matrice */
    if (x - 1 >= 0 && samePixel(bmp, color, x - 1, y)) {      // jos
        fill(bmp, pen, color, x - 1, y);
    }
    if (x + 1 < height && samePixel(bmp, color, x + 1, y)) {  // sus
        fill(bmp, pen, color, x + 1, y);
    }
    if (y - 1 >= 0 && samePixel(bmp, color, x, y - 1)) {      // stanga
        fill(bmp, pen, color, x, y - 1);
    }
    if (y + 1 < width && samePixel(bmp, color, x, y + 1)) {   // dreapta
        fill(bmp, pen, color, x, y + 1);
    }
}
