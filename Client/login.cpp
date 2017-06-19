#include "login.h"
#include "ui_login.h"
#include "cregistartion.h"
#include "mainwindow.h"

#include <QDebug>
#include <string>

login::login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::login),
    m_pNet(nullptr)
{
    ui->setupUi(this);

    ui->login_line->setPlaceholderText(m_cLoginPlaceholder);
    ui->pass_line->setPlaceholderText(m_cPasswPlacegolder);
    ui->pass_line->setEchoMode(QLineEdit::Password);
    ui->login_line->setValidator( new QRegExpValidator(QRegExp("[A-Za-z0-9]{4,32}"),this) );
    ui->pass_line->setValidator ( new QRegExpValidator(QRegExp("[A-Za-z0-9]{4,32}"),this) );

    ui->login_button->setEnabled(false);

    m_rememberPass = false;
    m_validLoginLen = false;
    m_validPasswLen = false;

    m_pNet = new CClientNetwork(g_cDefaultIp, g_ciServerPort);
    if ( !m_pNet->connectToServer() )
    {
        QMessageBox::warning(this, "Warning!", "Can't connect to server!");
    }
}

login::~login()
{
    delete ui;
}

void login::on_checkBox_clicked()
{
    m_rememberPass = ui->checkBox->isChecked();
}

void login::on_login_button_clicked()
{
    if (ui->login_line->text().isEmpty() || ui->pass_line->text().isEmpty())
    {
        QMessageBox::warning(this, "Empty fileds!", "Please fill all fileld!");
    }
    else
    {
        m_login = ui->login_line->text();
        m_pass  = ui->pass_line->text();

        char code = 6;
        m_pNet->Send((const char*)&code,1);

        auth_st user;

        memset(user.username, 0, BUFFER_SIZE);
        memset(user.password, 0, BUFFER_SIZE);

        memcpy(user.username, m_login.toUtf8().data(), 1024);
        memcpy(user.password, m_pass.toUtf8().data(), 1024);

        char tmp[BUFFER_SIZE*2];
        memset(tmp, 0, BUFFER_SIZE*2);

        memcpy(tmp, (const void*)&user, BUFFER_SIZE*2);
        m_pNet->Send(tmp, BUFFER_SIZE*2);
        char answerCode = -1;

        m_pNet->NonBlockingRecv( (char*)&answerCode, 1);

        if( answerCode == SERVER_OK )
        {
            this->hide();

            MainWindow * mw = new MainWindow(this);
            mw->show();

            this->show();
        }
        else
        {
            QMessageBox::warning(this, "Connection failed!", "Server don't receive!");
        }
    }
}

void login::on_regist_button_clicked()
{
    CRegistartion * regWindow = new CRegistartion(m_pNet, this);

    this->hide();

    regWindow->exec();

    this->show();
}

void login::on_login_line_textChanged(const QString &arg1)
{
    m_validLoginLen = arg1.length() < m_ciMinLogPassLen ? false : true;

    ( m_validLoginLen && m_validPasswLen ) ? ui->login_button->setEnabled(true) :
                                             ui->login_button->setEnabled(false);
}

void login::on_pass_line_textChanged(const QString &arg1)
{
    m_validPasswLen = arg1.length() < m_ciMinLogPassLen ? false : true;

    ( m_validLoginLen && m_validPasswLen ) ? ui->login_button->setEnabled(true) :
                                             ui->login_button->setEnabled(false);
}

void login::on_close_button_clicked()
{
    this->close();
}
