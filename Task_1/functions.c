#include "Combines_headers.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


void fill_product(Product *products, int count) {
    if (count == 0) return;

    Product *prev = &products[count - 1];
    Product *current = &products[count];

    if (strlen(current->name) == 0) {
        strcpy(current->name, prev->name);
    }
    if (current->quantity == 0) {
        current->quantity = prev->quantity;
    }
    if (current->workshop_number == 0) {
        current->workshop_number = prev->workshop_number;
    }
    if (strlen(current->date_of_production) == 0) {
        strcpy(current->date_of_production, prev->date_of_production);
    }
    if (strlen(current->type) == 0) {
        strcpy(current->type, prev->type);
    }
    if (current->details.batch_number == 0) {
        current->details.batch_number = prev->details.batch_number;
    }
    if (current->details.quality_score == 0) {
        current->details.quality_score = prev->details.quality_score;
    }
}




int validate_int_input(const char *prompt, int min_value, int max_value) {
    int input;
    char ch;
    while (1) {
        printf("%s", prompt);
        if (scanf("%d", &input) == 1 && input >= min_value && input <= max_value) {
            // Проверка на наличие букв после числа
            ch = getchar();
            if (ch == '\n') {
                return input;
            } else if (isalpha(ch)) {
                printf("Ошибка ввода. После числа идут буквы. Введите только число в диапазоне от %d до %d.\n", min_value, max_value);
            } else {
                printf("Ошибка ввода. Введите число в диапазоне от %d до %d.\n", min_value, max_value);
            }
            while (getchar() != '\n'); // Очищаем буфер ввода
        } else {
            printf("Ошибка ввода. Введите число в диапазоне от %d до %d.\n", min_value, max_value);
            while (getchar() != '\n'); // Очищаем буфер ввода
        }
    }
}

float validate_float_input(const char *prompt) {
    float input;
    while (1) {
        printf("%s", prompt);
        if (scanf("%f", &input) == 1) {
            while (getchar() != '\n'); // Очищаем буфер ввода
            return input;
        } else {
            printf("Ошибка ввода. Введите действительное число.\n");
            while (getchar() != '\n'); // Очищаем буфер ввода
        }
    }
}



void validate_string_input(const char *prompt, char *input, size_t max_length) {
    printf("%s", prompt);
    fgets(input, max_length, stdin);
    input[strcspn(input, "\n")] = '\0'; // Удаляем символ новой строки, если есть
}



char validate_char_input(const char *prompt, const char *valid_chars) {
    char input[2];
    while (1) {
        printf("%s", prompt);
        fgets(input, 2, stdin);
        if (strchr(valid_chars, input[0]) != NULL && strlen(input) == 1) {
            return input[0];
        } else {
            printf("Ошибка ввода. Введите один из символов: %s\n", valid_chars);
        }
        while (getchar() != '\n'); // Очищаем буфер ввода
    }
}

void create_array(Product **products, int *count) {
    printf("Введите количество продуктов: ");
    *count = validate_int_input("", 1, 1000);
    *products = (Product *)malloc(*count * sizeof(Product));

    for (int i = 0; i < *count; i++) {
        printf("Заполнение данных для продукта %d:\n", i + 1);

        if (i > 0) {
            Product *prev = &(*products)[i - 1];

            printf("Наименование продукта (оставьте пустым для автозаполнения): ");
            validate_string_input("", (*products)[i].name, 50);
            if (strlen((*products)[i].name) == 0) {
                strcpy((*products)[i].name, prev->name);
            }

            char quantity_input[20];
            printf("Количество (оставьте пустым для автозаполнения): ");
            validate_string_input("", quantity_input, 20);
            (*products)[i].quantity = (strlen(quantity_input) > 0) ? atoi(quantity_input) : prev->quantity;

            char workshop_number_input[20];
            printf("Номер цеха (оставьте пустым для автозаполнения): ");
            validate_string_input("", workshop_number_input, 20);
            (*products)[i].workshop_number = (strlen(workshop_number_input) > 0) ? atoi(workshop_number_input) : prev->workshop_number;

            printf("Дата производства (оставьте пустым для автозаполнения): ");
            validate_string_input("", (*products)[i].date_of_production, 11);
            if (strlen((*products)[i].date_of_production) == 0) {
                strcpy((*products)[i].date_of_production, prev->date_of_production);
            }
        } else {
            validate_string_input("Введите наименование продукта: ", (*products)[i].name, 50);
            (*products)[i].quantity = validate_int_input("Введите количество: ", 1, 1000000);
            (*products)[i].workshop_number = validate_int_input("Введите номер цеха: ", 1, 1000);
            
            while (1) {
                validate_string_input("Введите дату производства (дд.мм.гггг): ", (*products)[i].date_of_production, 11);
                if (validate_date((*products)[i].date_of_production)) {
                    break;
                } else {
                    printf("Ошибка ввода даты. Попробуйте снова.\n");
                }
            }
        }

        // Ввод типа продукта и дополнительных данных вручную
        printf("Введите тип (A/B): ");
        while (1) {
            validate_string_input("", (*products)[i].type, 2);
            if (strcmp((*products)[i].type, "A") == 0 || strcmp((*products)[i].type, "B") == 0) {
                break;
            } else {
                printf("Ошибка ввода типа. Попробуйте снова (A/B): ");
            }
        }

        if (strcmp((*products)[i].type, "B") == 0) {
            (*products)[i].details.batch_number = validate_int_input("Введите номер партии: ", 1, 1000000);
        } else {
            (*products)[i].details.quality_score = validate_float_input("Введите оценку качества: ");
        }
    }
}



void view_array(const Product *products, int count) {
    for (int i = 0; i < count; i++) {
        printf("Продукт %d:\n", i + 1);
        printf("Наименование: %s\n", products[i].name);
        printf("Количество: %d\n", products[i].quantity);
        printf("Номер цеха: %d\n", products[i].workshop_number);
        printf("Дата производства: %s\n", products[i].date_of_production);
        printf("Тип: %s\n", products[i].type);
        if (products[i].type[0] == 'B') {
            printf("Номер партии: %d\n", products[i].details.batch_number);
        } else {
            printf("Оценка качества: %.2f\n", products[i].details.quality_score);
        }
    }
}


void add_product(Product **products, int *count) {
    (*count)++;
    *products = realloc(*products, *count * sizeof(Product));
    
    printf("Введите данные для нового продукта:\n");
    validate_string_input("Введите наименование продукта: ", (*products)[*count - 1].name, 50);
    (*products)[*count - 1].quantity = validate_int_input("Введите количество: ", 1, 1000000);
    (*products)[*count - 1].workshop_number = validate_int_input("Введите номер цеха: ", 1, 1000);
    
    while (1) {
        validate_string_input("Введите дату производства (дд.мм.гггг): ", (*products)[*count - 1].date_of_production, 11);
        if (validate_date((*products)[*count - 1].date_of_production)) {
            break;
        } else {
            printf("Ошибка ввода даты. Попробуйте снова.\n");
        }
    }

    // Ввод типа продукта и дополнительных данных вручную
    printf("Введите тип (A/B): ");
    while (1) {
        validate_string_input("", (*products)[*count - 1].type, 2);
        if (strcmp((*products)[*count - 1].type, "A") == 0 || strcmp((*products)[*count - 1].type, "B") == 0) {
            break;
        } else {
            printf("Ошибка ввода типа. Попробуйте снова (A/B): ");
        }
    }

    if (strcmp((*products)[*count - 1].type, "B") == 0) {
        (*products)[*count - 1].details.batch_number = validate_int_input("Введите номер партии: ", 1, 1000000);
    } else {
        (*products)[*count - 1].details.quality_score = validate_float_input("Введите оценку качества: ");
    }
}



void search_product(const Product *products, int count, const char *name) {
    for (int i = 0; i < count; i++) {
        if (strcmp(products[i].name, name) == 0) {
            printf("Продукт найден:\n");
            printf("Наименование: %s\n", products[i].name);
            printf("Количество: %d\n", products[i].quantity);
            printf("Номер цеха: %d\n", products[i].workshop_number);
            printf("Дата производства: %s\n", products[i].date_of_production);
            printf("Тип: %s\n", products[i].type);
            if (products[i].type[0] == 'B') {
                printf("Номер партии: %d\n", products[i].details.batch_number);
            } else {
                printf("Оценка качества: %.2f\n", products[i].details.quality_score);
            }
            return;
        }
    }
    printf("Продукт с наименованием %s не найден.\n", name);
}


void delete_product(Product **products, int *count, const char *name) {
    for (int i = 0; i < *count; i++) {
        if (strcmp((*products)[i].name, name) == 0) {
            for (int j = i; j < *count - 1; j++) {
                (*products)[j] = (*products)[j + 1];
            }
            (*count)--;
            *products = (Product *)realloc(*products, *count * sizeof(Product));
            break;
        }
    }
}

void modify_product(Product *products, int count, const char *name) {
    for (int i = 0; i < count; i++) {
        if (strcmp(products[i].name, name) == 0) {
            printf("Изменение продукта %s:\n", name);
            products[i].quantity = validate_int_input("Введите новое количество: ", 1, 1000000);
            products[i].workshop_number = validate_int_input("Введите новый номер цеха: ", 1, 1000);

            while (1) {
                validate_string_input("Введите новую дату производства (дд.мм.гггг): ", products[i].date_of_production, 11);
                if (validate_date(products[i].date_of_production)) {
                    break;
                } else {
                    printf("Ошибка ввода даты. Попробуйте снова.\n");
                }
            }

            printf("Введите новый тип (A/B): ");
            char type_input[2];
            validate_string_input("", type_input, 2);
            strcpy(products[i].type, type_input);

            if (products[i].type[0] == 'B') {
                products[i].details.batch_number = validate_int_input("Введите новый номер партии: ", 1, 1000000);
            } else {
                products[i].details.quality_score = validate_float_input("Введите новую оценку качества: ");
            }
            
            save_to_file(products, count, "products.bin");
            break;
        }
    }
}


void shaker_sort_products(Product *products, int count) {
    int left = 0;
    int right = count - 1;
    while (left < right) {
        for (int i = left; i < right; i++) {
            if (strcmp(products[i].name, products[i + 1].name) > 0) {
                Product temp = products[i];
                products[i] = products[i + 1];
                products[i + 1] = temp;
            }
        }
        right--;

        for (int i = right; i > left; i--) {
            if (strcmp(products[i].name, products[i - 1].name) < 0) {
                Product temp = products[i];
                products[i] = products[i - 1];
                products[i - 1] = temp;
            }
        }
        left++;
    }
}


void save_to_file(const Product *products, int count, const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (file != NULL) {
        fwrite(&count, sizeof(int), 1, file);
        fwrite(products, sizeof(Product), count, file);
        fclose(file);
        printf("Данные сохранены в файл %s\n", filename);
    } else {
        printf("Ошибка при открытии файла для записи.\n");
    }
}

void load_from_file(Product **products, int *count, const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file != NULL) {
        fread(count, sizeof(int), 1, file);
        *products = (Product *)malloc(*count * sizeof(Product));
        fread(*products, sizeof(Product), *count, file);
        fclose(file);
        printf("Данные загружены из файла %s\n", filename);
    } else {
        printf("Ошибка при открытии файла для чтения.\n");
    }
}

void get_quantity_by_workshop(const Product *products, int count, int workshop_number) {
    int total_quantity = 0;
    int found = 0;

    for (int i = 0; i < count; i++) {
        if (products[i].workshop_number == workshop_number) {
            total_quantity += products[i].quantity;
            found = 1;
        }
    }

    if (found) {
        printf("Общее количество выпущенных изделий для цеха %d: %d\n", workshop_number, total_quantity);
    } else {
        printf("Цех с номером %d не найден.\n", workshop_number);
    }
}

void shaker_sort(Product *products, int count) {
    int left = 0;
    int right = count - 1;
    int shift = 0;

    while (left < right) {
        for (int i = left; i < right; i++) {
            if (products[i].quantity < products[i + 1].quantity) {
                Product temp = products[i];
                products[i] = products[i + 1];
                products[i + 1] = temp;
                shift = i;
            }
        }
        right = shift;
        for (int i = right; i > left; i--) {
            if (products[i].quantity > products[i - 1].quantity) {
                Product temp = products[i];
                products[i] = products[i - 1];
                products[i - 1] = temp;
                shift = i;
            }
        }
        left = shift;
    }
}

int validate_date(const char *date) {
    if (strlen(date) != 10) {
        return 0;
    }
    if (date[2] != '.' || date[5] != '.') {
        return 0;
    }
    for (int i = 0; i < 10; i++) {
        if (i == 2 || i == 5) continue;
        if (!isdigit(date[i])) {
            return 0;
        }
    }

    int day, month, year;
    sscanf(date, "%2d.%2d.%4d", &day, &month, &year);

    if (day < 1 || day > 31 || month < 1 || month > 12 || year < 1) {
        return 0;
    }
    if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30) {
        return 0;
    }
    if (month == 2) {
        int is_leap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
        if (day > 29 || (day == 29 && !is_leap)) {
            return 0;
        }
    }
    return 1;
}
