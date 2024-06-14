#ifndef NODENAME_H
#define NODENAME_H
/*
#include <QObject>
#include <QGraphicsItem>

class NodeName : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    NodeName() = delete;

    NodeName(const QString& name,

             const QColor& working_rest,
             const QColor& working_guidance,
             const QColor& working_pressing,

             const QColor& emergency_rest,
             const QColor& emergency_guidance,
             const QColor& emergency_pressing,

             const QFont& font,

             const int& width,
             const int& height,

             const int& frame_thickness,
             const int& pressing_thickness,
             const int& guidance_thickness,

             QObject *parent = nullptr);

    void SetCondition(bool condition);

private:
    QStringView name_;

    const QColor& working_rest_;
    const QColor& working_guidance_;
    const QColor& working_pressing_;

    const QColor& emergency_rest_;
    const QColor& emergency_guidance_;
    const QColor& emergency_pressing_;

    const QFont& font_;

    const int& width_;
    const int& height_;

    const int& frame_thickness_;
    const int& pressing_thickness_;
    const int& guidance_thickness_;

    bool is_pressed_     = false;
    bool is_hover_enter_ = false;

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
};
*/
#endif // NODENAME_H
