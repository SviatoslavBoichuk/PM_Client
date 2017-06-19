#ifndef CREGISTARTION_H
#define CREGISTARTION_H

#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>
#include "cclientnetwork.h"
#include "cuser.h"

namespace Ui {
class CRegistartion;
}

class CRegistartion : public QDialog
{
    Q_OBJECT

public:
    explicit CRegistartion(CClientNetwork * network, QWidget *parent = 0);
    ~CRegistartion();

private:
    bool userNameFieldEmpty();
    bool firstNameFiledEmpty();
    bool secondNameFiledEmpty();
    bool passFiledEmpty();
    bool emailFiledEmpty();
    void readAllFields();
    void setUpEchoMode(bool setEcho);
    void showWarningMessage(const QString& message);
    void copyData(register_st& reg);
    void checkErrors(QString& warningMessage);
    void sendRegRequest();

private slots:
    void on_accept_button_clicked();

    void on_checkBox_clicked();

    void on_openIcon_clicked();

    void on_cancel_button_clicked();

private:
    Ui::CRegistartion *ui;

    CUser * m_pUser;
    CClientNetwork * m_pNet;
};

#endif // CREGISTARTION_H
