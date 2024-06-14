#include "basenode.h"

BaseNode::BaseNode(const QString& id,
                   const QString& name,
                   const QString& ip,
                   int priority)
    : id_(id)
    , name_(name)
    , ip_(ip)
    , priority_(priority)
{

}

BaseNode::BaseNode(QString&& id,
                   QString&& name,
                   QString&& ip,
                   int priority)
    : id_(std::move(id))
    , name_(std::move(name))
    , ip_(std::move(ip))
    , priority_(priority)
{

}

const QString& BaseNode::GetID() const {
    return id_;
}

const QString& BaseNode::GetName() const {
    return name_;
}

const QString& BaseNode::GetIP() const {
    return ip_;
}

int BaseNode::GetPriority() const {
    return priority_;
}

bool BaseNode::GetCondition() const {
    return condition_;
}

void BaseNode::SetCondition(bool condition) {
    condition_ = condition;
}
