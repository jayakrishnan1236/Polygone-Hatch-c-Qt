#include <PolygonProcessing.h>
#include <additional_includes.h>

#include <math.h>


#include <QDebug>



// donne l'aire algébrique du triangle abc
float polyProc::aireAlgebrique(QPointF a, QPointF b, QPointF c)
{
    return ((b.x()-a.x())*(c.y()-a.y())-(c.x()-a.x())*(b.y()-a.y())) ;
}




void polyProc::enveloppe(QPolygonF *poly, QPolygonF* env)
{
    float mx,my,Mx,My ;

    mx= (*poly)[0].x() ;
    my= (*poly)[0].y() ;
    Mx= mx ;
    My= my ;

    //qDebug()<< mx << " " << my << " " << Mx << " " << My;

    for(int i=0; i< (*poly).size(); i++){
        if((*poly)[i].x()<=mx){
            mx = (*poly)[i].x();
        }
        if((*poly)[i].y()<=my){
            my = (*poly)[i].y();
        }
        if((*poly)[i].x()>=Mx){
            Mx = (*poly)[i].x();
        }
        if((*poly)[i].y()>=My){
            My = (*poly)[i].y();
        }
    }
    //on suppose que env est initialisée déjà

    (*env).clear();
    (*env).push_back(QPointF(mx,my));
    (*env).push_back(QPointF(Mx,my));
    (*env).push_back(QPointF(Mx,My));
    (*env).push_back(QPointF(mx,My));

}


void polyProc::hatchEnvelop(QPolygonF *env, float d, QVector<QLineF> *hatches)
{
    //on suppose que hatcches est déjà initialisé
    (*hatches).clear();
    float mx,my,Mx,My ;

    mx= (*env)[0].x() ;
    my= (*env)[0].y() ;
    Mx= mx ;
    My= my ;

    //qDebug()<< mx << " " << my << " " << Mx << " " << My;

    for(int i=0; i< (*env).size(); i++){
        if((*env)[i].x()<=mx){
            mx = (*env)[i].x();
        }
        if((*env)[i].y()<=my){
            my = (*env)[i].y();
        }
        if((*env)[i].x()>=Mx){
            Mx = (*env)[i].x();
        }
        if((*env)[i].y()>=My){
            My = (*env)[i].y();
        }
    }

    for(float y = my ; y <= My; y = y+d){
        QPointF *p1 = new QPointF(mx,y);
        QPointF *p2 = new QPointF(Mx,y);
        QLineF *line = new QLineF(*p1 , *p2);
        (*hatches).push_back(*line);
        //qDebug()<< *p1 << " " << *p2<< endl ;
    }
}


void polyProc::poly2lines(QPolygonF *poly, QVector<QLineF> *aretes)
{
    int N = (*poly).size();
    (*aretes).clear();

    for (int i = 0 ; i< N ; i++){

        (*aretes).push_back( QLineF((*poly).at(i%N), (*poly).at((i+1)%N))  );

    }

}




//------------------
//- contrainte , La ligne horizontale est orientée de gauche vers la droite
//- pInter est NULL au départ
//------------------
void polyProc::interHorizLine(QLineF *HorizLine, QLineF *Line, QPointF *pInter)
{
    float epsilon = 1.0f ;

    //qDebug() << ((*Line).y1() - (*HorizLine).y1() ) ;
    //qDebug() << ( (*Line).y2() - (*HorizLine).y1() );
    //qDebug() << ( ((*Line).y1() - (*HorizLine).y1())*((*Line).y2() - (*HorizLine).y1()) );

    if ((*Line).y1() - (*HorizLine).y1() == 0 ){
        if ( ( ((*Line).y1() + epsilon - (*HorizLine).y1()) * ((*Line).y2() - (*HorizLine).y1()) )< 0){
            //pInter = new QPointF((*Line).x1(), (*Line).y1());
            pInter->setX((*Line).x1());
            pInter->setY((*Line).y1());
        }
    }
    if (( (*Line).y2() - (*HorizLine).y1() ) == 0 ){
        if (((*Line).y2() + epsilon - (*HorizLine).y1())*((*Line).y1() - (*HorizLine).y1()) < 0){
            //pInter = new QPointF((*Line).x2(), (*Line).y2());
            pInter->setX((*Line).x2());
            pInter->setY((*Line).y2());
        }
    }
    if ( ( ((*Line).y1() - (*HorizLine).y1())*((*Line).y2() - (*HorizLine).y1()) ) < 0){
        float S1 , S2 ;
        S1 = 0 ;
        S2 = 0 ;

        S1 = polyProc::aireAlgebrique((*HorizLine).p1() , (*Line).p1(), (*Line).p2());
        S1 = abs(S1) ;



        S2 = polyProc::aireAlgebrique((*HorizLine).p2() , (*Line).p1(), (*Line).p2());
        S2 = abs(S2) ;

        float lambda = S1 / (S1 + S2) ;

        float x = (*HorizLine).x1() + lambda * ( (*HorizLine).length() );

        /*
        qDebug() << "S1 = " << S1 ;
        qDebug() << "S2 = " << S2 ;
        qDebug() << "lambda " << lambda ;
        qDebug() << "x = " << x;
        */
        //pInter = new QPointF(x , (*HorizLine).y1());
        pInter->setX(x);
        pInter->setY( (*HorizLine).y1());

        //qDebug() << "pInter" << *pInter;

    }


}

//------intersection Hachure Horizontale avec le polygone tout entier
//------deuxieme étape , réordonner les points selon x croissant et tracer les lignes
//------clear useful hatches before call
void polyProc::polyInterHorizHsh(QLineF *HorizLine, QVector<QLineF> *lines, QVector<QLineF> *usefulHsh)
{


    usefulHsh->clear();

    QPointF *p0 = new QPointF();
    QVector<QPointF> points ;


    for (int i = 0 ; i< lines->size(); i++){
        p0 = new QPointF();
        polyProc::interHorizLine(HorizLine, &((*(lines))[i]), p0);
        //qDebug()<< *p0;
        if (!p0->isNull()){
            points.push_back(*p0);

        }
    }
    //qDebug() << points ;

    //sort by x
    if (!points.isEmpty()){
        bool noPermutations = false ;
        QPointF aux ;

        while (!noPermutations){

            noPermutations = true ;

            for(int i=0 ; i< points.size()-1 ; i++){
                if (points[i].x() > points[i+1].x()){

                    noPermutations = false ;

                    aux.setX(points[i].x());
                    aux.setY(points[i].y());

                    points[i].setX(points[i+1].x());
                    points[i].setY(points[i+1].y());

                    points[i+1].setX(aux.x());
                    points[i+1].setY(aux.y());

                }


            }


        }
        for (int j = 0 ; j < points.size()-1 ; j++){
            // on n'ajoute que les hachures d'indice paire
            if (j%2 == 0){
                usefulHsh->push_back(QLineF(points[j].x(), points[j].y(), points[j+1].x(), points[j+1].y()));
            }

        }

    }//endIf isn't empty
    //qDebug() << *usefulHsh ;




}


void polyProc::hatchPolygonHoriz(QVector<QLineF> *aretes, QVector<QLineF> *hachureEnvelope, QVector<QLineF> *hachuresPoly)
{
    hachuresPoly->clear();
    QVector<QLineF>* singleH = new QVector<QLineF>();
    for (int i = 0 ; i< hachureEnvelope->size(); i++){

        polyProc::polyInterHorizHsh(&((*(hachureEnvelope))[i]), aretes, singleH);

        if(!singleH->isEmpty()){

            for (int k=0 ; k< singleH->size() ; k++){

                hachuresPoly->push_back((*(singleH))[k]);
            }


        }

    }

}




void polyProc::computeCentroid(QPolygonF *basePoly, QPointF *center)
{

    QPointF* volatileCenter = new QPointF();
    float weight = 0 ;
    float sWeight =0;
    float x , y ;
    x=0 ;
    y=0 ;


    int N = basePoly->size() ;
    for (int i=1 ; i < N-1; i++){

        polyProc::centroidTriangle((*basePoly)[0] , (*basePoly)[i] , (*basePoly)[(i+1)], *volatileCenter);
        weight = polyProc::aireAlgebrique((*basePoly)[0] , (*basePoly)[i] , (*basePoly)[(i+1)]);
        sWeight += weight ;

        x += weight* (volatileCenter->x());
        y += weight* (volatileCenter->y());


    }
    x = x/ sWeight ;
    y = y/ sWeight ;

    center->setX(x);
    center->setY(y);

}


void polyProc::computeSurface(QPolygonF *basePoly, float &surface)
{

    float S = 0 ;
    float Stot = 0 ;

    int N = basePoly->size();
    QPointF origin(0,0);

    for (int i=0 ; i< basePoly->size() ; i++ ){

        //S = polyProc::aireAlgebrique( (*basePoly).at(0) , (*basePoly).at(i) , (*basePoly).at((i+1)%N));
        S = polyProc::aireAlgebrique( origin , (*basePoly).at(i) , (*basePoly).at((i+1)%N));
        Stot = Stot + S ;

    }

    surface = 0.5f*Stot ;

    //si les points du polygones sont indexés dans le sens indirect, l'aire sera négative
    if(surface < 0){
        surface = -surface ;
    }


}


void polyProc::centroidTriangle(QPointF a, QPointF b, QPointF c, QPointF &centre)
{

    float x, y ;
    x = a.x() + b.x() + c.x() ;
    y = a.y() + b.y() + c.y() ;
    centre.setX(x/3);
    centre.setY(y/3);

}


bool polyProc::pointInPolygon(QPointF p, QPolygonF poly)
{
    qDebug() << "hello";

    float mx, my, Mx, My ;

    polyProc::minMaxPoly(poly, mx , Mx, my , My) ;

    qDebug() << mx << my << Mx << My;
    QLineF Hline(mx, p.y() , Mx , p.y());
    QLineF arete;
    int left = 0;
    int right =0;

    QPointF* interPt ;
    for (int i=0 ; i < poly.size() ; i++){
        interPt = new QPointF();
        arete.setP1(poly[i]);
        arete.setP2(poly[(i+1)%(poly.size())]);
        polyProc::interHorizLine(&Hline , &arete , interPt);
        qDebug( ) << interPt->isNull() << i << (i+1)%poly.size();

        if (!interPt->isNull()){

            if (interPt->x() < p.x()){
                left ++ ;
            }
            if (interPt->x() > p.x()){

                right++ ;
            }
        }


    }

    if (((left%2)==1)&&((right%2)==1)){
        return 1 ;
    }else{
        return 0 ;
    }


}


bool polyProc::linesAreCrossing(QLineF line1, QLineF line2)
{

    float  d11 , d12 , d21 , d22 ;

    // d1x det 1ere line avec le pt x

    d11 = polyProc::aireAlgebrique(line1.p1() , line1.p2() , line2.p1());
    d12 = polyProc::aireAlgebrique(line1.p1() , line1.p2() , line2.p2());

    if(d11*d12 > 0){
        return false ;
    }else{
        d21 = polyProc::aireAlgebrique(line2.p1(), line2.p2(), line1.p1());
        d22 = polyProc::aireAlgebrique(line2.p1(), line2.p2(), line1.p2());
        if(d21*d22 > 0){
            return false ;
        }else{
            return true ;
        }
    }


}


void polyProc::minMaxPoly(QPolygonF poly, float &minx, float &maxx, float &miny, float &maxy)
{

    float mx, my , Mx, My  ;
    mx = poly[0].x() ;
    my = poly[0].y() ;
    Mx = mx ;
    My = my ;

    for(int i=0; i< poly.size(); i++){
        if(poly[i].x()<=mx){
            mx = poly[i].x();
        }
        if(poly[i].y()<=my){
            my = poly[i].y();
        }
        if(poly[i].x()>=Mx){
            Mx = poly[i].x();
        }
        if(poly[i].y()>=My){
            My = poly[i].y();
        }
    }

    minx = mx ;
    miny = my ;
    maxx = Mx ;
    maxy = My ;
}


void polyProc::loadPolygon(QPolygonF *poly , QString filename)
{
    //catch filename from openFile window
    //QString filename = QFileDialog::getOpenFileName(this,
     //                                       tr("open file"),
       //                                     "./",
         //                                   "text file (*.txt)");
    //QFile myFile(filename);

    QFile* myFile = new QFile(filename);
    if (myFile->open(QIODevice::ReadOnly)){
        //read line by line, then split each line according to separator
        QTextStream textstream(myFile);
        QStringList stringList ;
        poly->clear();

        while (!textstream.atEnd()) {

            QString line = textstream.readLine();

            stringList = line.split(" ");
            //update points
            //program will crash if numbers contained in one line are not 2
            poly->push_back(QPointF(stringList.at(0).toFloat(),
                                stringList.at(1).toFloat()));
        }

        myFile->close();
    }
    else{
        //display error message if texfile cannot be opened (read permession denyied) or
        //empty file or if the user hasn't chosen anyfile somehow.
        //Errors related to data format havn't been treated.
        QMessageBox::information(0,"error",myFile->errorString());
    }
}
