TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
SOURCES += mainplib.cpp \
    plib.cpp \
    pmysql.cpp \
    pxml.cpp \
    tinystr.cpp \
    tinyxml.cpp \
    tinyxmlerror.cpp \
    tinyxmlparser.cpp \
    clib.c \
    pudp.cpp

include(deployment.pri)
qtcAddDeployment()

LIBS+=-lpthread
LIBS+=-lmysqlclient
HEADERS += \
    plib.h \
    clib.h \
    pmysql.h \
    pxml.h \
    tinystr.h \
    tinyxml.h \
    pudp.h

PRECOMPILED_HEADER =plib.h

