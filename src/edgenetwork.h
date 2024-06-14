#ifndef EDGENETWORK_H
#define EDGENETWORK_H

#include <QObject>
#include <QGraphicsItem>

class EdgeNetwork : public QObject, public QGraphicsItem {
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    EdgeNetwork() = delete;

    EdgeNetwork(const QPointF& from,
                const QPointF& to,
                const QString& id,
                const QString& name,

                int step,
                int node_width_,
                int node_height_,

                const QColor& working_rest_,
                const QColor& working_guidance_,

                const QColor& emergency_rest_,
                const QColor& emergency_guidance_,

                int rest_thickness,
                int guidance_thickness);

    const QString& GetID() const;

    void SetNodeHoverEnterEvent(bool node_hover_enter);

    void SetCondition(bool condition);

private:
    const QPointF from_;
    const QPointF to_;
    const QString id_;

    QLine line_;

    bool condition_ = true;

    double hypotenuse_ = 0;
    double angle_ = 0.0;
    bool is_hover_enter_ = false;
    bool is_node_hover_enter_ = false;

    const QColor& working_rest_;
    const QColor& working_guidance_;

    const QColor& emergency_rest_;
    const QColor& emergency_guidance_;

    int rest_thickness_;
    int guidance_thickness_;

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
};

#endif // EDGENETWORK_H
