#ifndef CUSERSMESSAGES_H
#define CUSERSMESSAGES_H

#include <map>
#include <vector>
#include <QString>

class CUsersMessages
{
public:
    CUsersMessages()  = default;
    ~CUsersMessages() = default;
    CUsersMessages& operator =(const CUsersMessages&) = default;

    void PushMessage(int id, const QString& msg);
    void PopMessage(int id, int msg_index);

    QVector<QString> GetMessages(int id);

private:
    std::map<int, QVector<QString> > m_message_map;
};

#endif // CUSERSMESSAGES_H
