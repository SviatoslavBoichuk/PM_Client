#include "cuser.h"

CUser::CUser()
{
    m_pUserData = new (std::nothrow) UserData;
}

CUser::~CUser()
{
    delete m_pUserData;
}

CUser::CUser(const QString &name, const QString &firstName, const QString &secondName, const QString &mail,
             const QString &pass, const State state, int ID)
{
    m_pUserData = new (std::nothrow) UserData;

    m_pUserData->m_userName = name;
    m_pUserData->m_userPass = pass;
    m_pUserData->m_userMail = mail;
    m_pUserData->m_firstName = firstName;
    m_pUserData->m_secondName = secondName;
    m_pUserData->m_userState = state;
    m_pUserData->m_id = ID;
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

int CUser::getId() const
{
    return m_pUserData->m_id;
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

void CUser::SetId(int ID)
{
    m_pUserData->m_id = ID;
}
