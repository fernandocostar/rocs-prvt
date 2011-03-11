/* This file is part of Rocs, a KDE EDU project
   Copyright (C) 2008 by:
   Tomaz Canabrava <tomaz.canabrava@gmail.com>
   Ugo Sangiori <ugorox@gmail.com>
   Andreas Cord-Landwehr <cola@uni-paderborn.de>

   Rocs is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   Rocs is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Step; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
#include "MoveData.h"
#include "GraphScene.h"
#include "DataStructure.h"
#include "DocumentManager.h"
#include "Data.h"
#include "Document.h"
#include "DataItem.h"
#include <KLocale>
#include <QGraphicsView>

#include <KDebug>
//#include "settings.h"

MoveDataAction::MoveDataAction(GraphScene *scene, QObject *parent)
        : AbstractAction(scene, parent) {
    setText(i18n ( "Move Node" ));
    setToolTip ( i18n ( "Moves a node around the drawing area." ) );
    setIcon ( KIcon ( "pointer" ) );
    _movableNode = 0;
    _name = "move";
}

MoveDataAction::~MoveDataAction() {
    kDebug() << "Destroyed";
}

void MoveDataAction::executePress(QPointF pos) {
    if ( !DocumentManager::self()->activeDocument()->activeDataStructure()) {
        return;
    }
    _movableNode = qgraphicsitem_cast<DataItem*>(_graphScene->itemAt(pos));
    if (!_movableNode ) return;

    _data = _movableNode->data();
    if (_data){
        qDebug() << "Got the data";
    }
}

void MoveDataAction::executeMove(QPointF pos) {
    if ( ! _movableNode ) {
        return;
    }
    if (!DocumentManager::self()->activeDocument()->isPointAtDocument(pos)) {
        if (pos.x() < DocumentManager::self()->activeDocument()->xLeft()) {
            pos.setX(DocumentManager::self()->activeDocument()->xLeft());
        }
        if (pos.x() > DocumentManager::self()->activeDocument()->xRight()) {
            pos.setX(DocumentManager::self()->activeDocument()->xRight());
        }
        if (pos.y() < DocumentManager::self()->activeDocument()->yTop()) {
            pos.setY(DocumentManager::self()->activeDocument()->yTop());
        }
        if (pos.y() > DocumentManager::self()->activeDocument()->yBottom()) {
            pos.setY(DocumentManager::self()->activeDocument()->yBottom());
        }
    }

    _data -> setPos(pos.x(), pos.y());
}

void MoveDataAction::executeRelease(QPointF pos) {
    if ( !_movableNode ) {
        return;
    }
    if (DocumentManager::self()->activeDocument()->isPointAtDocument(pos)) {
        _data -> setY(pos.y());
        _data -> setX(pos.x());
    }
    _movableNode = 0;
}
