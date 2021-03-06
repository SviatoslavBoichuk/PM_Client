#ifndef LOGIN_H
#define LOGIN_H

#include <string>

#include <QDebug>
#include <QDialog>
#include <QMessageBox>

#include "cclientnetwork.h"
#include "cregistartion.h"
#include "mainwindow.h"

namespace Ui {
class login;
}

class login : public QDialog
{
    Q_OBJECT

public:
    explicit login(QWidget *parent = 0);
    ~login();

private:
    void UiSetUp();
    void ReadField();
    void SendAuth(auth_st user);
    void GetUserInfo(user_st& user_data);
    void GetContactList(user_st ** contacts, server_response_st &response, int * count);

private slots:
    void on_checkBox_clicked();

    void on_login_button_clicked();

    void on_regist_button_clicked();

    void on_login_line_textChanged(const QString &arg1);

    void on_pass_line_textChanged(const QString &arg1);

    void on_close_button_clicked();

private:
    Ui::login *ui;

    const QString m_cLoginPlaceholder = "Login";
    const QString m_cPasswPlacegolder = "Password";
    const int m_ciMinLogPassLen = 4;

    QString m_login;
    QString m_pass;
    CClientNetwork * m_pNet;

    bool m_rememberPass;
    bool m_validLoginLen;
    bool m_validPasswLen;
};

#endif // LOGIN_H
