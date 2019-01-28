#ifndef SETTINGS_H
#define SETTINGS_H

#include "model.h"
#include <QObject>
#include <QtWidgets>

class Settings : public QMainWindow
{
    Q_OBJECT
public:
    Settings(Model*);
    ~Settings();
private slots:
    void apply();

private:
    QDoubleSpinBox* _setL;
    QDoubleSpinBox* _setTau;
    QDoubleSpinBox* _setPc;
    QDoubleSpinBox* _setPp;

    QPushButton* qpb_exit;
    QPushButton* qpb_apply;

    QLabel* _LText;
    QLabel* _TauText;
    QLabel* _PcText;
    QLabel* _PpText;

    Model* toChange;
};

#endif // SETTINGS_H
