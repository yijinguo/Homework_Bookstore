cmake_minimum_required(VERSION 3.16)
project(untitled3)

set(CMAKE_CXX_STANDARD 14)

add_executable(code Bookstore/main.cpp
        Bookstore/parser.h
        Bookstore/statement.h
        Bookstore/statement.cpp
        Bookstore/system/account.h
        Bookstore/system/account.cpp
        Bookstore/system/books.h
        Bookstore/system/book.cpp
        Bookstore/system/diary.h
        Bookstore/system/diary.cpp
        Bookstore/FileDataBase/FileData.h
        Bookstore/error.h)
