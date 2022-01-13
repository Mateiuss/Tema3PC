#include "headers.h"

int main()
{
    // Declar un element din structura BMP (continand imaginea si headerele)

    BMP *bmp = NULL;
    bmp = malloc(sizeof(BMP));
    if (bmp == NULL) {
        return 2;
    }

    Pen *pen = NULL;
    pen = malloc(sizeof(Pen));
    if (pen == NULL) {
        return 2;
    }
    pen->color.R = 0;
    pen->color.G = 0;
    pen->color.B = 0;
    pen->width = 1;

    // Sir in care se memoreaza fiecare linie citita
    // + aloc memorie pentru el

    char * command;
    command = malloc(MAXLEN * sizeof(char));
    if (command == NULL) {
        return 2;
    }
    
    // Structura repetitiva care ruleaza cat timp se poate citi din fisier
    // Parser
    while (fgets(command, MAXLEN, stdin)) {
        addNull(command); // Adaug NULL in locul \n de la finalul sirului

        char *p = strtok(command, " ");

        // Parsarea comenzilor
        if (strcmp("edit", p) == 0) { // edit
            p = strtok(NULL, " ");
            bmp = editCommand(p, bmp);
            if (bmp == NULL) {
                return 2;
            }
        }
        else if (strcmp("save", p) == 0) { // save
            p = strtok(NULL, " ");
            if (saveCommand(p, bmp) == 0) {
                return 2;
            }
        }
        else if (strcmp("quit", p) == 0) { // quit
            free(command);
            freeBMP(bmp);
            // DON'T FORGET TO FREE THE PEN!!
            return 0;
        }
        else if (strcmp("insert", p) == 0) {
            p = strtok(NULL, " ");
            char *y , *x;
            y = strtok(NULL, " ");
            x = strtok(NULL, " ");
            if (insertCommand(bmp, p, charToInt(x), charToInt(y)) == 0) {
                return 2;
            }
        }
        else if (strcmp("set", p) == 0) {
            p = strtok(NULL, " ");
            if (strcmp ("draw_color", p) == 0) {
                p = strtok(NULL, " ");
                pen->color.R = charToInt(p);
                p = strtok(NULL, " ");
                pen->color.G = charToInt(p);
                p = strtok(NULL, " ");
                pen->color.B = charToInt(p);
            }
            else if (strcmp ("line_width", p) == 0) {
                p = strtok(NULL, " ");
                pen->width = charToInt(p);
            }
        }
        else if (strcmp("draw", p) == 0) {
            p = strtok(NULL, " ");
            if(strcmp("line", p) == 0) {
                char *x1, *y1, *x2, *y2;
                y1 = strtok(NULL, " ");
                x1 = strtok(NULL, " ");
                y2 = strtok(NULL, " ");
                x2 = strtok(NULL, " ");
                drawLine(bmp, pen, charToInt(x1), charToInt(y1), charToInt(x2), charToInt(y2));
            }
            else if (strcmp("rectangle", p) == 0) {
                int x1, y1, width, height;
                p = strtok(NULL, " ");
                y1 = charToInt(p);
                p = strtok(NULL, " ");
                x1 = charToInt(p);
                p = strtok(NULL, " ");
                width = charToInt(p);
                p = strtok(NULL, " ");
                height = charToInt(p);
                drawRectangle(bmp, pen, x1, y1, height, width);
            }
            else if (strcmp("triangle", p) == 0) {
                int x1, y1, x2, y2, x3, y3;
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
                drawTriangle(bmp, pen, x1, y1, x2, y2, x3, y3);
            }
        }
    }
}