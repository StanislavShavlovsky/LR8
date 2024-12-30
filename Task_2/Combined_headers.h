#ifndef COMBINED_HEADERS_H
#define COMBINED_HEADERS_H

#include "book_struct.h"
#include <stddef.h>
#include <string.h>

// Функции для работы с Elasticsearch


int index_exists(const char *index_name);
void delete_index(const char *index_name);
void create_index(const char *index_name);
void index_books(const char *index_name, Book *books, int count);
void search_books(const char *index_name, const char *field, const char *value, int is_range, long min_value, long max_value);
void search_books_range(const char *index_name, const char *field, long min_value, long max_value);
void format_and_print_search_results(const char *response);









void menu();
void create_array(Book **books, int *count);
void view_array(const Book *books, int count);
void add_book(Book **books, int *count);
void search_book(const Book *books, int count, const char *search_value);
void delete_book(Book **books, int *count, const char *title);
void modify_book(Book *books, int count, const char *title);
void sort_books(Book *books, int count);
void save_to_file(const Book *books, int count, const char *filename);
void load_from_file(Book **books, int *count, const char *filename);
void get_books_by_year(const Book *books, int count, int year);
int validate_int_input(const char *prompt, int min, int max);
void validate_string_input(const char *prompt, char *input, size_t length);
float validate_float_input(const char *prompt);





#endif // COMBINED_HEADERS_H
