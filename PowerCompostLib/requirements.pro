#=================================================
#
# SqlWidget Project file 
#
#=================================================

#-------------------------------------------------
# Qt dependencies
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#-------------------------------------------------

#-------------------------------------------------
# SqlWidget
#-------------------------------------------------

# Type 
TEMPLATE     = lib

# Target name
TARGET       = SqlWidget

# Target dir
DESTDIR      = lib/SqlWidget/

# Target directories for moc and object files
MOC_DIR      = tmp/moc/
OBJECTS_DIR  = tmp/o/

# Defines
DEFINES     += SQLWIDGET

SOURCES     += src/SqlWidget.cpp

HEADERS     += PowerCompostLib/SqlWidget.h

target.path  = ../installed/lib/SqlWidget
INSTALLS += target
#headers.path = ../installed/include/SqlWidget
headers.path = ../installed/lib/SqlWidget
headers.files += $$HEADERS
INSTALLS += headers

#-------------------------------------------------
