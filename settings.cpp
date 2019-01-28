#include "settings.h"

Settings::Settings(Model* change): QMainWindow (nullptr)
{

    this->setGeometry(this->pos().x(), this->pos().y(),320,440);
    _setL = new QDoubleSpinBox();
    _setL->setParent(this);
    _setL->setDecimals(5);
    _setL->setSingleStep(0.00005);
    _setL->setValue(change->L);
    _setL->setGeometry(120,40,160,40);
    _setL->show();

    _LText = new QLabel("L:", this);
    _LText->setGeometry(40,40,80,40);
    _LText->show();

    _setTau = new QDoubleSpinBox();
    _setTau->setParent(this);
    _setTau->setValue(change->tau);
    _setTau->setSingleStep(0.05);
    _setTau->setGeometry(120,120,160,40);
    _setTau->show();

    _TauText = new QLabel("Tau:", this);
    _TauText->setGeometry(40,120,80,40);
    _TauText->show();

    _setPc = new QDoubleSpinBox();
    _setPc->setParent(this);
    _setPc->setValue(change->vilifac_pc);
    _setPc->setSingleStep(0.05);
    _setPc->setGeometry(120,200,160,40);
    _setPc->show();

    _PcText = new QLabel("vilifac-pc:", this);
    _PcText->setGeometry(40,200,80,40);
    _PcText->show();

    _setPp = new QDoubleSpinBox();
    _setPp->setParent(this);
    _setPp->setValue(change->vilifac_pp);
    _setPp->setMinimum(-5);
    _setPp->setSingleStep(0.05);
    _setPp->setGeometry(120,280,160,40);
    _setPp->show();

    _PpText = new QLabel("vilifac-pp:", this);
    _PpText->setGeometry(40,280,80,40);
    _PpText->show();

    qpb_apply = new QPushButton("apply", this);
    qpb_apply->setGeometry(200,360,80,40);
    qpb_apply->show();

    qpb_exit = new QPushButton("exit", this);
    qpb_exit->setGeometry(40,360,80,40);
    qpb_exit->show();

    toChange = change;
    QObject::connect(qpb_exit,SIGNAL(pressed()),this,SLOT(close()));
    QObject::connect(qpb_apply,SIGNAL(pressed()),this,SLOT(apply()));
}

Settings::~Settings(){}

void Settings::apply()
{
    toChange->setVal(_setL->value(),_setTau->value(),_setPc->value(),_setPp->value());
    this->close();
}
