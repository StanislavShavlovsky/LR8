#ifndef PRODUCT_H
#define PRODUCT_H

typedef struct {
    char name[50];
    int quantity;
    int workshop_number;
    char date_of_production[11];
    char type[2]; // Тип как строка, длина 2: один символ для типа и один для завершающего нуля
    union {
        int batch_number;
        float quality_score;
    } details;
} Product;


#endif 
