#ifndef CUSER_H
#define CUSER_H

#include <QString>
#include <QPixmap>
#include "pm_packets.h"

struct UserData
{
    QString m_userName;    // a.k. login
    QString m_firstName;   // name
    QString m_secondName;  // surname
    QString m_userMail;    // email
    QString m_userPass;    // pass
    QPixmap * m_pUserIcon; // icon
    State   m_userState;   // state
};

class CUser
{
public:
    CUser();
    CUser(const CUser&)             = delete;
    CUser& operator =(const CUser&) = delete;
    ~CUser();

    // CUser(const UserData& data);
    CUser(const QString& name, const QString& firstName, const QString& secondName,
          const QString& mail, const QString& pass , const State state);

    const QString& getFirstName() const;
    const QString& getSecondName() const;
    const QString& getUserName() const;
    const QString& getUserMail() const;
    const QString& getUserPass() const;
    const QPixmap* getUserIcon() const;
    State    getUserState() const;

    void SetUserName(const QString& userName);
    void SetUserPass(const QString& pass);
    void SetUserMail(const QString& mail);
    void SetFirstName(const QString& name);
    void SetSecondName(const QString& surname);
    void SetUserState(const State state);
    void SetUserIcon(const QString& path);


    QByteArray serialize();
private:
    UserData* m_pUserData;
};

#endif // CUSER_H
