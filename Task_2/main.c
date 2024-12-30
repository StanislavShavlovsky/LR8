#include <stdio.h>
#include <stdlib.h>
#include <ctype.h> // Добавляем для функции tolower
#include "Combined_headers.h"


void search_menu() {
    printf("Меню поиска:\n");
    printf("1. Поиск по названию\n");
    printf("2. Поиск по автору\n");
    printf("3. Поиск по ISBN\n");
    printf("4. Поиск по году издания\n");
    printf("5. Поиск по жанру\n");
    printf("6. Поиск по количеству страниц\n");
    printf("7. Поиск по рейтингу\n");
    printf("Введите ваш выбор: ");
}

void search_choice_menu() {
    printf("Тип поиска:\n");
    printf("1. Поиск по диапазону\n");
    printf("2. Нечёткий поиск\n");
    printf("Введите ваш выбор: ");
}

void to_lowercase(char *str) {
    for (size_t i = 0; i < strlen(str); i++) {
        str[i] = tolower(str[i]);
    }
}

int main() {
    Book *books = NULL;
    int count = 0;
    int choice, search_type;
    char search_value[100];
    char index_name[100] = "library_index";
    to_lowercase(index_name); // Приводим index_name к нижнему регистру
    long min_value, max_value;
    const char *filename = "library.txt";

    while (1) {
        menu();
        choice = validate_int_input("", 1, 15);

        switch (choice) {
            case 1:
                create_array(&books, &count);
                break;
            case 2:
                view_array(books, count);
                break;
            case 3:
                add_book(&books, &count);
                break;
            case 4:
                validate_string_input("Введите название книги для поиска: ", search_value, 100);
                search_book(books, count, search_value);
                break;
            case 5:
                validate_string_input("Введите название книги для удаления: ", search_value, 100);
                delete_book(&books, &count, search_value);
                break;
            case 6:
                validate_string_input("Введите название книги для изменения: ", search_value, 100);
                modify_book(books, count, search_value);
                break;
            case 7:
                sort_books(books, count);
                break;
            case 8:
                save_to_file(books, count, filename);
                break;
            case 9:
                load_from_file(&books, &count, filename);
                break;
            case 10:
                printf("Введите год издания: ");
                int year = validate_int_input("", 1000, 2100);
                get_books_by_year(books, count, year);
                break;
            case 11:
                create_index(index_name);
                index_books(index_name, books, count);
                break;
            case 12:
                search_menu();
                int search_choice = validate_int_input("", 1, 7);

                if (search_choice == 1) {
                    // Нечёткий поиск по названию
                    validate_string_input("Введите значение для нечёткого поиска: ", search_value, 100);
                    search_books(index_name, "title", search_value, 0, 0, 0);
                } else if (search_choice == 2) {
                    // Нечёткий поиск по автору
                    validate_string_input("Введите значение для нечёткого поиска: ", search_value, 100);
                    search_books(index_name, "author", search_value, 0, 0, 0);
                } else if (search_choice == 3) {
                    // Нечёткий поиск по ISBN
                    validate_string_input("Введите значение для нечёткого поиска: ", search_value, 100);
                    search_books(index_name, "isbn", search_value, 0, 0, 0);
                } else if (search_choice == 5) {
                    // Нечёткий поиск по жанру
                    validate_string_input("Введите значение для нечёткого поиска: ", search_value, 100);
                    search_books(index_name, "genre", search_value, 0, 0, 0);
                } else {
                    search_choice_menu();
                    search_type = validate_int_input("", 1, 2);

                    if (search_type == 1) {
                        printf("Введите минимальное значение для поиска: ");
                        min_value = validate_int_input("", 0, 10000);
                        printf("Введите максимальное значение для поиска: ");
                        max_value = validate_int_input("", min_value, 10000);
                    } else {
                        validate_string_input("Введите значение для нечёткого поиска: ", search_value, 100);
                    }

                    if (search_choice == 4) {
                        if (search_type == 1) {
                            search_books(index_name, "publication_year", "", 1, min_value, max_value);
                        } else {
                            search_books(index_name, "publication_year", search_value, 0, 0, 0);
                        }
                    } else if (search_choice == 6) {
                        if (search_type == 1) {
                            search_books(index_name, "pages", "", 1, min_value, max_value);
                        } else {
                            search_books(index_name, "pages", search_value, 0, 0, 0);
                        }
                    } else if (search_choice == 7) {
                        if (search_type == 1) {
                            search_books(index_name, "rating", "", 1, min_value, max_value);
                        } else {
                            search_books(index_name, "rating", search_value, 0, 0, 0);
                        }
                    }
                }
                break;
            case 13:
                delete_index(index_name);
                break;
            case 14:
                printf("Выход из программы.\n");
                free(books);
                return 0;
            default:
                printf("Неверный выбор! Попробуйте снова.\n");
        }
    }
    return 0;
}
