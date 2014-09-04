//----------------------------------------------------------------------------

// Specific header files

#include "PowerCompostLab/MainWindow.h"

//----------------------------------------------------------------------------

// Qt headers

#include <QApplication>

//----------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    // Creating QApplication
    QApplication application(argc, argv);
    
    // Creating MainWindow and show it
    MainWindow window;
    window.show();

    // Execute and return exit code
    return application.exec();
}
