#ifndef COMBINED_HEADERS_H
#define COMBINED_HEADERS_H

#include "product.h"
#include <stddef.h>

// Функции для работы с Elasticsearch
int index_exists(const char *index_name);
void delete_index(const char *index_name);
void create_index(const char *index_name);
void index_products(const char *index_name, Product *products, int count);
void search_products(const char *index_name, const char *field, const char *value, int is_range, long min_value, long max_value);
void format_and_print_search_results(const char *response);


// Функции для работы со структурой
void create_array(Product **products, int *count);
void fill_product(Product *products, int count);
void view_array(const Product *products, int count);
void add_product(Product **products, int *count);
void search_product(const Product *products, int count, const char *search_value);
void delete_product(Product **products, int *count, const char *name);
void modify_product(Product *products, int count, const char *name);
void save_to_file(const Product *products, int count, const char *filename);
void load_from_file(Product **products, int *count, const char *filename);
void get_quantity_by_workshop(const Product *products, int count, int workshop_number);
void shaker_sort(Product *products, int count);
int validate_date(const char *date);
int validate_int_input(const char *prompt, int min, int max);
void validate_string_input(const char *prompt, char *input, size_t max_length);
char validate_char_input(const char *prompt, const char *valid_chars);
float validate_float_input(const char *prompt);

#endif 
