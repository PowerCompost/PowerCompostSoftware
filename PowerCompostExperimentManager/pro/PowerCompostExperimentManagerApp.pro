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

SOURCES     += ../app/PowerCompostExperimentManagerApp.cpp\
               ../src/MainWindow.cpp

HEADERS     += ../PowerCompostExperimentManager/MainWindow.h

#-------------------------------------------------
