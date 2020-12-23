cmake_minimum_required(VERSION 3.16)
project(getInfoAllSC)

set(CMAKE_CXX_STANDARD 11)

add_executable(getInfoAllSC
        plib/clib.c
        plib/clib.h
        main.cpp
        plib/plib.cpp
        plib/plib.h
        plib/pmysql.cpp
        plib/pmysql.h
        plib/pxml.cpp
        plib/pxml.h
        plib/tinystr.cpp
        plib/tinystr.h
        plib/tinyxml.cpp
        plib/tinyxml.h
        plib/tinyxmlerror.cpp
        plib/tinyxmlparser.cpp main.h)
target_link_libraries(${PROJECT_NAME} -lpthread)
#target_link_libraries(${PROJECT_NAME} /usr/lib64/mysql/libmysqlclient.a)
target_link_libraries(${PROJECT_NAME} -lmysqlclient)