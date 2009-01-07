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
#ifndef KROSSBACKEND_H
#define KROSSBACKEND_H

#include <QVariant>

#include <kross/core/action.h>

class KrossBackend {
public:
  KrossBackend(QVariantList  *graphs);
  void setScript(const QString& s);
  void setBackend(const QString& backend);
  Kross::Action* execute();
  void loadFile(const QString& file);

private:
  void javaDefaults();
  void raptorDefaults();
  void pyDefaults();
  void rbDefaults();
  void jsDefaults();
  void luaDefaults();
  void csDefaults();
  
  QString _backend;
  QString _script;
  QString _autoGeneratedScript;

  QVariantList *_graphs;
};

#endif