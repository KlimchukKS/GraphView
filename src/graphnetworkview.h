#ifndef GRAPHNETWORKVIEW_H
#define GRAPHNETWORKVIEW_H

#include "nodenetwork.h"
#include "base_edge.h"

#include "VertexPlacemarker.h"

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItemGroup>
#include <QWheelEvent>

#include <vector>
#include <forward_list>
#include <unordered_map>
#include <unordered_set>

class GraphNetworkView  : public QGraphicsView {
    Q_OBJECT

    Q_PROPERTY( QColor working_rest         READ GetWorkingRest           WRITE SetWorkingRest           DESIGNABLE true SCRIPTABLE true)
    Q_PROPERTY( QColor working_guidance     READ GetWorkingGuidance       WRITE SetWorkingGuidance       DESIGNABLE true SCRIPTABLE true)
    Q_PROPERTY( QColor working_pressing     READ GetWorkingPressing       WRITE SetWorkingPressing       DESIGNABLE true SCRIPTABLE true)
    Q_PROPERTY( QColor emergency_rest       READ GetEmergencyRest         WRITE SetEmergencyRest         DESIGNABLE true SCRIPTABLE true)
    Q_PROPERTY( QColor emergency_guidance   READ GetEmergencyGuidance     WRITE SetEmergencyGuidance     DESIGNABLE true SCRIPTABLE true)
    Q_PROPERTY( QColor emergency_pressing   READ GetEmergencyPressing     WRITE SetEmergencyPressing     DESIGNABLE true SCRIPTABLE true)
    Q_PROPERTY( int node_width              READ GetNodeWidth             WRITE SetNodeWidth             DESIGNABLE true SCRIPTABLE true)
    Q_PROPERTY( int node_frame_thickness    READ GetNodeFrameThickness    WRITE SetNodeFrameThickness    DESIGNABLE true SCRIPTABLE true)
    Q_PROPERTY( int node_pressing_thickness READ GetNodePressingThickness WRITE SetNodePressingThickness DESIGNABLE true SCRIPTABLE true)
    Q_PROPERTY( int node_guidance_thickness READ GetNodeGuidanceThickness WRITE SetNodeGuidanceThickness DESIGNABLE true SCRIPTABLE true)
    Q_PROPERTY( int edge_rest_thickness     READ GetEdgeRestThickness     WRITE SetEdgeRestThickness     DESIGNABLE true SCRIPTABLE true)
    Q_PROPERTY( int edge_guidance_thickness READ GetEdgeGuidanceThickness WRITE SetEdgeGuidanceThickness DESIGNABLE true SCRIPTABLE true)
    Q_PROPERTY( QFont edge_font             READ GetEdgeFont              WRITE SetEdgeFont              DESIGNABLE true SCRIPTABLE true)
    Q_PROPERTY( QFont node_font             READ GetNodeFont              WRITE SetNodeFont              DESIGNABLE true SCRIPTABLE true)

public:
    GraphNetworkView(QWidget *parent = 0);

    void SetVertices(std::vector<std::pair<BaseNode, const QString>>&& base_node_and_path_to_image);
    void SetEdges(std::vector<BaseEdge>&& edges);

    int SetVerticesCondition(QString id, bool condition);

    int SetEdgeCondition(QString id, bool condition);

    QFont GetEdgeFont() const {
        return edge_font_;
    }

    void SetEdgeFont(QFont font) {
        edge_font_ = font;
    }

    QFont GetNodeFont() const {
        return node_font_;
    }

    void SetNodeFont(QFont font) {
        node_font_ = font;

        node_height_ = node_width_ + QFontMetrics(node_font_).height();
    }

    int GetEdgeGuidanceThickness() const {
        return edge_guidance_thickness_;
    }
    void SetEdgeGuidanceThickness(int edge_guidance_thickness) {
        edge_guidance_thickness_ = edge_guidance_thickness;
    }

    int GetEdgeRestThickness() const {
        return edge_rest_thickness_;
    }
    void SetEdgeRestThickness(int edge_rest_thickness) {
        edge_rest_thickness_ = edge_rest_thickness;
    }

    int GetNodeGuidanceThickness() const {
        return node_guidance_thickness_;
    }
    void SetNodeGuidanceThickness(int node_guidance_thickness) {
        node_guidance_thickness_ = node_guidance_thickness;
    }

    int GetNodePressingThickness() const {
        return node_pressing_thickness_;
    }
    void SetNodePressingThickness(int node_pressing_thickness) {
        node_pressing_thickness_ = node_pressing_thickness;
    }

    int GetNodeFrameThickness() const {
        return node_frame_thickness_;
    }
    void SetNodeFrameThickness(int node_frame_thickness) {
        node_frame_thickness_ = node_frame_thickness;
    }

    QColor GetWorkingRest() const {
        return working_rest_;
    }
    void SetWorkingRest(const QColor& color) {
        working_rest_ = color;
    }

    QColor GetWorkingGuidance() const {
        return working_guidance_;
    }
    void SetWorkingGuidance(const QColor& color) {
        working_guidance_ = color;
    }

    QColor GetWorkingPressing() const {
        return working_pressing_;
    }
    void SetWorkingPressing(const QColor& color) {
        working_pressing_ = color;
    }

    QColor GetEmergencyRest() const {
        return emergency_rest_;
    }
    void SetEmergencyRest(const QColor& color) {
        emergency_rest_ = color;
    }

    QColor GetEmergencyGuidance() const {
        return emergency_guidance_;
    }
    void SetEmergencyGuidance(const QColor& color) {
        emergency_guidance_ = color;
    }

    QColor GetEmergencyPressing() const {
        return emergency_pressing_;
    }
    void SetEmergencyPressing(const QColor& color) {
        emergency_pressing_ = color;
    }

    int GetNodeWidth() const {
        return node_width_;
    }
    void SetNodeWidth(int width) {
        node_width_ = width;

        node_height_ = node_width_ + QFontMetrics(node_font_).height();
    }

    void MakeSceneImage(const QString& path_to_new_image) {
        QPixmap pixMap = this->grab();
        pixMap.save(path_to_new_image);
    }

    enum class ERROR_CODE {
        SUCCSESS = 0,
        UNKNOWN_VERTEX_NAME_FROM = 1,
        UNKNOWN_VERTEX_NAME_TO = 2,
        UNKNOWN_VERTICES_NAMES_FROM_AND_TO = 3,
    };

signals:
    void VertexIsPressed(const QString& id);

private:
    using AdjacencyList = std::vector<NodeNetwork*>;
    using ConnectivityComponents = std::vector<AdjacencyList>;

    QGraphicsScene* scene_;

    std::forward_list<NodeNetwork> list_of_nodes_;

    std::vector<NodeNetwork*> adj_list_;

    std::forward_list<EdgeNetwork> list_of_edges_;

    std::unordered_map<QStringView, NodeNetwork*> node_id_to_node_ptr_;
    std::unordered_map<QStringView, EdgeNetwork*> edge_id_to_edge_ptr_;

    std::unordered_map<VertexPlacemarker::vertex_t, NodeNetwork*> vertex_t_to_node_ptr_;
    std::unordered_map<NodeNetwork*, VertexPlacemarker::vertex_t> node_ptr_to_vertex_t;

    // Параметры отрисовки

    QColor working_rest_{Qt::darkBlue};
    QColor working_guidance_{20, 232, 255, 186};
    QColor working_pressing_{Qt::darkBlue};

    QColor emergency_rest_{Qt::red};
    QColor emergency_guidance_{223, 35, 32};
    QColor emergency_pressing_{Qt::red};

    QFont edge_font_;
    QFont node_font_;

    int node_width_ = 80;
    int node_height_;

    int node_frame_thickness_ = 4;
    int node_pressing_thickness_ = 10;
    int node_guidance_thickness_ = 10;

    int edge_rest_thickness_ = 1;
    int edge_guidance_thickness_ = 3;

    VertexPlacemarker::graph g;

    void SetGraphicsViewSettings();

    void SetVerticesChildren(const std::unordered_set<BaseEdge, BaseEdge::BaseEdgeHasher>& edges);

    static ConnectivityComponents SearchForConnectivityComponents(std::vector<NodeNetwork*>& adj_list);

    static std::vector<ConnectivityComponents> GetSortedGraphByPriority(ConnectivityComponents&& connect_comp);

    void ArrangeVertices(std::vector<std::vector<AdjacencyList>>&& graph);

public slots:

    void wheelEvent(QWheelEvent* event) {
        setTransformationAnchor(AnchorUnderMouse);

        // Scale the view / do the zoom
        const double scaleFactor = 1.15;

        if(event->angleDelta().y() > 0)
        {
            // Zoom in
            scale(scaleFactor, scaleFactor);
        } else {
            // Zooming out
            scale(1.0 / scaleFactor, 1.0 / scaleFactor);
        }
    }
};


#endif // GRAPHNETWORKVIEW_H
