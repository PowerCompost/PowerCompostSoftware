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
DESTDIR      = bin/

# Target directories for moc and object files
MOC_DIR      = tmp/moc/
OBJECTS_DIR  = tmp/o/

# Libraries
unix|win32: LIBS += -L$$PWD/../installed/lib/SqlWidget/ -lSqlWidget

INCLUDEPATH += $$PWD/../installed/include/
DEPENDPATH  += $$PWD/../installed/lib/

# Sources
SOURCES     += app/PowerCompostExperimentManagerApp.cpp \
               src/MainWindow.cpp \
               src/TestBenchManagerWidget.cpp \
               src/ExperimentManagerWidget.cpp \
               src/SensorsManagerWidget.cpp \
               src/SensorWidget.cpp 

HEADERS     += PowerCompostExperimentManager/MainWindow.h \
               PowerCompostExperimentManager/TestBenchManagerWidget.h \
               PowerCompostExperimentManager/ExperimentManagerWidget.h \
               PowerCompostExperimentManager/SensorsManagerWidget.h \
               PowerCompostExperimentManager/SensorWidget.h

target.path=../installed/bin
INSTALLS += target

# Instructions to add and link properly PowerCompost libs
macx {
APP_DESTDIR = bin/$${TARGET}.app/Contents/MacOS
APP_DESTLIB = $${APP_DESTDIR}/../lib
QMAKE_POST_LINK += mkdir -p $${APP_DESTLIB} ;
QMAKE_POST_LINK += cp -r $$PWD/../installed/lib/SqlWidget $${APP_DESTLIB}/SqlWidget ;
QMAKE_POST_LINK += install_name_tool -change libSqlWidget.1.dylib @loader_path/../lib/SqlWidget/libSqlWidget.1.dylib $${APP_DESTDIR}/$${TARGET};
}
#-------------------------------------------------
