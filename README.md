# [Tema3 (Image processing)](https://ocw.cs.pub.ro/courses/programare/teme_2021/tema3_2021_cbd)

Cod realizat de Dudu Matei-Ioan (313CB).

## Numar de fisiere si rolul lor
Pentru realizarea acestei teme am folosit (excluzand README-ul) cinci fisiere:

* tema3.c
* functions.c
* bmp_header.h
* headers.h
* Makefile 

## tema3.c

Acesta este fisierul care contine functia main si care se ocupa de parsarea cuvintelor.

La inceput am declarat doi pointeri la doua tipuri diferite de structuri (BMP si Pen), dupa care le-am alocat memorie folosind functia malloc. Acesti doi pointeri sunt foarte importanti pentru rezolvarea task-urilor.

Dupa care am alocat dinamic un string de caractere pentru a putea citi linie cu linie inputul din stdin, folosindu-ma de un while:

* while(fgets(command, MAXLEN, stdin)) unde MAXLEN este definit ca 100

Mai departe, cu ajutorul pointerului *p si a functiei strtok, compar cuvintele date ca input cu lista de comenzi continuta in cerinta temei si determin semnificatia instructiunilor.

## functions.c

Acesta este fisierul care contine toate functiile (create de mine) pe care le-am utilizat pentru rezolvarea temei.

Dintre toate aceste functii, cele care merita sa fie mentionate aici sunt:

* editCommand 
  * Functie care primeste ca parametrii un sir de caractere(calea catre o imagine de tip bmp) si un pointer de tip BMP(in care va fi memorata imaginea, alaturi de header-ele sale).
  * Aceasta aloca memorie pentru fiecare dintre elementele lui BMP(fileH, infoH si img) si citeste din fisier cele doua headere ale sale. Mai departe calculeaza cati octeti de padding sunt utilizati.
  * Dupa care aloca memorie pentru matricea de pixeli si citeste fiecare linie, urmata de octetii de padding(al caror numar a fost calculat anterior).

* saveCommand
  * Functia are acelasi tip de parametrii ca editCommand.
  * In p pointeaza catre un sir de caractere care memoreaza calea spre fisierul de tip BMP ce urmeaza a fi salvat.
  * Procesul este asemanator cu cel al functiei precedente, doar ca este invers fata de aceasta. Salveaza cele doua headere, dupa care salveaza fiecare linie de pixeli, urmata de octetii de padding.

Restul functiilor sunt destul de bine explicate in comentariile din cod.

## bmp_header.h

Acesta este fisierul de tip header care contine structurile folosite ca headere pentru imaginea BMP.

## headers.h

Aceste fisier de tip header contine:

* Bibliotecile folosite in rezolvarea temei.
* Define-urile utilizate.
* Celelalte structuri create cu scopul de a ajuta la memorarea anumitor parti din program(exemple in comentariile din cod).
* Prototipurile functiilor din functions.c.

## Makefile

Contine trei target-uri:

* build - unde compilez programul
* run - unde rulez executabilul rezultat in urma compilarii(denumit bmp) 
* clean - unde sterg executabilul