#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <additional_includes.h>
#include <polydata.h>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:

    QPointF *fstInter ;

    polyData *data ;
    QGraphicsScene *scene ;
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private slots:
    void on_viewHsh_clicked(bool checked);

    void on_viewEnv_clicked(bool checked);

    void on_dSld_valueChanged(int value);

    void on_alphaSld_valueChanged(int value);

private:
    void resetGUI();

    float distance, angle ;

    bool displayHatches ;
    void drawHsh(bool b);

    bool dispEnv ;

    Ui::Dialog *ui;

    QGraphicsPolygonItem *envItem ;
    QGraphicsPolygonItem *polyItem ;


    QTransform* t ;
    QPolygonF* aux ;
    QPolygonF* vPoly ;
    QString filename ;

    QGraphicsEllipseItem* centre;
    bool centerVisible ;

signals:


    void distanceChanged(float d);
    void angleChanged(float a);

private slots:
    void updateDistance(float d);
    void updateAngle(float a);

    void on_loadBut_clicked();
    void on_checkBox_toggled(bool checked);
};

#endif // DIALOG_H
