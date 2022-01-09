#include "headers.h"

int main()
{
    // Declar un element din structura BMP (continand imaginea si headerele)
    BMP *bmp = NULL;
    bmp = malloc(sizeof(BMP));
    if (bmp == NULL) {
        return 2;
    }
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
            free(bmp->fileH);
            free(bmp->infoH);
            for (int i = 0; i < bmp->img->height; i++) {
                free(bmp->img->pix[i]);
            }
            free(bmp->img->pix);
            free(bmp->img);
            free(bmp);
            return 0;
        }
        else if (strcmp("insert", p) == 0) {
            
        }
    }
}