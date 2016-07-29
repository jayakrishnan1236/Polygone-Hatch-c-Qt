#ifndef POLYDATA_H
#define POLYDATA_H
#include <additional_includes.h>
#include <PolygonProcessing.h>

class polyData
{
public:
    polyData();

    QPolygonF *polygonF ;
    QPolygonF *rotatedPoly;
    QPolygonF *envelopF ;
    QPointF *centroid ;

    QVector<QLineF>* envHsh;
    QVector<QLineF>* clippedHsh ;
    QVector<QLineF>* finalHsh ;

    QVector<QLineF>* aretesPoly;
    QVector<QLineF>* aretesRectifs;

    float surface ;
    bool inOut ;


};

#endif // POLYDATA_H
