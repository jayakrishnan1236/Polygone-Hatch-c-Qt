#include "dialog.h"
#include "ui_dialog.h"
//#include "additional_includes.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    this->setWindowTitle("Lab01-GMT-6005-Khalil Ben Fadhel");
    displayHatches = false ;
    dispEnv = false ;
    ui->viewHsh->setChecked(displayHatches);
    ui->viewEnv->setChecked(dispEnv);

    ui->dSld->setMaximum(160);
    centerVisible = false ;

    ui->checkBox->setChecked(centerVisible);

    data = new polyData();

    data->polygonF = new QPolygonF();
    data->rotatedPoly = new QPolygonF();
    t = new QTransform();
    scene = new QGraphicsScene(this);
    data->envelopF = new QPolygonF();
    data->envHsh = new QVector<QLineF>();
    data->aretesPoly = new QVector<QLineF>();
    data->finalHsh = new QVector<QLineF>();
    data->centroid = new QPointF() ;
    data->clippedHsh = new QVector<QLineF>();

    angle = 0 ;

    distance = 10 ;
    QMessageBox::information(0, "Choisir un fichier", "Veuillez choisir un fichier texte, contenant les coordonnées des sommets d'un polygone. Les coordonnées doivent être separées par des espaces, et les sommets separes par un retour à la ligne");

    on_loadBut_clicked();

    ui->graphicsView->scale(4,-4);


    connect(this, SIGNAL(distanceChanged(float)), this , SLOT(updateDistance(float)));
    connect(this, SIGNAL(angleChanged(float)), this , SLOT(updateAngle(float)));


    // Ã  mettre dans l'openfile
    emit angleChanged(angle);
    emit distanceChanged(distance);



}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_viewHsh_clicked(bool checked)
{
    displayHatches = checked ;
    scene->clear();

    drawHsh(displayHatches);
    polyItem = scene->addPolygon(*data->polygonF, QPen(Qt::black,0.3f));


    data->envelopF->clear();
    polyProc::enveloppe(data->polygonF, data->envelopF);


    envItem = scene->addPolygon(*data->envelopF, QPen(Qt::red));
    envItem->setVisible(dispEnv);

    centre = scene->addEllipse(data->centroid->x()-1 , data->centroid->y()-1, 2, 2, QPen(Qt::black), QBrush(Qt::green));
    centre->setVisible(centerVisible);
}

void Dialog::drawHsh(bool b)
{
    if(b){

        if(!data->clippedHsh->isEmpty()){

            t->reset();
            t->rotate(angle);

            QLineF lineAux ;

            for (int  i = 0 ; i< data->clippedHsh->size(); i++){

                lineAux = t->map( (*(data->clippedHsh))[i] );
                scene->addLine(lineAux, QPen(Qt::blue));
            }




            qDebug() << lineAux;


            //scene->addEllipse(fstInter->x()-2 , fstInter->y()-2 , 4 ,4 );


        }
    }
}

void Dialog::updateDistance(float d)
{

    distance = d ;
    polyProc::hatchEnvelop(data->envelopF, distance , data->envHsh);


    data->clippedHsh->clear();
    polyProc::hatchPolygonHoriz(data->aretesPoly, data->envHsh, data->clippedHsh);

    emit angleChanged(angle);


}

void Dialog::updateAngle(float a)
{
    angle = a  ;


    if ((angle == 26)||(angle == 92)||(angle == 82)||(angle == 25)){
        angle = angle -0.1f  ;
    }
    if (angle == 98){
        angle = angle -0.7f ;
    }
    if (angle == 67){
        angle = angle -0.2f ;
    }
    if (angle == 71){
        angle = angle -0.2f ;
    }




    scene->clear();


    t->reset();
    t->rotate(-angle);
    (*data->rotatedPoly) = t->map(*data->polygonF);

    data->envelopF->clear();
    polyProc::enveloppe(data->rotatedPoly, data->envelopF);

    envItem = scene->addPolygon(*data->envelopF, QPen(Qt::blue, 1));
    envItem->setVisible(dispEnv);

    polyProc::hatchEnvelop(data->envelopF, distance , data->envHsh);

    data->aretesPoly->clear();
    polyProc::poly2lines(data->rotatedPoly, data->aretesPoly);

    polyProc::hatchEnvelop(data->envelopF, distance , data->envHsh);

    data->clippedHsh->clear();
    polyProc::hatchPolygonHoriz(data->aretesPoly, data->envHsh, data->clippedHsh);

    on_viewHsh_clicked(displayHatches);



}

void Dialog::on_viewEnv_clicked(bool checked)
{
    dispEnv = checked ;
    envItem->setVisible(dispEnv);
}

void Dialog::on_dSld_valueChanged(int value)
{
    emit distanceChanged(value);
}

void Dialog::on_alphaSld_valueChanged(int value)
{
    emit angleChanged(value);
}

void Dialog::resetGUI()//polygon already filled
{
    //ui->graphicsView =new QGraphicsView(this);

    polyProc::computeCentroid(data->polygonF , data->centroid);
    data->inOut = polyProc::pointInPolygon(*(data->centroid), *data->polygonF);
    //qDebug() << data->inOut ;
    if (data->inOut){
        ui->positionInOut->setText("interieur");
    }else{
        ui->positionInOut->setText("exterieur");
    }


    data->surface = 0 ;
    polyProc::computeSurface(data->polygonF, data->surface);
    ui->surfLabel->setText(QString::number(data->surface));

    ui->alphaSld->setValue(angle);
    ui->dSld->setValue(distance);


    ui->xClabel->setText(QString::number((data->centroid->x())));
    ui->yClab->setText(QString::number((data->centroid->y())));

    ui->graphicsView->setScene(scene);

    centre = scene->addEllipse(data->centroid->x()-1 , data->centroid->y()-1, 2, 2, QPen(Qt::black), QBrush(Qt::green));

}

void Dialog::on_loadBut_clicked()
{
    vPoly = new QPolygonF();
    filename = QFileDialog::getOpenFileName(this,
                                            tr("open file"),
                                            "./",
                                            "text file (*.txt)");
    polyProc::loadPolygon(data->polygonF, filename);
    resetGUI();
    //ui->graphicsView->fitInView(polyItem);



    emit angleChanged(angle);



}

void Dialog::on_checkBox_toggled(bool checked)
{

    centerVisible = checked ;
    centre->setVisible(centerVisible);

}
