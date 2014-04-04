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

#ifndef NODE_H
#define NODE_H

#include "libgraphtheoryexport.h"
#include "graphdocument.h"

#include <QObject>
#include <QSharedPointer>

namespace GraphTheory
{
class Node;
class NodePrivate;
typedef QSharedPointer<Node> NodePtr;

/**
 * \class Node
 */
class GRAPHTHEORY_EXPORT Node : public QObject
{
    Q_OBJECT

public:
    /**
     * Creates a new Node.
     *
     * @param document  the GraphDocument containing this node
     * @return a Node object
     */
    static NodePtr create(GraphDocumentPtr document);

    /** Destroys the node */
    virtual ~Node();

    /**
     * @return the GraphDocument that contains this node
     */
    GraphDocumentPtr document() const;

protected:
    Node();

private:
    Q_DISABLE_COPY(Node)
    const QScopedPointer<NodePrivate> d;
    void setQpointer(NodePtr q);
};
}

#endif
