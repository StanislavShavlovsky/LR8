#ifndef BOOK_H
#define BOOK_H

typedef struct {
    char title[100];
    char author[100];
    char isbn[20];
    int publication_year;
    char genre[50];
    char type[7]; // Тип как строка, длина 7: для 'pages' или 'rating'
    union {
        int pages;
        float rating;
    } details;
} Book;


#endif // BOOK_H

