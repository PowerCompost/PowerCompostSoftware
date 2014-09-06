#=================================================
#
# PowerCompostLabApp Project file 
#
#=================================================

#-------------------------------------------------
# Qt dependencies
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#-------------------------------------------------

#-------------------------------------------------
# PowerCompostLabApp
#-------------------------------------------------

# Type 
TEMPLATE     = app

# Target name
TARGET       = PowerCompostLab

# Target dir
DESTDIR      = ../bin/

# Target directories for moc and object files
MOC_DIR      = ../tmp/moc/
OBJECTS_DIR  = ../tmp/o/

# Where to look for included files in sources and headers
INCLUDEPATH += ..
#INCLUDEPATH += ../lib

# Libraries
#LIBS += -L../lib -lSqlWidget

# Sources
SOURCES     += ../app/PowerCompostLabApp.cpp\
               ../src/MainWindow.cpp \
               ../src/SqlWidget.cpp

HEADERS     += ../PowerCompostLab/MainWindow.h \
               ../PowerCompostLab/SqlWidget.h

#-------------------------------------------------