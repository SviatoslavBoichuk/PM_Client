#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
    ui(new Ui::MainWindow), m_filePath(""), m_pFileHandle(nullptr)
{
    ui->setupUi(this);
    ui->menuBar->setNativeMenuBar(false);
    ui->statusBar->showMessage("Ready!", m_ciStatusMessageTimeOut);

    setUpComboBox();

    ui->search_line->setPlaceholderText(m_cSearchPlaceholderText);
    ui->message_line->setPlaceholderText(m_cMessagePlaceholderText);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::checkFileSize()
{
    return ( m_pFileHandle->size() <= g_clMaxFileZie ) ? true : false;
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}

void MainWindow::on_attach_file_button_clicked()
{
    m_filePath = QFileDialog::getOpenFileName(this, "Open file", "", m_cFileTypeFilter);

    if ( !m_filePath.isEmpty())
    {
        m_pFileHandle = new QFile(m_filePath);

        if ( !m_pFileHandle->open(QIODevice::ReadOnly) )
        {
            QMessageBox::information(this, "Can't open file", m_filePath);
        }
        else
        {
            if ( checkFileSize() )
            {
                // TODO: if file size ok - then send to client!

                //QMessageBox::information(this, "OK!" , "File size ok!");
            }
        }
    }
    else
    {
        QMessageBox::warning(this, "File not opened!", "No such file or directory!");
    }
}

void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    qDebug() << index;
    //work ok!
    //QMessageBox::information(this, "info", QString::number(index));
}

void MainWindow::on_send_button_clicked()
{
    CClientNetwork net(g_cDefaultIp, g_ciDefaultPort);
    if ( !net.connectToServer() )
    {
        QMessageBox::information(this, "Error!", "no connection!");
    }
    else
    {
        //QByteArray buff =

        //net.send();
    }
}

void MainWindow::setUpComboBox()
{
    ui->comboBox->insertItem(0, QIcon(":/statusIcons/online.ico"),  "online");
    ui->comboBox->insertItem(1, QIcon(":/statusIcons/offline.ico"), "offline");
    ui->comboBox->insertItem(2, QIcon(":/statusIcons/busy.ico"),    "busy");
}
