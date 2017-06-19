#include "cregistartion.h"
#include "ui_cregistartion.h"
#include <QDebug>
#include <string.h>

CRegistartion::CRegistartion(CClientNetwork *network, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CRegistartion),
    m_pUser(nullptr)
{
    ui->setupUi(this);

    setUpEchoMode(true);
    m_pUser = new (std::nothrow) CUser;
    m_pNet = network;
}

CRegistartion::~CRegistartion()
{
    delete ui;
}

bool CRegistartion::userNameFieldEmpty()
{
    return ui->userName_line->text().isEmpty();
}

bool CRegistartion::secondNameFiledEmpty()
{
    return ui->secondName_line->text().isEmpty();
}

bool CRegistartion::firstNameFiledEmpty()
{
    return ui->firstName_line->text().isEmpty();
}

bool CRegistartion::passFiledEmpty()
{
    return (ui->userPass_line->text().isEmpty() ||
            ui->repeatPass_line->text().isEmpty());
}

bool CRegistartion::emailFiledEmpty()
{
    return ui->email_line->text().isEmpty();
}

void CRegistartion::readAllFields()
{
    m_pUser->SetUserName(ui->userName_line->text());
    m_pUser->SetUserPass(ui->userPass_line->text());
    m_pUser->SetUserMail(ui->email_line->text());
    m_pUser->SetFirstName(ui->firstName_line->text());
    m_pUser->SetSecondName(ui->secondName_line->text());
}

void CRegistartion::setUpEchoMode(bool setEcho)
{
    if ( setEcho )
    {
        ui->userPass_line->setEchoMode(QLineEdit::Password);
        ui->repeatPass_line->setEchoMode(QLineEdit::Password);
    }
    else
    {
        ui->userPass_line->setEchoMode(QLineEdit::Normal);
        ui->repeatPass_line->setEchoMode(QLineEdit::Normal);
    }
}

void CRegistartion::showWarningMessage(const QString &message)
{
    QMessageBox::warning(this, "Warning!", message);
}

void CRegistartion::copyData(register_st &reg)
{
    memset( (void*)&reg, 0, sizeof(reg));

    memcpy(reg.firstname,  m_pUser->getFirstName().toUtf8().data(), BUFFER_SIZE);
    memcpy(reg.secondname, m_pUser->getSecondName().toUtf8().data(),BUFFER_SIZE);
    memcpy(reg.username,   m_pUser->getUserName().toUtf8().data(),  BUFFER_SIZE);
    memcpy(reg.password,   m_pUser->getUserPass().toUtf8().data(),  BUFFER_SIZE);
    memcpy(reg.email,      m_pUser->getUserMail().toUtf8().data(),  BUFFER_SIZE);
}

void CRegistartion::checkErrors(QString &warningMessage)
{
    if ( userNameFieldEmpty() )
    {
        warningMessage.append("Enter your name!\n");
    }

    if( passFiledEmpty() )
    {
        warningMessage.append("Enter password!\n");
    }

    if( emailFiledEmpty() )
    {
        warningMessage.append("Enter your e-mail!\n");
    }

    if( firstNameFiledEmpty() || secondNameFiledEmpty() )
    {
        warningMessage.append("Enter your name and surname!\n");
    }

    if ( ui->repeatPass_line->text() != m_pUser->getUserPass() )
    {
        warningMessage.append("Passwords not equal!\n");
    }
}

void CRegistartion::sendRegRequest()
{
    register_st reg_st;
    copyData(reg_st);

    const int currentLen = BUFFER_SIZE * 5;

    char tmp[currentLen];
    memset(tmp, 0, currentLen);
    memcpy(tmp, (const void*)&reg_st, currentLen);
    m_pNet->Send(tmp, currentLen);
}

void CRegistartion::on_accept_button_clicked()
{
    bool regSuccess = false;

    readAllFields();

    QString warningMessage;

    checkErrors(warningMessage);

    if ( warningMessage.isEmpty() )
    {
        sendRegRequest();

        char answer = -1;
        m_pNet->NonBlockingRecv((char*)&answer, 1, 10);

        regSuccess = (answer == SERVER_OK) ? true : false;

        if ( regSuccess )
        {
            QMessageBox::information(this, "Info", "Account have been created!");
        }
    }
    else
    {
        showWarningMessage(warningMessage);
        regSuccess = false;
    }
}

void CRegistartion::on_checkBox_clicked()
{
    setUpEchoMode( !ui->checkBox->isChecked() );
}

void CRegistartion::on_openIcon_clicked()
{
    m_pUser->SetUserIcon( QFileDialog::getOpenFileName(this, "Open icon", "", "*.png, *.ico") );
    ui->userIcon_label->setPixmap( *m_pUser->getUserIcon() );
}

void CRegistartion::on_cancel_button_clicked()
{
    this->close();
}
