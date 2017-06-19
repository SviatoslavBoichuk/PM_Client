#include "cuser.h"
#include <new>

CUser::CUser()
{
    m_pUserData = new (std::nothrow) UserData;
    m_pUserData->m_pUserIcon = nullptr;
}

CUser::~CUser()
{
    delete m_pUserData->m_pUserIcon;
    delete m_pUserData;
}

CUser::CUser(const QString &name, const QString &firstName, const QString &secondName, const QString &mail,
             const QString &pass, const State state)
{
    m_pUserData = new (std::nothrow) UserData;

    m_pUserData->m_userName = name;
    m_pUserData->m_userPass = pass;
    m_pUserData->m_userMail = mail;
    m_pUserData->m_firstName = firstName;
    m_pUserData->m_secondName = secondName;
    m_pUserData->m_userState = state;

    m_pUserData->m_pUserIcon = nullptr;
}

const QString &CUser::getSecondName() const
{
    return m_pUserData->m_secondName;
}

const QString &CUser::getFirstName() const
{
    return m_pUserData->m_firstName;
}

const QString &CUser::getUserName() const
{
    return m_pUserData->m_userName;
}

const QString &CUser::getUserMail() const
{
    return m_pUserData->m_userMail;
}

const QString &CUser::getUserPass() const
{
    return m_pUserData->m_userPass;
}

const QPixmap *CUser::getUserIcon() const
{
    return m_pUserData->m_pUserIcon;
}

State CUser::getUserState() const
{
    return m_pUserData->m_userState;
}

void CUser::SetUserName(const QString &userName)
{
    m_pUserData->m_userName = userName;
}

void CUser::SetUserPass(const QString &pass)
{
    m_pUserData->m_userPass = pass;
}

void CUser::SetUserMail(const QString &mail)
{
    m_pUserData->m_userMail = mail;
}

void CUser::SetSecondName(const QString &surname)
{
    m_pUserData->m_secondName = surname;
}

void CUser::SetFirstName(const QString &name)
{
    m_pUserData->m_firstName = name;
}

void CUser::SetUserState(const State state)
{
    m_pUserData->m_userState = state;
}

void CUser::SetUserIcon(const QString &path)
{
    if ( !path.isEmpty() )
    {
        m_pUserData->m_pUserIcon = new QPixmap(path);
    }
}

QByteArray CUser::serialize()
{
    QByteArray buffer;
    QDataStream stream(&buffer, QIODevice::WriteOnly);
    stream.setVersion(QDataStream::Qt_5_9);

    stream << m_pUserData->m_userName << m_pUserData->m_userPass << m_pUserData->m_userMail
           << m_pUserData->m_firstName << m_pUserData->m_secondName;

    if( m_pUserData->m_pUserIcon != nullptr )
    {
        stream << (*m_pUserData->m_pUserIcon);
    }

    return buffer;
}
