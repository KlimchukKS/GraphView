#include "edgenetwork.h"

#include <QPainter>

#include <cmath>

EdgeNetwork::EdgeNetwork(const QPointF& from,
                         const QPointF& to,
                         const QString& id,
                         const QString& name,

                         int step,
                         int node_width_,
                         int node_height_,

                         const QColor& working_rest,
                         const QColor& working_guidance,

                         const QColor& emergency_rest,
                         const QColor& emergency_guidance,

                         int rest_thickness,
                         int guidance_thickness)
    : from_{from}
    , to_{to}
    , id_{id}
    , working_rest_{working_rest}
    , working_guidance_{working_guidance}
    , emergency_rest_{emergency_rest}
    , emergency_guidance_{emergency_guidance}
    , rest_thickness_{rest_thickness}
    , guidance_thickness_{guidance_thickness}
{
    QGraphicsItem::setAcceptHoverEvents(true);
    this->setToolTip(name);

    QGraphicsItem::setPos(from_.x() + step, from_.y() + node_height_);

    int fx = from_.x() + step, tox = to_.x() + step;
    int fy = from_.y() + node_height_, toy = to_.y();
    int y = toy - fy;
    int x = 0;

    if (from_.x() == to_.x()) {
        hypotenuse_ = to_.y() - from_.y() - node_height_;
    } else if (from_.x() > to_.x()) {
        x = fx - tox;

        hypotenuse_ = std::sqrt( x * x + y * y);

        double sinus = x / hypotenuse_;
                                    // Переводим радианы в градусы
        angle_ = std::asin(sinus) * 180 / 3.14;
    } else {
        x = tox - fx;

        hypotenuse_ = std::sqrt( x * x + y * y);

        double sinus = x / hypotenuse_;
        angle_ = std::asin(sinus) * -180 / 3.14;
    }

    QGraphicsItem::setRotation(angle_);
}

const QString& EdgeNetwork::GetID() const {
    return id_;
}

void EdgeNetwork::SetCondition(bool condition) {
    condition_ = condition;

    this->update(QRectF(0, 0, guidance_thickness_, hypotenuse_));
}

QRectF EdgeNetwork::boundingRect() const {
    return QRectF(0, 0, guidance_thickness_, hypotenuse_);
}

void EdgeNetwork::paint(QPainter *painter, const QStyleOptionGraphicsItem *option [[maybe_unused]], QWidget *widget [[maybe_unused]]) {
    line_.setLine(0, 0, 0, hypotenuse_);

    QPen pen((condition_) ? (is_hover_enter_ ? working_guidance_ : working_rest_) : (is_hover_enter_ ? emergency_guidance_ : emergency_rest_));
    pen.setWidth((is_hover_enter_ || is_node_hover_enter_) ? guidance_thickness_ : rest_thickness_);

    painter->setPen(pen);

    painter->drawLine(line_);
}

void EdgeNetwork::SetNodeHoverEnterEvent(bool node_hover_enter) {
    is_node_hover_enter_ = node_hover_enter;

    this->update(QRectF(0, 0, guidance_thickness_, hypotenuse_));
}

void EdgeNetwork::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
    is_hover_enter_ = true;

    this->update(QRectF(0, 0, guidance_thickness_, hypotenuse_));

    QGraphicsItem::hoverEnterEvent(event);
}

void  EdgeNetwork::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    is_hover_enter_ = false;

    this->update(QRectF(0, 0, guidance_thickness_, hypotenuse_));

    QGraphicsItem::hoverLeaveEvent(event);
}
