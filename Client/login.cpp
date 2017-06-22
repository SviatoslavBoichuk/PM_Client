#include "login.h"
#include "ui_login.h"

login::login(QWidget *parent) :
    QDialog(parent), ui(new Ui::login),
    m_pNet(nullptr), m_rememberPass(false),
    m_validLoginLen(false), m_validPasswLen(false)
{
    ui->setupUi(this);
    this->setWindowTitle("LogIn");

    UiSetUp();

    //m_pNet = new CClientNetwork(g_cDefaultIp, g_ciServerPort);
    m_pNet = new CClientNetwork(g_cServerIp, g_ciServerPort);
    if ( !m_pNet->ConnectToServer() )
    {
        QMessageBox::warning(this, "Warning!", "Can't connect to server! Restart programm!");
    }
}

login::~login()
{
    delete ui;
}

void login::UiSetUp()
{
    ui->login_line->setPlaceholderText(m_cLoginPlaceholder);
    ui->pass_line->setPlaceholderText(m_cPasswPlacegolder);
    ui->pass_line->setEchoMode(QLineEdit::Password);
    ui->login_line->setValidator( new QRegExpValidator(QRegExp("[A-Za-z0-9]{4,32}"),this) );
    ui->pass_line->setValidator ( new QRegExpValidator(QRegExp("[A-Za-z0-9]{4,32}"),this) );
    ui->login_button->setEnabled(false);
}

void login::ReadField()
{
    m_login = ui->login_line->text();
    m_pass  = ui->pass_line->text();
}

void login::SendAuth(auth_st user)
{
    m_pNet->SendRequestCode(AUTH);

    memset( (void*)&user, 0, sizeof(user) );
    memcpy(user.username, m_login.toUtf8().data(), m_login.length());
    memcpy(user.password, m_pass.toUtf8().data(),  m_pass.length());

    const int currentLen = sizeof(user);

    char tmp[currentLen];
    memset(tmp, 0, currentLen);
    memcpy(tmp, (const void*)&user, currentLen);

    m_pNet->Send(tmp, currentLen);
}

void login::GetUserInfo(user_st& user_data)
{
    memset((void*)&user_data, 0, sizeof(user_data) );
    m_pNet->NonBlockingRecv( (char*)&user_data, sizeof(user_data) );
}

void login::GetContactList(user_st ** contacts, server_response_st& response, int* count)
{
    m_pNet->SendRequestCode(LISTCONTACT);

    memset( (void*)&response, 0, sizeof(response) );
    m_pNet->NonBlockingRecv( (char*)&response, sizeof(response) );

    if ( response.response_code == SERVER_OK )
    {
        m_pNet->NonBlockingRecv( (char*)count, sizeof(*count));
        int dataSize = sizeof(user_st) * (*count);
        *contacts = (user_st *)malloc(dataSize);

        m_pNet->NonBlockingRecv( ( (char*)((*contacts)) ), dataSize, 2);
    }
    else
    {
        QMessageBox::warning(this, "Warning!" ,QString(response.msg_buffer) );
    }
}

void login::on_checkBox_clicked()
{
    m_rememberPass = ui->checkBox->isChecked();
}

void login::on_login_button_clicked()
{
    if ( (!ui->login_line->text().isEmpty()) && (!ui->pass_line->text().isEmpty()) )
    {
        auth_st user;

        ReadField();
        SendAuth(user);

        server_response_st response;
        memset( (void*)&response, 0, sizeof(response) );
        m_pNet->NonBlockingRecv( (char*)&response, sizeof(response) );

        if( response.response_code == SERVER_OK )
        {
            int count = 0;
            user_st user_data;
            user_st * contacts = nullptr;

            GetUserInfo(user_data);
            GetContactList(&contacts, response, &count);

            /*Open main window*/
            this->hide();
            MainWindow * mw = new MainWindow(m_pNet, user_data,
                                             count, contacts,this);
            mw->show();
        }
        else
        {
            QMessageBox::warning(this, "Warning!" ,
                                 QString(response.msg_buffer) );
        }
    }
    else
    {
        QMessageBox::warning(this, "Empty fileds!",
                             "Please fill all fileld!");
    }
}

void login::on_regist_button_clicked()
{
    CRegistartion * regWindow = new CRegistartion(m_pNet, this);

    this->hide();

    regWindow->setFixedSize(420,340);
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
