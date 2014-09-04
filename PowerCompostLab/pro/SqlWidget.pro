#=================================================
#
# SqlWidget Project file 
#
#=================================================

#-------------------------------------------------
# Qt dependencies
#-------------------------------------------------

#QT       += core gui sql

#greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#-------------------------------------------------

#-------------------------------------------------
# PowerCompostExperimentManagerApp
#-------------------------------------------------

# Type 
TEMPLATE     = lib
CONFIG       = staticlib

# Target name
TARGET       = libSqlWidget

# Target dir
DESTDIR      = ../lib/

# Target directories for moc and object files
MOC_DIR      = ../tmp/moc/
OBJECTS_DIR  = ../tmp/o/

# Where to look for included files in sources and headers
INCLUDEPATH += ..

SOURCES     += ../src/SqlWidget.cpp

HEADERS     += ../PowerCompostExperimentManager/SqlWidget.h

#-------------------------------------------------
