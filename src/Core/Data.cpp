/*
    This file is part of Rocs.
    Copyright 2004-2011  Tomaz Canabrava <tomaz.canabrava@gmail.com>
    Copyright 2012       Andreas Cord-Landwehr <cola@uni-paderborn.de>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "Data.h"
#include "Pointer.h"
#include "DataStructure.h"
#include <KDebug>
#include <KGlobal>
#include <QColor>
#include <QMap>

#include "DynamicPropertiesList.h"
#include "DataType.h"

class DataPrivate
{
public:
    DataPrivate(DataStructurePtr parent, int uniqueIdentifier, int dataType);

    /**
     * self pointer to Data object
     */
    boost::weak_ptr<Data> q;

    PointerList _inPointers;
    PointerList _outPointers;

    qreal _x;
    qreal _y;
    qreal _width;
    bool _showName;
    bool _showValue;
    bool _visible;
    bool _colored;

    DataStructurePtr _dataStructure;
    int _uniqueIdentifier;
    int _dataType;
    QString _name;
    QColor _color;

    QVariant _value;
    QScriptValue _scriptvalue;
    QScriptEngine *_engine;

    void empty(PointerList &list);

    void removePointer(PointerPtr e, PointerList &list);

    /**
     * Create a script value array out of a pointer list.
     *
     * \param list is a list of pointers
     * \return array of script value representation of the pointers
     */
    QScriptValue createScriptArray(PointerList list);
};

DataPrivate::DataPrivate(DataStructurePtr parent, int uniqueIdentifier, int dataType)
    : _x(0)
    , _y(0)
    , _width(0.3)
    , _showName(parent->isDataNameVisible(dataType))
    , _showValue(parent->isDataValueVisible(dataType))
    , _visible(parent->isDataVisible(dataType))
    , _colored(false)
    , _dataStructure(parent)
    , _uniqueIdentifier(uniqueIdentifier)
    , _dataType(dataType)
    , _color(parent->document()->dataType(dataType)->defaultColor())
    , _value(0)
{
    _inPointers = PointerList();
    _outPointers = PointerList();
}


void DataPrivate::empty(PointerList &list)
{
    while (!list.isEmpty()) {
        list.first()->remove();
    }
}

void DataPrivate::removePointer(PointerPtr e, PointerList &list)
{
    if (list.contains(e)) {
        list.removeOne(e);
    }
}

QScriptValue DataPrivate::createScriptArray(PointerList list)
{
    QScriptValue array = _engine->newArray();
    foreach(PointerPtr e, list) {
        array.property("push").call(array, QScriptValueList() << e->scriptValue());
    }
    return array;
}



DataPtr Data::create(DataStructurePtr dataStructure, int uniqueIdentifier, int dataType)
{
    return create<Data>(dataStructure, uniqueIdentifier, dataType);
}

void Data::setQpointer(DataPtr q)
{
    d->q = q;
}

DataPtr Data::getData() const
{
    DataPtr px(d->q);
    return px;
}

Data::Data(DataStructurePtr dataStructure, int uniqueIdentifier, int dataType)
    :  QObject(dataStructure.get()),
       d(new DataPrivate(dataStructure, uniqueIdentifier, dataType))
{
}

Data::~Data()
{
    emit removed();

    if (d) {
        d->empty(d->_inPointers);
        d->empty(d->_outPointers);
    }
}

DataStructurePtr Data::dataStructure() const
{
    return d->_dataStructure;
}

void Data::setShowName(bool b)
{
    if (d->_showName != b) {
        d->_showName = b;
        emit nameVisibilityChanged(b);
    }
}

void Data::setShowValue(bool b)
{
    if (d->_showValue != b) {
        d->_showValue = b;
        emit valueVisibilityChanged(b);
    }
}

void Data::setColored(bool b)
{
    if (d->_colored != b) {
        d->_colored = b;
        emit useColorChanged(b);
    }
}

bool Data::isVisible() const
{
    return d->_visible;
}

void Data::setVisible(bool visible)
{
    d->_visible = visible;
    emit visibilityChanged(visible);
}

void Data::setDataType(int dataType)
{
    Q_ASSERT(d->_dataStructure->document()->dataTypeList().contains(dataType));
    d->_dataType = dataType;
    d->_dataStructure->updateData(getData());
    emit dataTypeChanged(dataType);
}

DataList Data::adjacentDataList() const
{
    // use QMap as the DataPtr elements are not hashable
    // and we can speed up process by using the uniqe IDs
    QMap<int, DataPtr> adjacent;

    foreach(PointerPtr e, d->_outPointers) {
        adjacent[e->to()->identifier()] = e->to();
    }

    foreach(PointerPtr e, d->_inPointers) {
        adjacent[e->from()->identifier()] = e->from();
    }

    return adjacent.values();
}

PointerList Data::pointerList() const
{
    PointerList adjacent;

    adjacent << d->_outPointers;
    adjacent << d->_inPointers;

    return adjacent;
}

void Data::registerInPointer(PointerPtr e)
{
    Q_ASSERT(e->to()->identifier() == identifier() || e->from()->identifier() == identifier());
    d->_inPointers.append(e);
}

void Data::registerOutPointer(PointerPtr e)
{
    Q_ASSERT(e->to()->identifier() == identifier() || e->from()->identifier() == identifier());
    d->_outPointers.append(e);
}

PointerPtr Data::addPointer(DataPtr to)
{
    return d->_dataStructure->addPointer(this->getData(), to);
}

void Data::removePointer(PointerPtr e)
{
    // removes pointer from any list that could contain it
    d->removePointer(e, d->_inPointers);
    d->removePointer(e, d->_outPointers);
}

PointerList Data::pointerList(DataPtr to) const
{
    PointerList list;
    foreach(PointerPtr tmp, d->_outPointers) {
        if (tmp->to() == to) {
            list.append(tmp);
        }
    }
    foreach(PointerPtr tmp, d->_inPointers) {
        if (tmp->from() == to) {
            list.append(tmp);
        }
    }
    return list;
}

void Data::remove()
{
    if (d->_dataStructure) {
        d->_dataStructure->remove(getData());
        d->_dataStructure.reset();  // allow data structure to be destroyed
    }

    d->empty(d->_inPointers);
    d->empty(d->_outPointers);

    emit removed();
}

const QVariant Data::value() const
{
    return d->_value;
}

const QString& Data::name()  const
{
    return d->_name;
}

const QVariant Data::color() const
{
    return d->_color;
}

qreal Data::x() const
{
    return d->_x;
}

qreal Data::y() const
{
    return d->_y;
}

qreal Data::width() const
{
    return d->_width;
}

QString Data::icon() const
{
    return d->_dataStructure->document()->dataType(d->_dataType)->iconName();
}

PointerList& Data::inPointerList() const
{
    return d->_inPointers;
}

PointerList& Data::outPointerList() const
{
    return d->_outPointers;
}

bool Data::isNameVisible() const
{
    return d->_showName;
}

bool Data::isValueVisible() const
{
    return d->_showValue;
}

bool Data::isColored() const
{
    return d->_colored;
}

int Data::identifier() const
{
    return d->_uniqueIdentifier;
}

int Data::dataType() const
{
    return d->_dataType;
}

void Data::setX(int x)
{
    if (d->_x != x) {
        d->_x = x;
        emit posChanged(QPointF(d->_x, d->_y));
    }
}

void Data::setY(int y)
{
    if (d->_y != y) {
        d->_y  = y;
        emit posChanged(QPointF(d->_x, d->_y));
    }
}

void Data::setWidth(double w)
{
    if (d->_width != w) {
        d->_width = (qreal)w;
        emit widthChanged(w);
    }
}

void Data::setPos(qreal x, qreal y)
{
    if (d->_x != x || d->_y != y) {
        d->_x = x;
        d->_y = y;
        emit posChanged(QPointF(d->_x, d->_y));
    }
}

void Data::setColor(const QVariant& s)
{
    QColor c = s.value<QColor>();
    if (d->_color != c) {
        d->_color = c;
        emit colorChanged(c);
    }
}

void Data::setName(const QString& s)
{
    if (d->_name != s) {
        d->_name = s;
        emit nameChanged(s);
    }
}

void  Data::setValue(const QVariant& s)
{
    if (d->_value != s) {
        d->_value = s;
        emit valueChanged(s);
    }
}

void  Data::setValue(const QString& s)
{
    QVariant v(s);
    if (d->_value != v) {
        d->_value = v;
        emit valueChanged(v);
    }
}

void Data::addDynamicProperty(QString property, QVariant value)
{
    if (! setProperty(property.toUtf8(), value)  && value.isValid()) {
        DynamicPropertiesList::New()->addProperty(this, property);
    }
}

void Data::removeDynamicProperty(QString property)
{
    addDynamicProperty(property.toUtf8(), QVariant::Invalid);
    DynamicPropertiesList::New()->removeProperty(this, property);
}

QScriptValue Data::scriptValue() const
{
    return d->_scriptvalue;
}

void Data::setEngine(QScriptEngine *engine)
{
    d->_engine = engine;
    d->_scriptvalue = engine->newQObject(getData().get());
}

QScriptValue Data::set_type(int dataType)
{
    if (!d->_dataStructure->document()->dataTypeList().contains(dataType)) {
        kError() << "Specified data type '" << dataType << "' does not exist."; //TODO give script error
        return d->_dataStructure->engine()->newVariant(false);
    }
    setDataType(dataType);
    return d->_dataStructure->engine()->newVariant(true);
}

QScriptValue Data::type()
{
    return d->_dataStructure->engine()->newVariant(d->_dataType);
}

void Data::add_property(QString name, QString value)
{
    addDynamicProperty(name, value);
}

QScriptValue Data::adj_data()
{
    QList< DataPtr > list = adjacentDataList();
    QScriptValue array = d->_engine->newArray();
    foreach(DataPtr n, list) {
        array.property("push").call(array, QScriptValueList() << n->scriptValue());
    }
    return array;
}

QScriptValue Data::adj_pointers()
{
    PointerList list = pointerList();
    return d->createScriptArray(list);
}

QScriptValue Data::adj_pointers(int pointerType)
{
    PointerList list;
    foreach(PointerPtr n, pointerList()) {
        if (n->pointerType() != pointerType) {
            continue;
        }
        list.append(n);
    }
    return d->createScriptArray(list);
}

QScriptValue Data::input_pointers()
{
    PointerList list = inPointerList();
    return d->createScriptArray(list);
}

QScriptValue Data::output_pointers()
{
    PointerList list = outPointerList();
    return d->createScriptArray(list);
}

QScriptValue Data::output_pointers(int pointerType)
{
    PointerList list;
    foreach(PointerPtr n, outPointerList()) {
        if (n->pointerType() != pointerType) {
            continue;
        }
        list.append(n);
    }
    return d->createScriptArray(list);
}

QScriptValue Data::connected_pointers(DataPtr n)
{
    PointerList list = pointerList(n);
    return d->createScriptArray(list);
}
