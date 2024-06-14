#include "graphnetworkview.h"

#include <stack>

GraphNetworkView::GraphNetworkView(QWidget *parent)
    : QGraphicsView(parent)
{
    this->SetGraphicsViewSettings();

    node_height_ = node_width_ + QFontMetrics(node_font_).height();

    g.node_size = 80;
}

void GraphNetworkView::SetVertices(std::vector<std::pair<BaseNode, const QString>>&& base_node_and_path_to_image) {
    adj_list_.reserve(base_node_and_path_to_image.size());

    for (auto& [bn, path_to_image] : base_node_and_path_to_image) {
        list_of_nodes_.emplace_front(std::move(bn)
                                     , path_to_image

                                     , working_rest_
                                     , working_guidance_
                                     , working_pressing_

                                     , emergency_rest_
                                     , emergency_guidance_
                                     , emergency_pressing_

                                     , node_font_

                                     , node_width_
                                     , node_height_

                                     , node_frame_thickness_
                                     , node_pressing_thickness_
                                     , node_guidance_thickness_);

        adj_list_.emplace_back(&list_of_nodes_.front());

        connect(&list_of_nodes_.front(), SIGNAL(IsPressed(const QString&)), this, SIGNAL(VertexIsPressed(const QString&)));

        node_id_to_node_ptr_[list_of_nodes_.front().GetID()] = &list_of_nodes_.front();

        node_ptr_to_vertex_t[&list_of_nodes_.front()] = g.add_node();
        vertex_t_to_node_ptr_[node_ptr_to_vertex_t[&list_of_nodes_.front()]] = &list_of_nodes_.front();
    }
}

void GraphNetworkView::SetEdges(std::vector<BaseEdge>&& edges) {
    // Создаём коллекцию, чтобы избавиться от дубликатов
    std::unordered_set<BaseEdge, BaseEdge::BaseEdgeHasher> set_edges({edges.begin(), edges.end()});

    // Задаем вершинам указатели на связанные вершины
    SetVerticesChildren(set_edges);

    //------------------------------------------------------------------

    for (auto from : adj_list_) {
        for (auto to : from->GetListOfNodes()) {
            g.add_edge(node_ptr_to_vertex_t[from], node_ptr_to_vertex_t[to]);
        }
    }

    VertexPlacemarker::sugiyama_layout layout(g);

    for (auto node : layout.vertices()) {
        vertex_t_to_node_ptr_[node.u]->setPos(node.pos.x, node.pos.y);
        scene_->addItem(vertex_t_to_node_ptr_[node.u]);
    }

    //------------------------------------------------------------------

    std::unordered_map<BaseEdgeKey, std::vector<BaseEdge>, BaseEdgeKey::HashFromTo> edge_hash_to_edges;

    for (const auto& be : set_edges) {
        edge_hash_to_edges[BaseEdgeKey{be.from, be.to}].push_back(be);
    }

    for (auto& [_, edges] : edge_hash_to_edges) {
        int step = node_width_ / edges.size();

        int count = step / 2;

        std::cout << "node_width_: " << node_width_ << " edges.size(): " << edges.size() << " step: " << step << " count: " << count << std::endl;

        for (auto& base_edge : edges) {
            QPointF from = node_id_to_node_ptr_[base_edge.from]->pos();
            QPointF to = node_id_to_node_ptr_[base_edge.to]->pos();

            auto tmp_from_ptr = node_id_to_node_ptr_[base_edge.from];
            auto tmp_to_ptr = node_id_to_node_ptr_[base_edge.to];

            if (node_id_to_node_ptr_[base_edge.from]->GetPriority() < node_id_to_node_ptr_[base_edge.to]->GetPriority()) {
                std::swap(from, to);

                std::swap(tmp_from_ptr, tmp_to_ptr);
            }

            std::cout << tmp_from_ptr->GetName().toStdString() << " x: " << from.x() << " y: " << from.y() << std::endl;
            std::cout << tmp_to_ptr->GetName().toStdString() << " x: " << to.x() << " y: " << to.y() << std::endl;

            std::cout << "step: " << count << '\n';

            list_of_edges_.emplace_front(from
                                         , to
                                         , base_edge.id
                                         , base_edge.name

                                         , count
                                         , node_width_
                                         , node_height_

                                         , working_rest_
                                         , working_guidance_

                                         , emergency_rest_
                                         , emergency_guidance_

                                         , edge_rest_thickness_
                                         , edge_guidance_thickness_);

            edge_id_to_edge_ptr_[list_of_edges_.front().GetID()] = &list_of_edges_.front();

            count += step;

            // Задаём вершинам указатели на исходящие из них рёбра
            node_id_to_node_ptr_[base_edge.from]->SetEdge(&list_of_edges_.front());
            node_id_to_node_ptr_[base_edge.to]->SetEdge(&list_of_edges_.front());

            // Добавляем на сцену ребро
            scene_->addItem(&list_of_edges_.front());
        }
    }

    auto sr = sceneRect();

    sr.setX(0);

    sr.setY(0);

    sr.setWidth(sr.width() + node_width_);

    sr.setHeight(sr.height() + node_width_);

    scene_->addRect(sr);
}

void GraphNetworkView::SetGraphicsViewSettings() {
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn); // Cкроллбар по горизонтали
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);   // Cкроллбар по вертикали

    scene_ = new QGraphicsScene();
    // Устанавливаем сцену в виджет
    this->setScene(scene_);
}

void GraphNetworkView::SetVerticesChildren(const std::unordered_set<BaseEdge, BaseEdge::BaseEdgeHasher>& edges) {
    std::unordered_map<QString, std::unordered_set<NodeNetwork*>> id_to_vertex_list_of_links;

    /*// Инициализация не направленного графа
    for (const auto& edge : edges) {
        id_to_vertex_list_of_links[edge.from].insert(node_id_to_node_ptr_[edge.to]);
        id_to_vertex_list_of_links[edge.to].insert(node_id_to_node_ptr_[edge.from]);
    }*/

    //Инициализация направленного графа. Связи идут от вершины с большим приоритетом к вершине с меньшим
    for (const auto& edge : edges) {
        if (!node_id_to_node_ptr_.count(edge.from) && !node_id_to_node_ptr_.count(edge.to)) {
            throw ERROR_CODE::UNKNOWN_VERTICES_NAMES_FROM_AND_TO;
        }

        if (!node_id_to_node_ptr_.count(edge.from)) {
            throw ERROR_CODE::UNKNOWN_VERTEX_NAME_FROM;
        }

        if (!node_id_to_node_ptr_.count(edge.to)) {
            throw ERROR_CODE::UNKNOWN_VERTEX_NAME_TO;
        }

        if (node_id_to_node_ptr_[edge.from]->GetPriority() > node_id_to_node_ptr_[edge.to]->GetPriority()) {
            id_to_vertex_list_of_links[edge.from].insert(node_id_to_node_ptr_[edge.to]);
        } else {
            id_to_vertex_list_of_links[edge.to].insert(node_id_to_node_ptr_[edge.from]);
        }
    }
    // Устанавливаем вершинам их детей
    for (const auto& [id, list_of_links] : id_to_vertex_list_of_links) {
        node_id_to_node_ptr_[id]->SetListOfNodes({list_of_links.begin(), list_of_links.end()});
    }
}

GraphNetworkView::ConnectivityComponents GraphNetworkView::SearchForConnectivityComponents(std::vector<NodeNetwork*>& adj_list) {
    std::unordered_map<NodeNetwork*, int> color;
    // Заполняем словарь цветов
    for (auto v : adj_list) {
        color[v] = -1;
    }

    int component_count = 0;

    ConnectivityComponents ans{ 1 };

    for (auto i : adj_list) {
        if (color[i] != -1) {
            continue;
        }

        std::stack<NodeNetwork*> st;

        st.push(i);

        while (!st.empty()) {
            // Получаем из стека очередную вершину.
            NodeNetwork* vertex = st.top();
            st.pop();

            // Если эту вершину уже посетили, то переходим к следующей.
            if (color[vertex] != -1) {
                continue;
            }

            // Добавляем в компоненту связности очередную вершину.
            ans[component_count].push_back(vertex);

            color[vertex] = component_count;

            st.push(vertex);

            // Перебираем смежные вершины
            for (NodeNetwork* v : vertex->GetListOfNodes()) {
                // Если вершина не посещена, то
                if (color[v] == -1) {
                    st.push(v);
                }
            }
        }
        ans.push_back({});
        ++component_count;
    }

    ans.resize(component_count);

    return ans;
}

std::vector<GraphNetworkView::ConnectivityComponents> GraphNetworkView::GetSortedGraphByPriority(GraphNetworkView::ConnectivityComponents&& connect_comp) {
    std::vector<std::vector<AdjacencyList>> graph{ connect_comp.size() };

    for (auto i = 0; i < connect_comp.size(); ++i) {
        for (auto vertex : connect_comp[i]) {
            if (vertex->GetPriority() + 1 > graph[i].size()) {
                graph[i].resize(vertex->GetPriority() + 1);
            }

            graph[i][vertex->GetPriority()].push_back(vertex);
        }

        graph[i] = {graph[i].rbegin(), graph[i].rend()};
    }

    return graph;
}

void GraphNetworkView::ArrangeVertices(std::vector<std::vector<AdjacencyList>>&& graph) {
    int height = 0;

    std::unordered_map<std::vector<AdjacencyList>*, int> connectivity_components_width;

    for (std::vector<AdjacencyList>& connectivity_components : graph) {
        height = (height < connectivity_components.size()) ? connectivity_components.size() : height;

        int max = 0;

        for (std::vector<NodeNetwork*>& priority : connectivity_components) {
            max = (max < priority.size()) ? priority.size() : max;
        }

        connectivity_components_width[&connectivity_components] = max;
    }

    int step_y = node_width_ * 3;
    int step_x = node_width_ * 2;

    int counter = 0;

    for (std::vector<AdjacencyList>& connectivity_components : graph) {
        int pos_y = 0;

        int max_x = 0;

        for (std::vector<NodeNetwork*>& priority : connectivity_components) {
            int pos_x = counter + (step_x * connectivity_components_width[&connectivity_components] / priority.size() / 2);

            for (NodeNetwork* nn : priority) {
                nn->setPos(pos_x, pos_y);

                scene_->addItem(nn);

                pos_x += step_x;
            }

            max_x = (max_x > pos_x) ? max_x : pos_x;

            pos_y += step_y;
        }
        counter += max_x;
    }
}

int GraphNetworkView::SetVerticesCondition(QString id, bool condition) {
    if (node_id_to_node_ptr_.count(id)) {
        node_id_to_node_ptr_[id]->SetCondition(condition);

        return 0;
    }

    return -1;
}

int GraphNetworkView::SetEdgeCondition(QString id, bool condition) {
    if (edge_id_to_edge_ptr_.count(id)) {
        edge_id_to_edge_ptr_[id]->SetCondition(condition);

        return 0;
    }

    return -1;
}
