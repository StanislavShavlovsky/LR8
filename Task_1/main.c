#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "product.h"
#include "Combines_headers.h"

void menu()
{
    printf("Меню:\n");
    printf("1. Ввести новый массив продуктов\n");
    printf("2. Просмотреть все продукты\n");
    printf("3. Добавить новый продукт\n");
    printf("4. Поиск продукта по наименованию\n");
    printf("5. Удалить продукт по наименованию\n");
    printf("6. Изменить продукт по наименованию\n");
    printf("7. Сортировать продукты\n");
    printf("8. Сохранить в двоичный файл\n");
    printf("9. Загрузить из двоичного файла\n");
    printf("10. Вывести количество выпущенных изделий для заданного цеха\n");
    printf("11. Индексация продуктов в Elasticsearch\n");
    printf("12. Поиск продуктов в Elasticsearch\n");
    printf("13. Удалить индекс Elasticsearch\n");
    printf("14. Выход\n");
    printf("Введите ваш выбор: ");
}

void search_menu()
{
    printf("Меню поиска:\n");
    printf("1. Поиск по наименованию\n");
    printf("2. Поиск по количеству\n");
    printf("3. Поиск по номеру цеха\n");
    printf("4. Поиск по дате производства\n");
    printf("5. Поиск по типу\n");
    printf("6. Поиск по номеру партии\n");
    printf("7. Поиск по оценке качества\n");
    printf("Введите ваш выбор: ");
}

void search_choice_menu()
{
    printf("Тип поиска:\n");
    printf("1. Поиск по диапазону\n");
    printf("2. Нечёткий поиск\n");
    printf("Введите ваш выбор: ");
}

int main()
{
    Product *products = NULL;
    int count = 0;
    int choice, search_type;
    char search_value[50];
    long min_value, max_value;
    const char *filename = "products.bin";
    const char *index_name = "products_index";

    while (1)
    {
        menu();
        choice = validate_int_input("", 1, 14);

        switch (choice)
        {
        case 1:
            create_array(&products, &count);
            break;
        case 2:
            view_array(products, count);
            break;
        case 3:
            add_product(&products, &count);
            fill_product(products, count - 1);
            break;
        case 4:
            validate_string_input("Введите наименование для поиска: ", search_value, 50);
            search_product(products, count, search_value);
            break;
        case 5:
            validate_string_input("Введите наименование для удаления: ", search_value, 50);
            delete_product(&products, &count, search_value);
            break;
        case 6:
            validate_string_input("Введите наименование для изменения: ", search_value, 50);
            modify_product(products, count, search_value);
            break;
        case 7:
            shaker_sort(products, count);
            break;
        case 8:
            save_to_file(products, count, filename);
            break;
        case 9:
            load_from_file(&products, &count, filename);
            break;
        case 10:
            printf("Введите номер цеха: ");
            int workshop_number = validate_int_input("", 1, 1000);
            get_quantity_by_workshop(products, count, workshop_number);
            break;
        case 11:
            create_index(index_name);
            index_products(index_name, products, count);
            break;
        case 12:
            search_menu();
            int search_choice = validate_int_input("", 1, 7);

            if (search_choice == 1)
            {
                // Нечёткий поиск по наименованию
                validate_string_input("Введите значение для нечёткого поиска: ", search_value, 50);
                search_products(index_name, "name", search_value, 0, 0, 0);
            }
            else if (search_choice == 5)
            {
                // Нечёткий поиск по типу
                validate_string_input("Введите значение для нечёткого поиска: ", search_value, 50);
                search_products(index_name, "type", search_value, 0, 0, 0);
            }
            else
            {
                search_choice_menu();
                search_type = validate_int_input("", 1, 2);

                if (search_type == 1)
                {
                    printf("Введите минимальное значение для поиска: ");
                    min_value = validate_int_input("", 0, 1000000);
                    printf("Введите максимальное значение для поиска: ");
                    max_value = validate_int_input("", min_value, 1000000);
                }
                else
                {
                    validate_string_input("Введите значение для нечёткого поиска: ", search_value, 50);
                }

                if (search_choice == 2)
                {
                    if (search_type == 1)
                    {
                        search_products(index_name, "quantity", "", 1, min_value, max_value);
                    }
                    else
                    {
                        search_products(index_name, "quantity", search_value, 0, 0, 0);
                    }
                }
                else if (search_choice == 3)
                {
                    if (search_type == 1)
                    {
                        search_products(index_name, "workshop_number", "", 1, min_value, max_value);
                    }
                    else
                    {
                        search_products(index_name, "workshop_number", search_value, 0, 0, 0);
                    }
                }
                else if (search_choice == 4)
                {
                    if (search_type == 1)
                    {
                        search_products(index_name, "date_of_production", "", 1, min_value, max_value);
                    }
                    else
                    {
                        search_products(index_name, "date_of_production", search_value, 0, 0, 0);
                    }
                }
                else if (search_choice == 6)
                {
                    if (search_type == 1)
                    {
                        search_products(index_name, "details.batch_number", "", 1, min_value, max_value);
                    }
                    else
                    {
                        search_products(index_name, "details.batch_number", search_value, 0, 0, 0);
                    }
                }
                else if (search_choice == 7)
                {
                    if (search_type == 1)
                    {
                        search_products(index_name, "details.quality_score", "", 1, min_value, max_value);
                    }
                    else
                    {
                        search_products(index_name, "details.quality_score", search_value, 0, 0, 0);
                    }
                }
            }
            break;
        case 13:
            delete_index(index_name);
            break;
        case 14:
            printf("Выход из программы.\n");
            free(products);
            return 0;
        default:
            printf("Неверный выбор! Попробуйте снова.\n");
        }
    }
    return 0;
}
