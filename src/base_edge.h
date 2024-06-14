#ifndef BASEEDGE_H
#define BASEEDGE_H

#include <QString>
#include <QHash>

struct BaseEdge {
    QString from;
    QString to;
    QString name;
    QString id;

    bool operator == (const BaseEdge& rhs) const;

    // Хешер для поиска одинаковых рёбер {from = "1", to = "2", name = {test}} == {from = "2", to = "1", name = {test}}
    struct BaseEdgeHasher {
        std::size_t operator () (const BaseEdge& edge) const;

        std::hash<QString> hasher_;
    };
};

struct BaseEdgeKey {
    QString from;
    QString to;

    bool operator == (const BaseEdgeKey& rhs) const;

    // Хешер для поиска рёбер с одинаковыми вершинами, на разными именами
    struct HashFromTo {
        std::size_t operator () (const BaseEdgeKey& edge) const;

        std::hash<QString> hasher_;
    };
};

#endif // BASEEDGE_H
