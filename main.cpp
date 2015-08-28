#include <QApplication>
#include "mp3player.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    mp3player w;
    if(w.mp3init()<0){
        return -1;
    }

    w.showFullScreen();
    
    return a.exec();
}
