#include "settings.h"

Settings::Settings(QList<double> change, QMainWindow* parent): QMainWindow (nullptr)
{
    this->setFixedSize(400, 520);
    _parent = parent;
    ///Set Inputfield for N
    _setN = new QDoubleSpinBox();
    _setN->setParent(this);
    _setN->setDecimals(0);
    _setN->setMaximum(10000000);
    _setN->setSingleStep(10000);
    _setN->setValue(change[0]);
    _setN->setGeometry(200,40,160,40);
    _setN->show();

    _NText = new QLabel("N:", this);
    _NText->setGeometry(40,40,160,40);
    _NText->show();

    ///Set Inputfield for Outpressure
    _setOut = new QDoubleSpinBox();
    _setOut->setParent(this);
    _setOut->setMaximum(1000);
    _setOut->setValue(change[1]);
    _setOut->setSingleStep(10);
    _setOut->setGeometry(200,120,160,40);
    _setOut->show();

    _OutText = new QLabel("Outpressure:", this);
    _OutText->setGeometry(40,120,160,40);
    _OutText->show();

    ///Set Inputfield for Kappa
    _setKap = new QDoubleSpinBox();
    _setKap->setParent(this);
    _setKap->setDecimals(10);
    _setKap->setValue(change[2]);
    _setKap->setSingleStep(0.0000000001);
    _setKap->setGeometry(200,200,160,40);
    _setKap->show();

    _KapText = new QLabel("kappa:", this);
    _KapText->setGeometry(40,200,160,40);
    _KapText->show();

    ///Set Inputfield for Secretion rate
    _setSR = new QDoubleSpinBox();
    _setSR->setParent(this);
    _setSR->setDecimals(4);
    _setSR->setValue(change[3]);
    _setSR->setSingleStep(0.0001);
    _setSR->setGeometry(200,280,160,40);
    _setSR->show();

    _SRText = new QLabel("secretion rate", this);
    _SRText->setGeometry(40,280,160,40);
    _SRText->show();

    ///Set Inputfield for Central vein radius
    _setCVR = new QDoubleSpinBox();
    _setCVR->setParent(this);
    _setCVR->setDecimals(6);
    _setCVR->setValue(change[4]);
    _setCVR->setSingleStep(0.000001);
    _setCVR->setGeometry(200,360,160,40);
    _setCVR->show();

    _CVRText = new QLabel("Central vein radius:", this);
    _CVRText->setGeometry(40,360,160,40);
    _CVRText->show();

    qpb_apply = new QPushButton("apply", this);
    qpb_apply->setGeometry(280,440,80,40);
    qpb_apply->show();

    qpb_exit = new QPushButton("exit", this);
    qpb_exit->setGeometry(40,440,80,40);
    qpb_exit->show();

    QObject::connect(qpb_exit,SIGNAL(pressed()),this,SLOT(close()));
    QObject::connect(qpb_apply,SIGNAL(pressed()),this,SLOT(apply()));
    QObject::connect(qpb_exit,SIGNAL(pressed()), this, SLOT(reopen()));
    QObject::connect(this, SIGNAL(delMe(Settings*)), parent, SLOT(delsets(Settings*)));
}

Settings::~Settings(){std::cout<<"del sets"<<std::endl;}

///apply the values to the Model
void Settings::apply()
{
    tmp = {_setN->value(),_setOut->value(),_setKap->value(),_setSR->value(), _setCVR->value()};

    if(tmp.size() == 0)
    {
        emit getSets({1000000,100,3E-10,1.7E-3,20E-6});
    }else{
        emit getSets(tmp);
    }
    reopen();
}

void Settings::reopen()
{
    this->close();
    _parent->setDisabled(false);
    emit delMe(this);
}
