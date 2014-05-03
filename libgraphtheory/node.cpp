/*
 *  Copyright 2014  Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) version 3, or any
 *  later version accepted by the membership of KDE e.V. (or its
 *  successor approved by the membership of KDE e.V.), which shall
 *  act as a proxy defined in Section 6 of version 3 of the license.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "node.h"
#include "graphdocument.h"
#include "nodetype.h"
#include "edge.h"

#include <QPointF>
#include <QColor>
#include <QDebug>

using namespace GraphTheory;

// initialize number of edge objects
uint Node::objectCounter = 0;

class GraphTheory::NodePrivate {
public:
    NodePrivate()
        : m_valid(false)
        , m_x(0)
        , m_y(0)
        , m_color(Qt::black)
        , m_id(-1)
    {
    }

    ~NodePrivate()
    {
    }

    NodePtr q;
    GraphDocumentPtr m_document;
    NodeTypePtr m_type;
    EdgeList m_edges;
    bool m_valid;
    qreal m_x;
    qreal m_y;
    QColor m_color;
    int m_id;
};

Node::Node()
    : QObject()
    , d(new NodePrivate)
{
    ++Node::objectCounter;
}

Node::~Node()
{
    --Node::objectCounter;
}

NodePtr Node::create(GraphDocumentPtr document)
{
    NodePtr pi(new Node);
    pi->setQpointer(pi);
    pi->d->m_document = document;
    pi->d->m_type = document->nodeTypes().first();
    pi->d->m_valid = true;

    // insert completely initialized node into document
    document->insert(pi->d->q);

    return pi;
}

void Node::destroy()
{
    d->m_valid = false;
    foreach (EdgePtr edge, d->m_edges) {
        d->m_document->remove(edge);
    }
    d->m_document->remove(d->q);

    // reset last reference to this object
    d->q.reset();
}

bool Node::isValid() const
{
    return d->m_valid;
}

GraphDocumentPtr Node::document() const
{
    return d->m_document;
}

NodeTypePtr Node::type() const
{
    Q_ASSERT(d->m_type);
    return d->m_type;
}

void Node::setType(NodeTypePtr type)
{
    Q_ASSERT(d->m_document->nodeTypes().contains(type));
    if (d->m_type == type) {
        return;
    }
    d->m_type = type;
    emit typeChanged(type);
}

void Node::insert(EdgePtr edge)
{
    if (edge->from() != d->q && edge->to() != d->q) {
        return;
    }
    if (d->m_edges.contains(edge)) {
        return;
    }
    d->m_edges.append(edge);
    emit edgeAdded(edge);
}

void Node::remove(EdgePtr edge)
{
    if (edge && edge->isValid()) {
        edge->destroy();
    }
    d->m_edges.removeOne(edge);
}

EdgeList Node::edges() const
{
    return d->m_edges;
}

int Node::id() const
{
    return d->m_id;
}

void Node::setId(int id)
{
    if (id == d->m_id) {
        return;
    }
    d->m_id = id;
    emit idChanged(id);
}

qreal Node::x() const
{
    return d->m_x;
}

void Node::setX(qreal x)
{
    if (x == d->m_x) {
        return;
    }
    d->m_x = x;
    emit positionChanged(QPointF(x, d->m_y));
}

qreal Node::y() const
{
    return d->m_y;
}

void Node::setY(qreal y)
{
    if (y == d->m_y) {
        return;
    }
    d->m_y = y;
    emit positionChanged(QPointF(d->m_x, y));
}

QColor Node::color() const
{
    return d->m_color;
}

void Node::setColor(const QColor &color)
{
    if (color == d->m_color) {
        return;
    }
    d->m_color = color;
    emit colorChanged(color);
}

void Node::setQpointer(NodePtr q)
{
    d->q = q;
}
