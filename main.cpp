#include "mainwindow.h"

#include <QApplication>

#include <QHBoxLayout>
#include <QListView>
#include <QTreeView>
#include <QFileSystemModel>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    w.show();
    return a.exec();
}
