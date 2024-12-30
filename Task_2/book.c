#include <stdio.h>
#include <stdlib.h>
#include <ctype.h> 
#include <string.h>
#include "Combined_headers.h"

void menu() {
    printf("Меню:\n");
    printf("1. Ввести новый массив книг\n");
    printf("2. Просмотреть все книги\n");
    printf("3. Добавить новую книгу\n");
    printf("4. Поиск книги по названию\n");
    printf("5. Удалить книгу по названию\n");
    printf("6. Изменить книгу по названию\n");
    printf("7. Сортировать книги\n");
    printf("8. Сохранить информацию о книгах в текстовый файл\n");
    printf("9. Загрузить информацию о книгах из текстового файла\n");
    printf("10. Вывести список книг, изданных после заданного года\n");
    printf("11. Создать индекс и индексирование в Elasticsearch\n");
    printf("12. Поиск в Elasticsearch\n");
    printf("13. Удалить индекс в Elasticsearch\n");
    printf("14. Выход\n");
    printf("Введите ваш выбор: ");
}


void add_book(Book **books, int *count) {
    (*count)++;
    *books = realloc(*books, *count * sizeof(Book));
    
    printf("Введите данные для новой книги:\n");
    validate_string_input("Введите название книги: ", (*books)[*count - 1].title, sizeof((*books)[*count - 1].title));
    validate_string_input("Введите автора книги: ", (*books)[*count - 1].author, sizeof((*books)[*count - 1].author));
    validate_string_input("Введите ISBN книги: ", (*books)[*count - 1].isbn, sizeof((*books)[*count - 1].isbn));
    (*books)[*count - 1].publication_year = validate_int_input("Введите год издания книги: ", 1000, 2100);
    validate_string_input("Введите жанр книги: ", (*books)[*count - 1].genre, sizeof((*books)[*count - 1].genre));

    // Ввод типа информации и соответствующих данных вручную
    printf("Введите тип информации (pages/rating): ");
    while (1) {
        validate_string_input("", (*books)[*count - 1].type, sizeof((*books)[*count - 1].type));
        if (strcmp((*books)[*count - 1].type, "pages") == 0) {
            (*books)[*count - 1].details.pages = validate_int_input("Введите количество страниц: ", 1, 10000);
            break;
        } else if (strcmp((*books)[*count - 1].type, "rating") == 0) {
            (*books)[*count - 1].details.rating = validate_float_input("Введите рейтинг книги: ");
            break;
        } else {
            printf("Ошибка ввода типа. Попробуйте снова (pages/rating): ");
        }
    }
}


void delete_book(Book **books, int *count, const char *title) {
    for (int i = 0; i < *count; i++) {
        if (strcmp((*books)[i].title, title) == 0) {
            for (int j = i; j < *count - 1; j++) {
                (*books)[j] = (*books)[j + 1];
            }
            (*count)--;
            *books = realloc(*books, *count * sizeof(Book));
            printf("Книга с названием %s удалена.\n", title);
            return;
        }
    }
    printf("Книга с названием %s не найдена.\n", title);
}

void modify_book(Book *books, int count, const char *title) {
    for (int i = 0; i < count; i++) {
        if (strcmp(books[i].title, title) == 0) {
            printf("Изменение данных для книги %s:\n", title);
            validate_string_input("Введите новое название книги: ", books[i].title, 100);
            validate_string_input("Введите нового автора книги: ", books[i].author, 100);
            validate_string_input("Введите новый ISBN книги: ", books[i].isbn, 20);
            books[i].publication_year = validate_int_input("Введите новый год издания книги: ", 1000, 2100);
            validate_string_input("Введите новый жанр книги: ", books[i].genre, 50);

            printf("Введите количество страниц (или 0, если хотите ввести рейтинг): ");
            int pages = validate_int_input("", 0, 10000);
            if (pages > 0) {
                books[i].details.pages = pages;
            } else {
                books[i].details.rating = validate_float_input("Введите рейтинг книги: ");
            }

            printf("Данные для книги %s успешно изменены.\n", title);
            return;
        }
    }
    printf("Книга с названием %s не найдена.\n", title);
}


void sort_books(Book *books, int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (strcmp(books[j].author, books[j + 1].author) > 0) {
                Book temp = books[j];
                books[j] = books[j + 1];
                books[j + 1] = temp;
            }
        }
    }
}

void save_to_file(const Book *books, int count, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Ошибка открытия файла.\n");
        return;
    }
    fprintf(file, "%d\n", count);
    for (int i = 0; i < count; i++) {
        fprintf(file, "%s\n%s\n%s\n%d\n%s\n%s\n", 
                books[i].title, 
                books[i].author, 
                books[i].isbn, 
                books[i].publication_year, 
                books[i].genre, 
                books[i].type);
        if (strcmp(books[i].type, "pages") == 0) {
            fprintf(file, "%d\n", books[i].details.pages);
        } else if (strcmp(books[i].type, "rating") == 0) {
            fprintf(file, "%.2f\n", books[i].details.rating);
        }
    }
    fclose(file);
    printf("Данные сохранены в файл %s.\n", filename);
}


void load_from_file(Book **books, int *count, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Ошибка открытия файла.\n");
        return;
    }
    fscanf(file, "%d\n", count);
    *books = (Book *)malloc(*count * sizeof(Book));
    for (int i = 0; i < *count; i++) {
        fgets((*books)[i].title, sizeof((*books)[i].title), file);
        (*books)[i].title[strcspn((*books)[i].title, "\n")] = 0; // Удаляем символ новой строки
        fgets((*books)[i].author, sizeof((*books)[i].author), file);
        (*books)[i].author[strcspn((*books)[i].author, "\n")] = 0; // Удаляем символ новой строки
        fgets((*books)[i].isbn, sizeof((*books)[i].isbn), file);
        (*books)[i].isbn[strcspn((*books)[i].isbn, "\n")] = 0; // Удаляем символ новой строки
        fscanf(file, "%d\n", &(*books)[i].publication_year);
        fgets((*books)[i].genre, sizeof((*books)[i].genre), file);
        (*books)[i].genre[strcspn((*books)[i].genre, "\n")] = 0; // Удаляем символ новой строки
        fgets((*books)[i].type, sizeof((*books)[i].type), file);
        (*books)[i].type[strcspn((*books)[i].type, "\n")] = 0; // Удаляем символ новой строки
        if (strcmp((*books)[i].type, "pages") == 0) {
            fscanf(file, "%d\n", &(*books)[i].details.pages);
        } else if (strcmp((*books)[i].type, "rating") == 0) {
            fscanf(file, "%f\n", &(*books)[i].details.rating);
        }
    }
    fclose(file);
    printf("Данные загружены из файла %s.\n", filename);
}




void get_books_by_year(const Book *books, int count, int year) {
    int total_books = 0;
    for (int i = 0; i < count; i++) {
        if (books[i].publication_year > year) {
            total_books++;
            printf("Книга %d:\n", total_books);
            printf("Название: %s\n", books[i].title);
            printf("Автор: %s\n", books[i].author);
            printf("ISBN: %s\n", books[i].isbn);
            printf("Год издания: %d\n", books[i].publication_year);
            printf("Жанр: %s\n", books[i].genre);
            if (books[i].details.pages > 0) {
                printf("Количество страниц: %d\n", books[i].details.pages);
            } else {
                printf("Рейтинг: %.2f\n", books[i].details.rating);
            }
        }
    }
    if (total_books == 0) {
        printf("Нет книг, изданных после %d года.\n", year);
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


void create_array(Book **books, int *count) {
    printf("Введите количество книг: ");
    *count = validate_int_input("", 1, 1000);
    *books = (Book *)malloc(*count * sizeof(Book));

    for (int i = 0; i < *count; i++) {
        printf("Заполнение данных для книги %d:\n", i + 1);

        validate_string_input("Введите название книги: ", (*books)[i].title, sizeof((*books)[i].title));
        validate_string_input("Введите автора книги: ", (*books)[i].author, sizeof((*books)[i].author));
        validate_string_input("Введите ISBN книги: ", (*books)[i].isbn, sizeof((*books)[i].isbn));
        (*books)[i].publication_year = validate_int_input("Введите год издания книги: ", 1000, 2100);
        validate_string_input("Введите жанр книги: ", (*books)[i].genre, sizeof((*books)[i].genre));

        // Ввод типа информации и соответствующих данных вручную
        printf("Введите тип информации (pages/rating): ");
        while (1) {
            validate_string_input("", (*books)[i].type, sizeof((*books)[i].type));
            if (strcmp((*books)[i].type, "pages") == 0) {
                (*books)[i].details.pages = validate_int_input("Введите количество страниц: ", 1, 10000);
                break;
            } else if (strcmp((*books)[i].type, "rating") == 0) {
                (*books)[i].details.rating = validate_float_input("Введите рейтинг книги: ");
                break;
            } else {
                printf("Ошибка ввода типа. Попробуйте снова (pages/rating): ");
            }
        }
    }
}




void view_array(const Book *books, int count) {
    for (int i = 0; i < count; i++) {
        printf("Книга %d:\n", i + 1);
        printf("Название: %s\n", books[i].title);
        printf("Автор: %s\n", books[i].author);
        printf("ISBN: %s\n", books[i].isbn);
        printf("Год издания: %d\n", books[i].publication_year);
        printf("Жанр: %s\n", books[i].genre);
        if (strcmp(books[i].type, "pages") == 0) {
            printf("Количество страниц: %d\n", books[i].details.pages);
        } else if (strcmp(books[i].type, "rating") == 0) {
            printf("Рейтинг: %.2f\n", books[i].details.rating);
        }
        printf("\n");
    }
}


void search_book(const Book *books, int count, const char *title) {
    for (int i = 0; i < count; i++) {
        if (strcmp(books[i].title, title) == 0) {
            printf("Книга найдена:\n");
            printf("Название: %s\n", books[i].title);
            printf("Автор: %s\n", books[i].author);
            printf("ISBN: %s\n", books[i].isbn);
            printf("Год издания: %d\n", books[i].publication_year);
            printf("Жанр: %s\n", books[i].genre);
            if (books[i].details.pages > 0) {
                printf("Количество страниц: %d\n", books[i].details.pages);
            } else {
                printf("Рейтинг: %.2f\n", books[i].details.rating);
            }
            printf("\n");
            return;
        }
    }
    printf("Книга с названием %s не найдена.\n", title);
}


