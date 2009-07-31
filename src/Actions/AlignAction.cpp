/* This file is part of Rocs,
   Copyright (C) 2008 by:
   Tomaz Canabrava <tomaz.canabrava@gmail.com>
   Ugo Sangiori <ugorox@gmail.com>

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
#include "AlignAction.h"
#include <KIcon>
#include "GraphVisualEditor.h"
#include "GraphScene.h"
#include "generics.h"
#include <KDebug>

AlignAction::AlignAction(const QString& actionName, const QString& tooltip,AlignAction::Orientation o, QWidget *parent)
  : KAction(KIcon(actionName), tooltip, parent){
  m_orientation = o;
  connect(this, SIGNAL(triggered()), this, SLOT(align()));
    kDebug() << " Allign Action Created!";
}

void AlignAction::align(){
  GraphVisualEditor *gEditor = qobject_cast<GraphVisualEditor*>(parent());
  QList<NodeItem*> l = gEditor->selectedNodes();
  
  if (l.size() < 1) return;
  gEditor->scene()->setHideEdges(true);
  switch(m_orientation){
    case Left :    case MiddleVertical : 
	  qSort(l.begin(), l.end(),  leftLessThan);
	  allignX(l); 
    break;
    case Bottom :    case MiddleHorizontal : 
     qSort(l.begin(), l.end(), bottomLessThan);
     allignY(l); 
    break;
    case Right : 
      qSort(l.begin(), l.end(), rightLessThan);
      allignX(l);
    break;
    case Top :  
	qSort( l.begin(), l.end(), topLessThan);
	allignY(l);
      break;
  }

    gEditor->scene()->setHideEdges(false);
}

void AlignAction::allignY(QList<NodeItem*>& l){
  qreal final = l[0]->pos().y();
  if (m_orientation == MiddleHorizontal || m_orientation == MiddleVertical){
    qreal otherSide = l[l.size()-1]->pos().y();
     final = (final + otherSide) / 2;
  }
  foreach(NodeItem *i, l) {
    i->node()->setY(final);
  }
}

void AlignAction::allignX(QList<NodeItem*>& l){
  qreal final = l[0]->pos().x();
  if (m_orientation == MiddleHorizontal || m_orientation == MiddleVertical){
    qreal otherSide = l[l.size()-1]->pos().x();
     final = (final + otherSide) / 2;
  }
  foreach(NodeItem *i, l) {
    i->node()->setX(final);
  }
}
