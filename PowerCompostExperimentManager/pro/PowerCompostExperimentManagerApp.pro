#=================================================
#
# PowerCompostExperimentManagerApp Project file 
#
#=================================================

#-------------------------------------------------
# Qt dependencies
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#-------------------------------------------------

#-------------------------------------------------
# PowerCompostExperimentManagerApp
#-------------------------------------------------

# Type 
TEMPLATE     = app

# Target name
TARGET       = PowerCompostExperimentManager

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
SOURCES     += ../app/PowerCompostExperimentManagerApp.cpp\
               ../src/MainWindow.cpp \
               ../src/SqlWidget.cpp

HEADERS     += ../PowerCompostExperimentManager/MainWindow.h \
               ../PowerCompostExperimentManager/SqlWidget.h

#-------------------------------------------------
