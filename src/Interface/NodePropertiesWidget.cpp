#include "NodePropertiesWidget.h"
#include "Data.h"
#include "MainWindow.h"
#include <KDebug>
#include "DataItem.h"
#include "DataStructure.h"
#include "model_GraphProperties.h"
#include <DataStructurePluginManager.h>

DataPropertiesWidget::DataPropertiesWidget (MainWindow* /*parent*/  ): QWidget(0) {
    setupUi(this);
    _item = 0;
    _data = 0;
}

void DataPropertiesWidget::setData(DataItem *n, QPointF pos) {
    if (_data == n->data())
      return;

    if (_data){
      disconnectData(_data);
    }

    _data = n->data();
    if (! _item ){
      _svgFile = _data->iconPackage();
    }

    _item = n;
    move(pos.x()+ 10,  pos.y() + 10);

    show(); activateWindow(); raise();

    reflectAttributes();


    connect(_data, SIGNAL(changed()), this, SLOT(reflectAttributes()));
//     connect(_data->parent(), SIGNAL(automateChanged(bool)), this, SLOT(updateAutomateAttributes(bool)));

    connect( _showName,     SIGNAL(toggled(bool)),          _data, SLOT(hideName(bool)));
    connect( _showValue,    SIGNAL(toggled(bool)),          _data, SLOT( hideValue(bool)));
//     connect( _begin,        SIGNAL(toggled(bool)),          _data, SLOT(setBegin(bool)));
//     connect( _end,          SIGNAL(toggled(bool)),          _data, SLOT(setEnd(bool)));
    connect( _name,         SIGNAL(textChanged(QString)),   _data, SLOT(setName(QString)));
    connect( _value,        SIGNAL(textChanged(QString)),   _data, SLOT(setValue(QString)));
//     connect( _x,            SIGNAL(valueChanged(int)),      _data, SLOT(setX(int)));
//     connect( _y,            SIGNAL(valueChanged(int)),      _data, SLOT(setY(int)));
//     connect( _width,        SIGNAL(valueChanged(double)),   _data, SLOT(setWidth(double)));

    GraphPropertiesModel *model = new GraphPropertiesModel();
    model->setDataSource(_data);

    _propertiesTable->setModel(model);

}

void DataPropertiesWidget::reflectAttributes(){
    if (extraItens->layout()){
      delete extraItens->layout();
    }
    if(QLayout *lay = DataStructurePluginManager::self()->dataExtraProperties(_data, this)){
      extraItens->setLayout(lay);
    }
   _color->setColor(_data->color());
//    _x->setValue(_data->x());
//    _y->setValue(_data->y());
   _name->setText(_data->name());
   _value->setText(_data->value().toString());
//    _width->setValue(_data->width());
   _showName->setChecked(_data->showName());
   _showValue->setChecked(_data->showValue());
//    updateAutomateAttributes(qobject_cast< Graph* >(_data->parent())->automate());
   _propertyName->setText("");
   _propertyValue->setText("");
   _isPropertyGlobal->setCheckState(Qt::Unchecked);
   if (( _svgFile == _data->iconPackage()) && (_images->count() != 0)){
      kDebug() << _svgFile << "already set, and images combo box is not empty";
      return;
   }
    _images->clear();
    QFile svgFile(_item->data()->iconPackage());
    if (!svgFile.open(QIODevice::ReadOnly | QIODevice::Text)){
      kDebug() << "could not open file for reading";
      return;
    }

    QXmlStreamReader reader(&svgFile);
    while(!reader.atEnd()){
      reader.readNext();
      if ( reader.attributes().hasAttribute("id")){
          QString attribute = reader.attributes().value("id").toString();
          if (attribute.startsWith("rocs_")){
              attribute.remove("rocs_");
              _images->addItem(attribute);
          }
      }
    }
}

void DataPropertiesWidget::on__images_activated(const QString& s)
{
  _data->setIcon("rocs_"+s);
}

void DataPropertiesWidget::on__color_activated(const QColor& c) {
  _data->setColor(c.name());
}

// void DataPropertiesWidget::updateAutomateAttributes(bool b){
// //     if (b) {
// //         _begin->setChecked(_data->begin());
// //         _end->setChecked(_data->end());
// //         _begin->show();
// //         _end->show();
// //     }
// //     else {
// //         _begin->setChecked(false);
// //         _end->setChecked(false);
// //         _begin->hide();
// //         _end->hide();
// //     }
// }

void DataPropertiesWidget::on__addProperty_clicked(){

    GraphPropertiesModel *model =  qobject_cast< GraphPropertiesModel*>(_propertiesTable->model());
    model->addDynamicProperty(_propertyName->text(),
                            QVariant(_propertyValue->text()),
                            _data,
                            (_isPropertyGlobal->checkState() == Qt::Checked));

}

void DataPropertiesWidget::disconnectData(Data *n){

    disconnect(n, SIGNAL(changed()), this, SLOT(reflectAttributes()));
    disconnect(n->parent(), SIGNAL(automateChanged(bool)), this, SLOT(updateAutomateAttributes(bool)));

    disconnect( _showName,     SIGNAL(toggled(bool)),          n, SLOT(hideName(bool)));
    disconnect( _showValue,    SIGNAL(toggled(bool)),          n, SLOT( hideValue(bool)));
//     disconnect( _begin,        SIGNAL(toggled(bool)),          n, SLOT(setBegin(bool)));
//     disconnect( _end,          SIGNAL(toggled(bool)),          n, SLOT(setEnd(bool)));
    disconnect( _name,         SIGNAL(textChanged(QString)),   n, SLOT(setName(QString)));
    disconnect( _value,        SIGNAL(textChanged(QString)),   n, SLOT(setValue(QString)));
//     disconnect( _x,            SIGNAL(valueChanged(int)),      n, SLOT(setX(int)));
//     disconnect( _y,            SIGNAL(valueChanged(int)),      n, SLOT(setY(int)));
//     disconnect( _width,        SIGNAL(valueChanged(double)),   n, SLOT(setWidth(double)));

}
