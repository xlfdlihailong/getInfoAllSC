cmake_minimum_required(VERSION 3.16)
project(plib)

set(CMAKE_CXX_STANDARD 11)

add_executable(plib
        clib.c
        clib.h
        mainplib.cpp
        pudp.h
        pudp.cpp
        plib.cpp
        plib.h
        pmysql.cpp
        pmysql.h
        pxml.cpp
        pxml.h
        tinystr.cpp
        tinystr.h
        tinyxml.cpp
        tinyxml.h
        tinyxmlerror.cpp
        tinyxmlparser.cpp ptrans.cpp ptrans.h)
target_link_libraries(${PROJECT_NAME} -lpthread)
#target_link_libraries(${PROJECT_NAME} /usr/lib64/mysql/libmysqlclient.a)
target_link_libraries(${PROJECT_NAME} -lmysqlclient)