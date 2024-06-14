#include "graphnetworkview.h"

#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    std::vector<std::pair<BaseNode, const QString>> base_node_and_path_to_image{{{{"A1", "A1", "A1", 1}, ""},
                                                                                 {{"A2", "A2", "A2", 1}, ""},
                                                                                 {{"A3", "A3", "A3", 1}, ""},
                                                                                 {{"B1", "B1", "B1", 0}, ""},
                                                                                 {{"B2", "B2", "B2", 0}, ""},
                                                                                 {{"B3", "B3", "B3", 0}, ""},
                                                                                 {{"B4", "B4", "B4", 0}, ""},
                                                                                 {{"B5", "B5", "B5", 0}, ""}
                                                                                 }};

    std::vector<BaseEdge> edges{{{"A1", "B1", "from A1 to B1", "1"},
                                 {"A1", "B4", "from A1 to B4", "2"},
                                 {"A2", "B2", "from A2 to B2", "3"},
                                 {"A2", "B3", "from A2 to B3", "4"},
                                 {"A2", "B4", "from A2 to B4", "5"},
                                 {"A3", "B3", "from A2 to B3", "6"},
                                 {"A3", "B5", "from A2 to B5", "7"}
                                 }};

    auto graph_view = new GraphNetworkView();

    graph_view->SetVertices(std::move(base_node_and_path_to_image));

    try {
        graph_view->SetEdges(std::move(edges));
    } catch(GraphNetworkView::ERROR_CODE code) {
        qDebug() << static_cast<int>(code);
    }

    graph_view->show();

    return a.exec();
}
