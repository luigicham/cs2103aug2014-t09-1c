#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint );
    ui->setupUi(this);
    connect(ui->lineEdit, SIGNAL(returnPressed()), ui->tableWidget, SLOT(createTableData()));
}

MainWindow::~MainWindow()
{
    delete ui;
}
