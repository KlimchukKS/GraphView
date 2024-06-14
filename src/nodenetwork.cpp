#include "nodenetwork.h"

#include <QMouseEvent>
#include <QPainter>

#include <QToolTip>

#include <QGraphicsSceneHoverEvent>

#include <QDebug>

NodeNetwork::NodeNetwork(BaseNode&& bn,
                         const QString& path_to_image,

                         const QColor& working_rest,
                         const QColor& working_guidance,
                         const QColor& working_pressing,

                         const QColor& emergency_rest,
                         const QColor& emergency_guidance,
                         const QColor& emergency_pressing,

                         const QFont& font,

                         int width,
                         int height,

                         int frame_thickness,
                         int pressing_thickness,
                         int guidance_thickness,

                         QObject *parent)
    : QObject(parent)
    , QGraphicsItem()
    , BaseNode{std::move(bn)}
    , image_{path_to_image}

    , working_rest_{working_rest}
    , working_guidance_{working_guidance}
    , working_pressing_{working_pressing}

    , emergency_rest_{emergency_rest}
    , emergency_guidance_{emergency_guidance}
    , emergency_pressing_{emergency_pressing}

    , font_{font}

    , width_{width}
    , height_{height}

    , frame_thickness_{frame_thickness}
    , pressing_thickness_{pressing_thickness}
    , guidance_thickness_{guidance_thickness}
{
    QGraphicsItem::setAcceptHoverEvents(true);

    if (int text_pixels_len = QFontMetrics(font_).boundingRect(BaseNode::GetName()).width() > width_) {
        this->setToolTip(BaseNode::GetName());

        abbreviated_name_ += " ";

        abbreviated_name_.reserve(BaseNode::GetName().size());

        for(auto ch : BaseNode::GetName()) {
            abbreviated_name_.push_back(ch);
            if (QFontMetrics(font_).boundingRect(abbreviated_name_ + "... ").width() > width_) {
                abbreviated_name_.resize(abbreviated_name_.size() - 1);
                break;
            }
        }

        abbreviated_name_ += "... ";
    }
}

void NodeNetwork::SetCondition(bool condition) {
    BaseNode::SetCondition(condition);

    this->update(NodeNetwork::boundingRect());
}

QRectF NodeNetwork::boundingRect() const {
    // Возвращаем координаты расположения вершины
    // по ним будет определяться нажатие вершины
    return QRectF(0, 0, width_, height_);
}

void NodeNetwork::mousePressEvent(QGraphicsSceneMouseEvent* event [[maybe_unused]]) {
    emit IsPressed(this->GetID());

    is_pressed_ = true;

    this->update(NodeNetwork::boundingRect());
}

void NodeNetwork::mouseReleaseEvent(QGraphicsSceneMouseEvent *event [[maybe_unused]]) {
    is_pressed_ = false;

    this->update(NodeNetwork::boundingRect());
}

void NodeNetwork::paint(QPainter *painter, const QStyleOptionGraphicsItem *option [[maybe_unused]], QWidget *widget [[maybe_unused]]) {
    QRect dst_rc(0, 0, width_, width_);
    QRect text_rc(0, width_, width_, 2 + height_ - width_);


    if (image_.isNull()) {
        painter->drawText(dst_rc, Qt::AlignCenter, no_image_message_);
    } else {
        painter->drawPixmap(dst_rc, image_);
    }

    QPen pen;

    painter->setFont(font_);

    if (is_pressed_) {
        const QBrush brush((GetCondition()) ? working_pressing_ : emergency_pressing_);

        pen.setBrush(brush);
        pen.setWidth(pressing_thickness_);
    } else if (is_hover_enter_) {
        const QBrush brush((GetCondition()) ? working_guidance_ : emergency_guidance_);

        pen.setBrush(brush);
        pen.setWidth(guidance_thickness_);
    } else {
        const QBrush brush((GetCondition()) ? working_rest_ : emergency_rest_);

        pen.setBrush(brush);
        pen.setWidth(frame_thickness_);
    }

    painter->drawText(text_rc, Qt::AlignCenter, (abbreviated_name_.isEmpty()) ? BaseNode::GetName() : abbreviated_name_);

    painter->setPen(pen);

    painter->drawRect(text_rc);
    painter->drawRect(dst_rc);
}

void NodeNetwork::hoverEnterEvent(QGraphicsSceneHoverEvent* event) {
    is_hover_enter_ = true;

    this->update(NodeNetwork::boundingRect());

    for (auto e : list_of_edges_) {
        e->SetNodeHoverEnterEvent(true);
    }

    QGraphicsItem::hoverEnterEvent(event);
}

void NodeNetwork::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    is_hover_enter_ = false;

    this->update(NodeNetwork::boundingRect());

    for (auto e : list_of_edges_) {
        e->SetNodeHoverEnterEvent(false);
    }

    QGraphicsItem::hoverLeaveEvent(event);
}
