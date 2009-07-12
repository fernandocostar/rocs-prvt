/***************************************************************************
 * main.cpp
 * This file is part of the KDE project
 * copyright (C)2004-2007 by Tomaz Canabrava (tomaz.canabrava@gmail.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 * You should have received a copy of the GNU Library General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 ***************************************************************************/

#ifndef SUI_GRAPHSCENE_H
#define SUI_GRAPHSCENE_H

#include <QGraphicsScene>
#include <QMultiHash>

class AbstractAction;
class Node;
class Edge;
class QGraphicsItem;
class GraphDocument;
class Graph;
class QGraphicsSceneMouseEvent;
class QGraphicsSceneWheelEvent;
class QKeyEvent;
class NodeItem;
class EdgeItem;


class GraphScene : public QGraphicsScene {
public:
    GraphScene(QObject *parent);
    void setAction(AbstractAction *action);
    QGraphicsItem* createNode(Graph *g, Node *n);
    QGraphicsItem* createEdge(Graph *g, Edge *e);
    void updateGraph(Graph *g);
    void updateDocument();
    void setActiveGraph(Graph *g);
    void setActiveGraphDocument(GraphDocument *gd);
    void insertGraphItem(Graph *g, QGraphicsItem *item);
    void clearGraph();
    void removeGItem(QGraphicsItem *i);
    void setHideEdges(bool h);
    bool hideEdges();
    void updateAfter(QGraphicsItem *item);

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void wheelEvent(QGraphicsSceneWheelEvent *wheelEvent);
    void keyPressEvent(QKeyEvent *keyEvent);

private:
    GraphDocument *_graphDocument;
    Graph *_graph;
    AbstractAction *_action;
    QMultiHash<Graph*, QGraphicsItem* > _hashGraphs;
    QList<QGraphicsItem*> _hidedEdges;

    bool _hideEdges;
};

#endif
