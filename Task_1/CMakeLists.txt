cmake_minimum_required(VERSION 3.10)

# Название проекта
project(ProductsProject)

# Установка стандартов C
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

# Добавление исходных файлов проекта
add_executable(ProductsProject
    main.c
    functions.c
    elastic_search.c
)

# Указание директорий для поиска заголовочных файлов
target_include_directories(ProductsProject PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Добавление зависимостей для работы с libcurl и json-c
find_package(CURL REQUIRED)
find_package(json-c REQUIRED)

target_link_libraries(ProductsProject PRIVATE CURL::libcurl json-c)
