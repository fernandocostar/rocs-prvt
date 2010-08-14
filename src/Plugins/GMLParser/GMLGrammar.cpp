/* This file is part of Rocs.
   Copyright (C) 2010 Wagner Reck <wagner.reck@gmail.com>

   Rocs is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation, version 2.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
   02110-1301, USA
*/

#include "GMLGrammar.h"

#include "GMLGraphParsingHelper.h"
#include "graphDocument.h"

#include <kdebug.h>


#define KGV_MAX_ITEMS_TO_LOAD std::numeric_limits<size_t>::max()
#define BOOST_SPIRIT_DEBUG 1

namespace Rocs{
namespace GMLPlugin{

GMLGraphParsingHelper* phelper = 0;
std::string lastKey = "";
QObject * lastInserted = 0;
Graph * actualGraph = 0;
Node * actualNode = 0;
Edge * actualEdge = 0;

GraphDocument *document = 0;

void beginList(){
    phelper->startList(QString::fromStdString(lastKey));
}

void endList(){
  phelper->endList();
}

void gotKey(const std::string& key){
  lastKey = key.c_str();
//   QString k = key.c_str();
//   if (k.compare("graph", Qt::CaseInsensitive) == 0){
//     kDebug() << "create a graph";
//     actualGraph =  document->addGraph("new");
//     lastInserted = actualGraph;
//   }else if (k.compare("node", Qt::CaseInsensitive) == 0){
//     kDebug() << "create a node";
//     actualNode = actualGraph->addNode("new");
//     lastInserted = actualNode;
//   }else if (k.compare("edge", Qt::CaseInsensitive) == 0){
//     kDebug() << "create a edge";
//   }else {
//     kDebug() << "Process atribute " << k;
//     lastKey = key;
//   }

}

void gotValue(const std::string& Value){
  if (Value == ""){
    return; //end of the list.
  }else{
    phelper->setAtribute(QString::fromStdString(lastKey), QString::fromStdString(Value));
//     if (lastInserted){
//       if (lastKey == "id"){
//         lastInserted->setProperty("name", Value.c_str());
//       }
//       lastInserted->setProperty(lastKey.c_str(), Value.c_str());
//     }else{
//       kDebug() << "ignoring keyvalue: "<< lastKey.c_str() << Value.c_str();
//     }
  }
}


    void t(){
      std::cout << "Found whitespace.\n";
    }
    void t1(const std::string &key){
      std::cout << "Found " << key << ".\n";
    }

bool parse( QString &content, GraphDocument * doc){
    unsigned result;
    phelper = new GMLGraphParsingHelper;
    phelper->gd = doc;
    typedef std::string::const_iterator iterator_type;
    typedef GMLPlugin::roman<iterator_type> roman;

    roman roman_parser; // Our grammar

//     std::string content = file.readAll().data();
//     kDebug() << QString::fromStdString(content);
    int index;
    while ( ( index = content.indexOf('#') ) != -1){
      content.remove(index, content.indexOf('\n', index) - index);
    }
    std::string str = content.toStdString();
    iterator_type iter = str.begin();
    iterator_type end = str.end();

    bool r = parse(iter, end, roman_parser, result);

        if (r && iter == end)
        {
            std::cout << "-------------------------\n";
            std::cout << "Parsing succeeded\n";
            std::cout << "result = " << result << std::endl;
            std::cout << "-------------------------\n";
        }
        else
        {
            std::string rest(iter, end);
            std::cout << "-------------------------\n";
            std::cout << "Parsing failed\n";
            std::cout << "stopped at: \": " << rest << "\"\n";
            std::cout << "-------------------------\n";
        }

        delete phelper;
        return r;
}
}
}