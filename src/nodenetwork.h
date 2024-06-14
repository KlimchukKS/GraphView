#ifndef NODENETWORK_H
#define NODENETWORK_H

#include <QObject>
#include <QGraphicsItem>
#include <QPainter>
#include <QVector>

#include "basenode.h"
#include "edgenetwork.h"

class NodeNetwork : public QObject, public QGraphicsItem, public BaseNode {
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    NodeNetwork() = delete;

    NodeNetwork(BaseNode&& bn,
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

                QObject *parent = nullptr);

    void SetEdge(EdgeNetwork* edge) {
        list_of_edges_.push_back(edge);
    }

    std::vector<EdgeNetwork*>& GetListOfEdges() {
        return list_of_edges_;
    }

    void SetListOfNodes(std::vector<NodeNetwork*>&& list_of_nodes) {
        list_of_nodes_ = std::move(list_of_nodes);
    }

    std::vector<NodeNetwork*>& GetListOfNodes() {
        return list_of_nodes_;
    }

    void SetCondition(bool condition);

signals:
    void IsPressed(const QString& id);
private:
    const QPixmap image_;
    const QString no_image_message_{"No image"};

    QString abbreviated_name_;

    const QColor& working_rest_;
    const QColor& working_guidance_;
    const QColor& working_pressing_;

    const QColor& emergency_rest_;
    const QColor& emergency_guidance_;
    const QColor& emergency_pressing_;

    QFont font_;

    int width_;
    int height_;

    int frame_thickness_;
    int pressing_thickness_;
    int guidance_thickness_;

    bool is_pressed_     = false;
    bool is_hover_enter_ = false;

    std::vector<EdgeNetwork*> list_of_edges_;
    std::vector<NodeNetwork*> list_of_nodes_;

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
};

#endif // NODENETWORK_H
