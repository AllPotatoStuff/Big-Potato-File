cmake_minimum_required(VERSION 3.20)
project(BigPotatoFile)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
    sqlite3
    URL https://www.sqlite.org/2024/sqlite-amalgamation-3450100.zip
)
FetchContent_MakeAvailable(sqlite3)

add_library(sqlite3 STATIC
    ${sqlite3_SOURCE_DIR}/sqlite3.c
)
target_include_directories(sqlite3 PUBLIC ${sqlite3_SOURCE_DIR})

add_executable(bigpotatofile
    main.cpp
    src/database/database.cpp
    src/filesystem/scanner.cpp
    src/index/indexer.cpp
    src/threadpool/taskqueue.cpp
    src/threadpool/threadpool.cpp
    src/argumentparser.cpp
)

target_include_directories(bigpotatofile PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(bigpotatofile PRIVATE sqlite3)