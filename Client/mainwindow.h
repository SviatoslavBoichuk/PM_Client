#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <thread>

#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QMainWindow>
#include <QMessageBox>
#include <QListWidgetItem>
#include <QString>

#include <stdio.h>

#include "cclientnetwork.h"
#include "cuser.h"
#include "cusersmessages.h"


const long g_clMaxFileSize = 5242880l;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(CClientNetwork* pNet, const user_st& user_info,int count,
                        const user_st * p_contacts, QWidget *parent = 0);
    ~MainWindow();

private:
    bool checkFileSize();
    void showContactList();
    void setUpComboBox();
    void setUserInfo(const user_st& info);
    QListWidgetItem makeItem(const user_st& user);
    void pushItem(const QListWidgetItem& item);

    void GetContactList(user_st ** contacts,
                        server_response_st& response, int* count);

    void MessageHandler();

    static bool GetCondition();

    const QString GetUserNameById(int id);

private slots:

    void on_actionExit_triggered();
    void on_attach_file_button_clicked();

    void on_send_button_clicked();

    void on_search_new_button_clicked();
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_disp_msg_itemClicked(QListWidgetItem *item);

    void on_contactList_itemClicked(QListWidgetItem *item);

private:
    Ui::MainWindow *ui;
    const int m_ciStatusMessageTimeOut      = 1000;
    const QString m_cSearchPlaceholderText  = "Search";
    const QString m_cMessagePlaceholderText = "Type message here...";
    const QString m_cFileTypeFilter         = "*.*";

    QVector<user_st> contacts;

    CUser       userInfo;
    QString     m_filePath;
    QFile * m_pFileHandle;
    CClientNetwork * m_pNet;

    CUsersMessages m_msgMap;

    int m_iUserIndex;

    std::thread* backgroundThread;
    QVector<QListWidgetItem> itemsList;
};

#endif // MAINWINDOW_H
