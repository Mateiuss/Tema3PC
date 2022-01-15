/* Dudu Matei-Ioan 313CB */

#include "headers.h"

int main() {
    // Declar un pointer de tip BMP si ii aloc memorie
    BMP *bmp = NULL;
    bmp = malloc(sizeof(BMP));
    if (bmp == NULL) {
        return 2;
    }

    /* Declar un pointer de tip Pen, ii aloc memorie si ii setez
    valorile standard */
    Pen *pen = NULL;
    pen = malloc(sizeof(Pen));
    if (pen == NULL) {
        return 2;
    }
    pen->color.R = 0;
    pen->color.G = 0;
    pen->color.B = 0;
    pen->width = 1;

    // Sir alocat dinamic in care se memoreaza fiecare linie citita din stdin
    char * command = NULL;
    command = malloc(MAXLEN * sizeof(char));
    if (command == NULL) {
        return 2;
    }

    // Structura repetitiva care ruleaza cat timp se poate citi din stdin
    while (fgets(command, MAXLEN, stdin)) {
        // Adaug \0 in loc de \n de la sfarsitul sirului
        addNull(command);

        char *p = strtok(command, " ");

        if (strcmp("edit", p) == 0) {  // edit
            p = strtok(NULL, " ");

            bmp = editCommand(p, bmp);
            if (bmp == NULL) {
                return 2;
            }
        } else if (strcmp("save", p) == 0) {  // save
            p = strtok(NULL, " ");

            if (saveCommand(p, bmp) == 0) {
                return 2;
            }
        } else if (strcmp("quit", p) == 0) {  // quit
            // Eliberez memoria alocata
            free(command);
            freeBMP(bmp);
            free(pen);
            return 0;
        } else if (strcmp("insert", p) == 0) {  // insert
            p = strtok(NULL, " ");

            char *photo = p;
            int x = 0, y = 0;

            p = strtok(NULL, " ");
            y = charToInt(p);

            p = strtok(NULL, " ");
            x = charToInt(p);

            if (insertCommand(bmp, photo, x, y) == 0) {
                return 2;
            }
        } else if (strcmp("set", p) == 0) {  // set
            p = strtok(NULL, " ");

            if (strcmp("draw_color", p) == 0) {  // set draw_color
                p = strtok(NULL, " ");
                pen->color.R = charToInt(p);

                p = strtok(NULL, " ");
                pen->color.G = charToInt(p);

                p = strtok(NULL, " ");
                pen->color.B = charToInt(p);
            } else if (strcmp("line_width", p) == 0) {  // set line_width
                p = strtok(NULL, " ");

                pen->width = charToInt(p);
            }
        } else if (strcmp("draw", p) == 0) {  // draw
            p = strtok(NULL, " ");
            if (strcmp("line", p) == 0) {  // draw line
                int x1 = 0, y1 = 0, x2 = 0, y2 = 0;

                p = strtok(NULL, " ");
                y1 = charToInt(p);

                p = strtok(NULL, " ");
                x1 = charToInt(p);

                p = strtok(NULL, " ");
                y2 = charToInt(p);

                p = strtok(NULL, " ");
                x2 = charToInt(p);

                drawLine(bmp, pen, x1, y1, x2, y2);
            } else if (strcmp("rectangle", p) == 0) {  // draw rectangle
                int x1 = 0, y1 = 0, width = 0, height = 0;

                p = strtok(NULL, " ");
                y1 = charToInt(p);

                p = strtok(NULL, " ");
                x1 = charToInt(p);

                p = strtok(NULL, " ");
                width = charToInt(p);

                p = strtok(NULL, " ");
                height = charToInt(p);

                drawRectangle(bmp, pen, x1, y1, height, width);
            } else if (strcmp("triangle", p) == 0) {  // draw triangle
                int x1 = 0, y1 = 0, x2 = 0, y2 = 0, x3 = 0, y3 = 0;
                Point p1, p2, p3;

                p = strtok(NULL, " ");
                y1 = charToInt(p);

                p = strtok(NULL, " ");
                x1 = charToInt(p);

                p = strtok(NULL, " ");
                y2 = charToInt(p);

                p = strtok(NULL, " ");
                x2 = charToInt(p);

                p = strtok(NULL, " ");
                y3 = charToInt(p);

                p = strtok(NULL, " ");
                x3 = charToInt(p);

                p1.x = x1;
                p1.y = y1;
                p2.x = x2;
                p2.y = y2;
                p3.x = x3;
                p3.y = y3;

                drawTriangle(bmp, pen, p1, p2, p3);
            }
        } else if (strcmp("fill", p) == 0) {  // fill
            pixel color;

            p = strtok(NULL, " ");
            int y = charToInt(p);

            p = strtok(NULL, " ");
            int x = charToInt(p);

            color.R = bmp->img->pix[x][y].R;
            color.G = bmp->img->pix[x][y].G;
            color.B = bmp->img->pix[x][y].B;

            fill(bmp, pen, &color, x, y);
        }
    }
}
