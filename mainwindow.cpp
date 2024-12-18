#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("LasPlayerFiles");
    setFixedSize(size());


    pointsGLWidget = new PointsGlWidget(this);
    ui->verticalLayout->addWidget(pointsGLWidget);
    ui->radioButton->setChecked(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    pointsGLWidget->stopSignal(true);
    // Открытие диалога выбора директории
    QString selectedDirectory = QFileDialog::getExistingDirectory(this, tr("Select Directory"));

    if (selectedDirectory.isEmpty()) {
        qDebug() << "No directory selected";
        return;
    }

    // Получение списка всех файлов в директории
    QDir directory(selectedDirectory);
    QStringList files = directory.entryList(QDir::Files);

    // Сортировка файлов по имени
    files.sort(Qt::CaseInsensitive);

    // Обработка файлов
    ui->label->setText("Кол-во файлов: " + QString::number(files.size()));

    if (files.isEmpty()) {
        qDebug() << "No files found in the directory";
        return;
    }

    for (int i = 0; i < files.size(); i++) {
        if(!pointsGLWidget->getStopSignal()) break;

        QString filePath = directory.absolutePath() + "/" + files.at(i);

        // Преобразование QString к const char*
        const char* fileContent = filePath.toUtf8().constData();

        LasFileLoder* lasFile = new LasFileLoder(fileContent);

        pointsGLWidget->drawLasPoints(lasFile->getLasPoints());

        delete lasFile;
        QThread::msleep(17);
    }


}


void MainWindow::on_radioButton_clicked(bool checked)
{
    if (checked) {
        pointsGLWidget->switchViewMode(1);
    }
}


void MainWindow::on_radioButton_2_clicked(bool checked)
{
    if (checked) {
        pointsGLWidget->switchViewMode(2);
    }
}


void MainWindow::on_radioButton_3_clicked(bool checked)
{
    if (checked) {
        pointsGLWidget->switchViewMode(3);
    }
}


void MainWindow::on_radioButton_4_clicked(bool checked)
{
    if (checked) {
        pointsGLWidget->switchViewMode(4);
    }
}


void MainWindow::on_pushButton_2_clicked()
{
    pointsGLWidget->stopSignal(false);
}

