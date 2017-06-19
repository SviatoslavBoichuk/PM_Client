#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFile>
#include <QMainWindow>
#include <QString>

#include "cclientnetwork.h"

const long g_clMaxFileZie = 5242880l;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    bool checkFileSize();

    void setUpComboBox();

private slots:

    void on_actionExit_triggered();
    void on_attach_file_button_clicked();

    void on_comboBox_currentIndexChanged(int index);
    void on_send_button_clicked();

private:
    Ui::MainWindow *ui;
    const int m_ciStatusMessageTimeOut      = 1000;
    const QString m_cSearchPlaceholderText  = "Search";
    const QString m_cMessagePlaceholderText = "Type message here...";
    const QString m_cFileTypeFilter         = "*.jpg *.jpeg *.png";

    //State eUserState;
    QString m_filePath;
    QFile * m_pFileHandle;
};

#endif // MAINWINDOW_H
