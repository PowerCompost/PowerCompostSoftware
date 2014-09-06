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

# Sources
SOURCES     += src/SqlWidget.cpp

HEADERS     += PowerCompostLib/SqlWidget.h

# Install instructions
target.path  = ../installed/lib/SqlWidget
INSTALLS += target
headers.path = ../installed/include/SqlWidget
headers.files += $$HEADERS
INSTALLS += headers

#-------------------------------------------------
