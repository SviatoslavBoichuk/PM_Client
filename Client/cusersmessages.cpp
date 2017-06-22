#include "cusersmessages.h"

#include <QVector>
#include <QString>

void CUsersMessages::PushMessage(int id, const QString &msg)
{
    auto it = m_message_map.find(id);

    if (it != m_message_map.end())
    {
        it->second.push_back(msg);
    }
    else
    {
        m_message_map[id];
        auto it1 = m_message_map.find(id);
        it1->second.push_back(msg);
    }
}

void CUsersMessages::PopMessage(int id, int msg_index)
{
    auto it = m_message_map.find(id);
    it->second.remove(msg_index);
}

QVector<QString> CUsersMessages::GetMessages(int id)
{    
    auto findItPos = m_message_map.find(id);

    QVector<QString> ret;

    if ( findItPos != m_message_map.end())
    {
        ret = findItPos->second;
    }

    return ret;
}
