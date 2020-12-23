TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt



TARGET=getInfoAllSC
LIBS+=-lpthread
LIBS+=-lmysqlclient

include(deployment.pri)
qtcAddDeployment()



HEADERS += \
    main.h \
    plib/tinyxml.h \
    plib/tinystr.h \
    plib/pxml.h \
    plib/pmysql.h \
    plib/plib.h \
    plib/clib.h

SOURCES += \
    main.cpp \
    plib/tinyxmlparser.cpp \
    plib/tinyxmlerror.cpp \
    plib/tinyxml.cpp \
    plib/tinystr.cpp \
    plib/pxml.cpp \
    plib/pmysql.cpp \
    plib/plib.cpp \
    plib/clib.c

