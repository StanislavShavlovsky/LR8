cmake_minimum_required(VERSION 3.10)
project(BookManager)

# Устанавливаем стандарт C для компиляции
set(CMAKE_C_STANDARD 99)

# Добавляем исходные файлы
set(SOURCES
    main.c
    book.c
    elastic_search.c
)

# Добавляем заголовочные файлы
set(HEADERS
    Combined_headers.h
    book_struct.h
)

# Указываем директории для заголовочных файлов
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Создаем исполняемый файл
add_executable(BookManager ${SOURCES} ${HEADERS})

# Подключаем библиотеки
find_package(CURL REQUIRED)
find_package(json-c REQUIRED)

target_link_libraries(BookManager CURL::libcurl json-c)
