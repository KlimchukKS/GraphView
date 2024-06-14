#ifndef BASENODE_H
#define BASENODE_H

#include <QString>
#include <QVector>

class BaseNode {
public:
    BaseNode(const QString& id,
             const QString& name,
             const QString& ip,
             int priority);

    BaseNode(QString&& id,
             QString&& name,
             QString&& ip,
             int priority);

    const QString& GetID() const;
    const QString& GetName() const;
    const QString& GetIP() const;

    int GetPriority() const;
    bool GetCondition() const;

    void SetCondition(bool condition);

    //virtual ~BaseNode();
private:
    const QString id_;

    const QString name_;

    const QString ip_;

    bool condition_ = true;

    int priority_ = 0;
};

#endif // BASENODE_H
