#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <json-c/json.h>
#include "Combined_headers.h"

#define ELASTIC_URL "http://localhost:9200"

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    strncat(userp, contents, realsize);
    return realsize;
}

int index_exists(const char *index_name) {
    CURL *curl;
    CURLcode res;
    int exists = 0;

    curl = curl_easy_init();
    if (curl) {
        char url[256];
        snprintf(url, sizeof(url), "%s/%s", ELASTIC_URL, index_name);

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);

        res = curl_easy_perform(curl);
        if (res == CURLE_OK) {
            long response_code;
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
            if (response_code == 200) {
                exists = 1;
            }
        }

        curl_easy_cleanup(curl);
    }

    return exists;
}

void delete_index(const char *index_name) {
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    if (curl) {
        char url[256];
        snprintf(url, sizeof(url), "%s/%s", ELASTIC_URL, index_name);

        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
        curl_easy_setopt(curl, CURLOPT_URL, url);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }

        curl_easy_cleanup(curl);
    }
}

void create_index(const char *index_name) {
    if (index_exists(index_name)) {
        printf("Индекс уже существует: %s\n", index_name);
        delete_index(index_name); // Удаление существующего индекса
    }

    CURL *curl;
    CURLcode res;
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    
    if(curl) {
        const char *json_data = "{"
                                "\"settings\": {"
                                    "\"index\": {"
                                        "\"max_ngram_diff\": 19"
                                    "},"
                                    "\"analysis\": {"
                                        "\"tokenizer\": {"
                                            "\"ngram_tokenizer\": {"
                                                "\"type\": \"ngram\","
                                                "\"min_gram\": 1,"
                                                "\"max_gram\": 20,"
                                                "\"token_chars\": ["
                                                    "\"letter\","
                                                    "\"digit\""
                                                "]"
                                            "}"
                                        "},"
                                        "\"filter\": {"
                                            "\"russian_stemmer\": {"
                                                "\"type\": \"stemmer\","
                                                "\"language\": \"russian\""
                                            "}"
                                        "},"
                                        "\"analyzer\": {"
                                            "\"partial_match_analyzer\": {"
                                                "\"type\": \"custom\","
                                                "\"tokenizer\": \"ngram_tokenizer\","
                                                "\"filter\": ["
                                                    "\"lowercase\","
                                                    "\"russian_stemmer\""
                                                "]"
                                            "}"
                                        "}"
                                    "}"
                                "},"
                                "\"mappings\": {"
                                    "\"properties\": {"
                                        "\"title\": { \"type\": \"text\", \"analyzer\": \"partial_match_analyzer\", \"search_analyzer\": \"standard\" },"
                                        "\"title_str\": { \"type\": \"text\" },"
                                        "\"author\": { \"type\": \"text\", \"analyzer\": \"partial_match_analyzer\", \"search_analyzer\": \"standard\" },"
                                        "\"author_str\": { \"type\": \"text\" },"
                                        "\"isbn\": { \"type\": \"text\", \"analyzer\": \"partial_match_analyzer\", \"search_analyzer\": \"standard\" },"
                                        "\"isbn_str\": { \"type\": \"text\" },"
                                        "\"publication_year\": { \"type\": \"integer\" },"
                                        "\"publication_year_str\": { \"type\": \"text\", \"analyzer\": \"partial_match_analyzer\", \"search_analyzer\": \"standard\" },"
                                        "\"genre\": { \"type\": \"text\", \"analyzer\": \"partial_match_analyzer\", \"search_analyzer\": \"standard\" },"
                                        "\"genre_str\": { \"type\": \"text\" },"
                                        "\"pages\": { \"type\": \"integer\" },"
                                        "\"pages_str\": { \"type\": \"text\" },"
                                        "\"rating\": { \"type\": \"float\" },"
                                        "\"rating_str\": { \"type\": \"text\" }"
                                    "}"
                                "}"
                                "}";

        char url[256];
        snprintf(url, sizeof(url), "%s/%s", ELASTIC_URL, index_name);
        
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_data);
        
        res = curl_easy_perform(curl);
        
        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }
        
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }
    
    curl_global_cleanup();
}



void index_books(const char *index_name, Book *books, int count) {
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    if(curl) {
        for(int i = 0; i < count; i++) {
            char url[256];
            snprintf(url, sizeof(url), "%s/%s/_doc/%d", ELASTIC_URL, index_name, i + 1);
            char data[2048];
            if (books[i].details.pages > 0) {
                snprintf(data, sizeof(data), "{\"title\": \"%s\", \"title_str\": \"%s\", \"author\": \"%s\", \"author_str\": \"%s\", \"isbn\": \"%s\", \"isbn_str\": \"%s\", \"publication_year\": %d, \"publication_year_str\": \"%d\", \"genre\": \"%s\", \"genre_str\": \"%s\", \"pages\": %d, \"pages_str\": \"%d\"}",
                         books[i].title, books[i].title, books[i].author, books[i].author, books[i].isbn, books[i].isbn, books[i].publication_year, books[i].publication_year, books[i].genre, books[i].genre, books[i].details.pages, books[i].details.pages);
            } else {
                snprintf(data, sizeof(data), "{\"title\": \"%s\", \"title_str\": \"%s\", \"author\": \"%s\", \"author_str\": \"%s\", \"isbn\": \"%s\", \"isbn_str\": \"%s\", \"publication_year\": %d, \"publication_year_str\": \"%d\", \"genre\": \"%s\", \"genre_str\": \"%s\", \"rating\": %.2f, \"rating_str\": \"%.2f\"}",
                         books[i].title, books[i].title, books[i].author, books[i].author, books[i].isbn, books[i].isbn, books[i].publication_year, books[i].publication_year, books[i].genre, books[i].genre, books[i].details.rating, books[i].details.rating);
            }

            curl_easy_setopt(curl, CURLOPT_URL, url);
            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
            curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(data));
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, curl_slist_append(NULL, "Content-Type: application/json"));

            res = curl_easy_perform(curl);
            if(res != CURLE_OK) {
                fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            }
        }

        curl_easy_cleanup(curl);
    }
}

void search_books(const char *index_name, const char *field, const char *value, int is_range, long min_value, long max_value) {
    CURL *curl;
    CURLcode res;
    char response[10240] = {0}; // Буфер для хранения ответа

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();

    if(curl) {
        char json_data[2048];
        size_t value_length = strlen(value);

        if (is_range) {
            snprintf(json_data, sizeof(json_data),
                     "{\"query\": { \"range\": { \"%s\": { \"gte\": \"%ld\", \"lte\": \"%ld\" }}}}", field, min_value, max_value);
        } else {
            // Используем строковые представления полей для нечётких запросов
            char field_str[64];
            if (strcmp(field, "title") == 0 || strcmp(field, "author") == 0 || strcmp(field, "isbn") == 0 ||
                strcmp(field, "publication_year") == 0 || strcmp(field, "genre") == 0 ||
                strcmp(field, "pages") == 0 || strcmp(field, "rating") == 0) {
                snprintf(field_str, sizeof(field_str), "%s_str", field);
            } else {
                strcpy(field_str, field);
            }

            if (value_length <= 2) {
                // Поиск всех совпадений при вводе одного или двух символов
                snprintf(json_data, sizeof(json_data),
                         "{\"query\": { \"wildcard\": { \"%s\": \"*%s*\" }}}", field_str, value);
            } else {
                // Используем комбинацию фазинесс и n-грамм для поиска одного наилучшего совпадения при вводе более двух символов
                snprintf(json_data, sizeof(json_data),
                         "{\"query\": { \"bool\": { \"should\": [ "
                         "{\"fuzzy\": { \"%s\": { \"value\": \"%s\", \"fuzziness\": \"AUTO\" }}}, "
                         "{\"match\": { \"%s\": { \"query\": \"%s\", \"analyzer\": \"partial_match_analyzer\" }}}, "
                         "{\"wildcard\": { \"%s\": \"*%s*\" }} "
                         "] }}, \"size\": 1}", field_str, value, field_str, value, field_str, value);
            }
        }

        char url[256];
        snprintf(url, sizeof(url), "%s/%s/_search", ELASTIC_URL, index_name);
        
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_data);
        
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, response);
        
        res = curl_easy_perform(curl);
        
        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        } else {
            // Форматируем и выводим результаты
            struct json_object *parsed_json;
            struct json_object *hits;
            struct json_object *hit;
            struct json_object *source;
            struct json_object *item;

            parsed_json = json_tokener_parse(response);
            json_object_object_get_ex(parsed_json, "hits", &hits);
            json_object_object_get_ex(hits, "hits", &hits);

            if (json_object_get_type(hits) == json_type_array && json_object_array_length(hits) > 0) {
                if (value_length <= 2) {
                    // Вывод всех совпадений для одного или двух символов
                    for (size_t i = 0; i < json_object_array_length(hits); i++) {
                        hit = json_object_array_get_idx(hits, i);
                        json_object_object_get_ex(hit, "_source", &source);

                        printf("\nРезультат %lu:\n", i + 1);

                        json_object_object_get_ex(source, "title", &item);
                        printf("Название: %s\n", json_object_get_string(item));

                        json_object_object_get_ex(source, "author", &item);
                        printf("Автор: %s\n", json_object_get_string(item));

                        json_object_object_get_ex(source, "isbn", &item);
                        printf("ISBN: %s\n", json_object_get_string(item));

                        json_object_object_get_ex(source, "publication_year", &item);
                        printf("Год издания: %d\n", json_object_get_int(item));

                        json_object_object_get_ex(source, "genre", &item);
                        printf("Жанр: %s\n", json_object_get_string(item));

                        json_object_object_get_ex(source, "pages", &item);
                        if (item) {
                            printf("Количество страниц: %d\n", json_object_get_int(item));
                        }

                        json_object_object_get_ex(source, "rating", &item);
                        if (item) {
                            printf("Рейтинг: %.2f\n", json_object_get_double(item));
                        }
                    }
                } else {
                    // Вывод только одного наилучшего совпадения для более длинных запросов
                    hit = json_object_array_get_idx(hits, 0); // Получаем первый элемент массива
                    json_object_object_get_ex(hit, "_source", &source);

                    printf("\nСамый релевантный результат:\n");

                    json_object_object_get_ex(source, "title", &item);
                    printf("Название: %s\n", json_object_get_string(item));

                    json_object_object_get_ex(source, "author", &item);
                    printf("Автор: %s\n", json_object_get_string(item));

                    json_object_object_get_ex(source, "isbn", &item);
                    printf("ISBN: %s\n", json_object_get_string(item));

                    json_object_object_get_ex(source, "publication_year", &item);
                    printf("Год издания: %d\n", json_object_get_int(item));

                    json_object_object_get_ex(source, "genre", &item);
                    printf("Жанр: %s\n", json_object_get_string(item));

                    json_object_object_get_ex(source, "pages", &item);
                    if (item) {
                        printf("Количество страниц: %d\n", json_object_get_int(item));
                    }

                    json_object_object_get_ex(source, "rating", &item);
                    if (item) {
                        printf("Рейтинг: %.2f\n", json_object_get_double(item));
                    }
                }
            } else {
                printf("Совпадений не найдено.\n");
            }

            json_object_put(parsed_json); // Освобождаем память
        }
        
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
}


void search_books_range(const char *index_name, const char *field, long min_value, long max_value) {
    search_books(index_name, field, "", 1, min_value, max_value);
}



void format_and_print_search_results(const char *response) {
    struct json_object *parsed_json;
    struct json_object *hits;
    struct json_object *total;
    struct json_object *hit;
    struct json_object *source;
    struct json_object *item;
    size_t n_hits;

    parsed_json = json_tokener_parse(response);
    json_object_object_get_ex(parsed_json, "hits", &hits);
    json_object_object_get_ex(hits, "total", &total);
    printf("Общее количество совпадений: %d\n", json_object_get_int(json_object_object_get(total, "value")));

    json_object_object_get_ex(hits, "hits", &hits);
    if (json_object_get_type(hits) == json_type_array) {
        n_hits = json_object_array_length(hits);

        for (size_t i = 0; i < n_hits; i++) {
            hit = json_object_array_get_idx(hits, i);
            json_object_object_get_ex(hit, "_source", &source);

            printf("\nСовпадение %lu:\n", i + 1);

            json_object_object_get_ex(source, "title", &item);
            printf("Название: %s\n", json_object_get_string(item));

            json_object_object_get_ex(source, "author", &item);
            printf("Автор: %s\n", json_object_get_string(item));

            json_object_object_get_ex(source, "isbn", &item);
            printf("ISBN: %s\n", json_object_get_string(item));

            json_object_object_get_ex(source, "publication_year", &item);
            printf("Год издания: %d\n", json_object_get_int(item));

            json_object_object_get_ex(source, "genre", &item);
            printf("Жанр: %s\n", json_object_get_string(item));

            json_object_object_get_ex(source, "pages", &item);
            if (item) {
                printf("Количество страниц: %d\n", json_object_get_int(item));
            }

            json_object_object_get_ex(source, "rating", &item);
            if (item) {
                printf("Рейтинг: %.2f\n", json_object_get_double(item));
            }
        }
    } else {
        printf("Совпадений не найдено.\n");
    }

    json_object_put(parsed_json); // Освобождаем память
}
