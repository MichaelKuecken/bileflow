#ifndef SETTINGS_H
#define SETTINGS_H

#include "model.h"
#include <QObject>
#include <QThread>
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
    QDoubleSpinBox* _setN;
    QDoubleSpinBox* _setOut;
    QDoubleSpinBox* _setKap;
    QDoubleSpinBox* _setSR;
    QDoubleSpinBox* _setCVR;

    QPushButton* qpb_exit;
    QPushButton* qpb_apply;

    QLabel* _NText;
    QLabel* _OutText;
    QLabel* _KapText;
    QLabel* _SRText;
    QLabel* _CVRText;

    Model* toChange;
};

#endif // SETTINGS_H
