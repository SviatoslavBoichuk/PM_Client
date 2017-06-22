#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <mutex>
#include <condition_variable>
#include <QMimeDatabase>
#include <QDateTime>

bool g_bFlag = false;
std::condition_variable g_condVar;
std::mutex              g_mutex;

MainWindow::MainWindow(CClientNetwork *pNet, const user_st& user_info, int count,
                       const user_st * p_contacts, QWidget *parent) : QMainWindow(parent),
    ui(new Ui::MainWindow), m_filePath(""), m_pFileHandle(nullptr), m_pNet(pNet)
{
    ui->setupUi(this);
    ui->menuBar->setNativeMenuBar(false);

    m_iUserIndex = - 1;

    setUpComboBox();
    setUserInfo(user_info);

    for(int i = 0; i < count; i++)
    {
        pushItem(makeItem(p_contacts[i]));
        contacts.push_back(p_contacts[i]);
    }

    showContactList();

    ui->search_line->setPlaceholderText(m_cSearchPlaceholderText);
    ui->message_line->setPlaceholderText(m_cMessagePlaceholderText);

    backgroundThread = new std::thread(&MainWindow::MessageHandler, this);
    backgroundThread->detach();

    g_bFlag = true;
    g_condVar.notify_one();
}

void MainWindow::MessageHandler()
{
    qDebug("In thread!");
    server_response_st response;
    memset( (void*)&response, 0, sizeof(response) );

    std::unique_lock<std::mutex> ulock(g_mutex);
    while (true)
    {
        g_condVar.wait( ulock, GetCondition );

        m_pNet->NonBlockingRecv( (char*)&response, sizeof(response), 1);
        user_st * contacts_st = nullptr;

        if ( response.response_code == SERVER_UPDATE )
        {
            int count = 0;

            memset( (void*)&response, 0, sizeof(response) );
            GetContactList(&contacts_st, response, &count);

            contacts.clear();
            itemsList.clear();

            for(int i = 0; i < count; i++)
            {
                pushItem(makeItem(contacts_st[i]));
                user_st us = contacts_st[i];
                contacts.push_back(us);
            }

            showContactList();

            free(contacts_st);
            contacts_st = nullptr;
            count = 0;
        }
        else if ( response.response_code == SERVER_MSG )
        {
            qDebug() << response.response_code << " MSG";
            msg_st msg;
            memset((void*)&msg, 0, sizeof(msg));

            m_pNet->NonBlockingRecv((char*)&msg, sizeof(msg), 1);

            if ( strcmp(msg.mime_type, "text") == 0 )
            {
                char * textMessage = (char*)malloc(sizeof(char) * msg.msg_size);

                m_pNet->NonBlockingRecv(textMessage, msg.msg_size, 1);

                QString name = GetUserNameById(msg.who);

                QString message = QString("[") + QDateTime::currentDateTime().toString()
                        + QString("] ") + GetUserNameById(msg.who) + QString(": ") + QString(textMessage);

                m_msgMap.PushMessage(msg.who, message );

                // ui->disp_msg->addItem(QString(textMessage));

                free(textMessage);
            }
            else if( strcmp(msg.mime_type, "file") == 0 )
            {
                ssize_t len;
                char buffer[BUFSIZ];
                int file_size;
                FILE *received_file;
                int remain_data = 0;

                file_size = msg.msg_size;

                received_file = fopen(msg.file_name, "w");
                if (received_file == NULL)
                {
                    qDebug() << "Failed to open file " << msg.file_name << strerror(errno);
                }
                else
                {
                    remain_data = file_size;
                    qDebug() << msg.file_name << " file " << msg.msg_size;
                    while (((len = m_pNet->Recv(buffer, BUFSIZ)) > 0) && (remain_data > 0))
                    {
                        fwrite(buffer, sizeof(char), len, received_file);
                        remain_data -= len;
                    }
                }
                fclose(received_file);
            }
        }

        memset( (void*)&response, 0, sizeof(response) );
    }
}

bool MainWindow::GetCondition()
{
    return g_bFlag;
}

const QString MainWindow::GetUserNameById(int id)
{
    QString retval;

    for(int i = 0; i < contacts.size(); i++)
    {
        if (contacts[i].id == id)
        {
            retval += QString(contacts[i].firstname) + QString(" ") + QString(contacts[i].secondname);
        }
    }

    return retval;
}

void MainWindow::GetContactList(user_st ** contacts, server_response_st& response, int* count)
{
    m_pNet->SendRequestCode(LISTCONTACT);

    memset( (void*)&response, 0, sizeof(response) );
    m_pNet->NonBlockingRecv( (char*)&response, sizeof(response), 1 );

    if ( response.response_code == SERVER_OK )
    {
        m_pNet->NonBlockingRecv( (char*)count, sizeof(*count), 1);
        int dataSize = sizeof(user_st) * (*count);
        *contacts = (user_st *)malloc(dataSize);

        m_pNet->NonBlockingRecv( ( (char*)((*contacts)) ), dataSize);
    }
    else
    {
        QMessageBox::warning(this, "Warning!" ,QString(response.msg_buffer) );
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::checkFileSize()
{
    return ( m_pFileHandle->size() <= g_clMaxFileSize ) ? true : false;
}

void MainWindow::showContactList()
{
    ui->contactList->clear();

    for (QListWidgetItem &item : itemsList)
    {
        ui->contactList->addItem(&item);
    }
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
        g_bFlag = false;
        g_condVar.notify_one();

        m_iUserIndex = ui->contactList->selectionModel()->currentIndex().row();

        if ( m_iUserIndex != -1 )
        {
            char code = SENDTO;
            m_pNet->Send((const char*)&code,1);

            QFileInfo fInfo = QFileInfo(m_filePath);

            msg_st msg;
            memset( (void*)&msg, 0, sizeof(msg) );

            msg.who  = userInfo.getId();
            msg.whom = contacts[m_iUserIndex].id;
            memcpy(msg.mime_type, "file", strlen("file")+1);
            memcpy( msg.file_name, fInfo.fileName().toUtf8().data(), fInfo.fileName().length() );
            msg.msg_size = fInfo.size();

            m_pNet->Send( (char*)&msg, sizeof(msg) );
            m_pNet->SendFile(m_filePath.toStdString().c_str());

            server_response_st resp;
            memset((void*)&resp, 0 , sizeof(resp));

            m_pNet->NonBlockingRecv((char*)&resp,sizeof(resp),1);
        }
        else if( m_iUserIndex == -1 )
        {
            QMessageBox::information(this, "Info", "Please select recipient!");
        }

        server_response_st response;
        memset((void*)&response, 0, sizeof(response));

        m_pNet->NonBlockingRecv((char*)&response, sizeof(response),1);

        if ( response.response_code == SERVER_OK )
        {
            ui->newContacts->addItem("Message sent!");
        }
        else if ( response.response_code == SERVER_FAIL )
        {
            ui->newContacts->addItem("Message send error!");
        }

        g_bFlag = true;
        g_condVar.notify_one();
    }
    else
    {
        QMessageBox::warning(this, "File not opened!", "No such file or directory!");
    }
}

void MainWindow::on_send_button_clicked()
{
    g_bFlag = false;
    g_condVar.notify_one();

    m_iUserIndex = ui->contactList->selectionModel()->currentIndex().row();

    if ( m_iUserIndex != -1 && !(ui->message_line->text().isEmpty()) )
    {
        char code = SENDTO;
        m_pNet->Send((const char*)&code,1);

        msg_st msg;
        memset( (void*)&msg, 0, sizeof(msg) );

        msg.who  = userInfo.getId();
        msg.whom = contacts[m_iUserIndex].id;
        memcpy( msg.mime_type, "text", strlen("text")+ 1);
        msg.msg_size = ui->message_line->text().length() + 1;

        m_pNet->Send( (char*)&msg, sizeof(msg) );

        char * buf = (char *)malloc(sizeof(char) * msg.msg_size);
        memcpy( (void*)buf, (const void*)ui->message_line->text().toUtf8().data(),
                msg.msg_size );

        qDebug() << "Messge" << buf;
        m_pNet->Send(buf, msg.msg_size);

        free(buf);

        server_response_st response;
        memset((void*)&response, 0, sizeof(response));

        qDebug() << "send";
        m_pNet->Recv((char*)&response, sizeof(response));

        if ( response.response_code == SERVER_OK )
        {
            ui->newContacts->addItem("Message sent!");
        }
        else if ( response.response_code == SERVER_FAIL )
        {
            ui->newContacts->addItem("Message send error!");
        }

    }
    else if( m_iUserIndex == -1 )
    {
        QMessageBox::information(this, "Info", "Please select recipient!");
    }

    m_iUserIndex = -1;
    ui->message_line->clear();

    g_bFlag = true;
    g_condVar.notify_one();
}

void MainWindow::setUpComboBox()
{
    ui->comboBox->insertItem(0, QIcon(":/statusIcons/online.ico"),  "online");
    ui->comboBox->insertItem(1, QIcon(":/statusIcons/offline.ico"), "offline");
    ui->comboBox->insertItem(2, QIcon(":/statusIcons/busy.ico"),    "busy");
}

void MainWindow::setUserInfo(const user_st &info)
{
    userInfo.SetId(info.id);
    userInfo.SetFirstName(QString(info.firstname));
    userInfo.SetSecondName(QString(info.secondname));
    userInfo.SetUserName(info.username);

    ui->userName->setText( userInfo.getFirstName() + " " + userInfo.getSecondName() );
}

QListWidgetItem MainWindow::makeItem(const user_st &user)
{
    QListWidgetItem item;
    item.setText( QString(user.firstname) + " " + QString(user.secondname) );

    return item;
}

void MainWindow::pushItem(const QListWidgetItem &item)
{
    itemsList.push_back(item);
}

void MainWindow::on_search_new_button_clicked()
{
    g_bFlag = false;
    g_condVar.notify_one();

    QString userName = ui->lineEdit->text();

    if( !userName.isEmpty() )
    {
        m_pNet->SendRequestCode(ADDCONTACT);

        contact_add user_add;
        memset(user_add.username, 0, BUFFER_SIZE);
        user_add.who = userInfo.getId();
        memcpy(user_add.username, userName.toUtf8().data(), userName.length());

        const int currentLen = sizeof(user_add);

        char tmp[currentLen];
        memset(tmp, 0, currentLen);
        memcpy(tmp, (const void*)&user_add, currentLen);

        m_pNet->Send(tmp, currentLen);
    }

    g_bFlag = true;
    g_condVar.notify_one();
}

void MainWindow::on_pushButton_clicked()
{
    ui->newContacts->clear();
}

void MainWindow::on_pushButton_2_clicked()
{
    g_bFlag = false;
    g_condVar.notify_one();

    int currentState = ui->comboBox->currentIndex();
    qDebug() << "current state " << currentState;
    m_pNet->SendRequestCode(CHANGESTATE);

    m_pNet->Send( (const char*)&currentState, sizeof(currentState) );

    server_response_st response;
    memset( (void*)&response, 0, sizeof(response) );

    m_pNet->Recv( (char *)&response, sizeof(response) );

    if ( response.response_code == SERVER_OK )
    {
        qDebug() << "Send state ok!";
    }

    qDebug() << "rc = " << response.response_code;

    g_bFlag = true;
    g_condVar.notify_one();
}

void MainWindow::on_disp_msg_itemClicked(QListWidgetItem *item)
{
    int index = ui->contactList->currentIndex().row();
    index = contacts[index].id;
    m_msgMap.PopMessage(index, ui->disp_msg->currentIndex().row() );

    delete item;
}

void MainWindow::on_contactList_itemClicked(QListWidgetItem *item)
{
    ui->disp_msg->clear();

    int index = ui->contactList->currentIndex().row();

    QVector<QString> msgs = m_msgMap.GetMessages(contacts[index].id);

    for(int i = 0; i < msgs.size(); i++ )
    {
        ui->disp_msg->addItem(msgs[i]);
    }
}
