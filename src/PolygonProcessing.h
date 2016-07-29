#ifndef POLYGONPROCESSING_H
#define POLYGONPROCESSING_H
#include <additional_includes.h>
//Date: 12/02/2012
//Auteur: Khalil Ben Fadhel

namespace polyProc {
	
	
	//-----------------------------------------
	//Nom: loadPolygon
	//Description: Charger un polygone à partir d'un fichier texte de nom filename
	//Entrée(s): chaine de charactères filename
	//Sortie(s): polygone , passage par variable
	//-------------------------------------------
    void loadPolygon(QPolygonF* poly , QString filename) ;
	
	
	
	//-----------------------------------------
	//Nom: computeSurface
	//Description: calculer l'aire d'un polygone
	//Entrée(s): polygone basePolypoly
	//Sortie(s): surface de type float , passage par variable
	//-------------------------------------------
    void computeSurface(QPolygonF* basePoly, float& surface );

	
	//-----------------------------------------
	//Nom: computeCentroid
	//Description: Calculer le centroide d'un polygone
	//Entrée(s): polygone basePoly
	//Sortie(s): point 2D center , passage par variable
	//-------------------------------------------
	void computeCentroid(QPolygonF* basePoly , QPointF* center);
	
	
	//-----------------------------------------
	//Nom: pointInPolygon
	//Description: affirmes si un point se trouve à l'interieur ou à l'exterieur d'un polygone
	//Entrée(s): point et un polygone
	//Sortie(s): booléenne, vraie si le point est intérieur au polygone
	//-------------------------------------------
    bool pointInPolygon(QPointF p, QPolygonF poly);
	
	//-----------------------------------------
	//Nom: hatchPolygonHoriz
	//Description: Hachurer un polygone horizontalement
	//   			Pour le hachurer avec un angle non nul, nous avons utilisé QTransform pour effectuer
	//				Les rotations décrites dans le rapport. Les détails de l'operation sont dans le fichier dialog.cpp
	//Entrée(s): aretes du polygone, hachures initiales (de l'enveloppe)
	//Sortie(s): hachures utiles	, passage par variable
	//-------------------------------------------
    void hatchPolygonHoriz(QVector<QLineF>* aretes , QVector<QLineF>* hachureEnvelope, QVector<QLineF>* hachuresPoly);
	
	
	
	//-----------------------------------------
	//Nom: aireAlgebrique
	//Description: Calculer l'aire algébrique d'un triangle orienté abc
	//Entrée(s): 3 points a b et c , l'ordre est important
	//Sortie(s): aire algébrique de type float
	//-------------------------------------------
    float aireAlgebrique(QPointF a, QPointF b, QPointF c);
    
	
	//-----------------------------------------
	//Nom: enveloppe
	//Description: Calculer la fenêtre englobante d'un polygone
	//Entrée(s): polygone poly
	//Sortie(s): enveloppe env , passage par variable
	//-------------------------------------------
	void enveloppe(QPolygonF *poly, QPolygonF *env);
    
	
	//-----------------------------------------
	//Nom: hatchEnvelop
	//Description: Hachurer la fenetre englobante horizontalement, ceci sert comme initialisation de L'etape de hachurage
	//Entrée(s): enveloppe env
	//Sortie(s): un vecteur de lignes hatches(hachures initiales), le vecteur est supposé vide au départ
	//-------------------------------------------
	void hatchEnvelop(QPolygonF *env, float d, QVector<QLineF> *hatches);

	
    //-----------------------------------------
	//Nom: poly2lines
	//Description: Extraire les arêtes à partir d'un polygone
	//Entrée(s): polygone poly
	//Sortie(s): vecteur de lignes aretes, passage par variable
	//-------------------------------------------
	void poly2lines(QPolygonF *poly, QVector<QLineF>* aretes);
    
	
	//-----------------------------------------
	//Nom: interHorizLine
	//Description: Chercher le point d'intersection entre une ligne horizontale HorizLine et une ligne quelconque.
	//Entrée(s): 2 lignes, le premier argument est une ligne horizontale
	//Sortie(s): point d'intersection, peut etre vide.
	//-------------------------------------------
	void interHorizLine(QLineF *HorizLine, QLineF *Line, QPointF *pInter);
    
	
	
	//-----------------------------------------
	//Nom: polyInterHorizHsh
	//Description: Chercher les intersections d'une ligne horizontale avec tous les aretes d'un polygone
	//Entrée(s): une ligne horizontale, ensemble des aretes
	//Sortie(s): hachures utiles
	//-------------------------------------------
	void polyInterHorizHsh(QLineF* HorizLine, QVector<QLineF>* lines , QVector<QLineF>* usefulHsh);

	
	


	//-----------------------------------------
	//Nom: centroidTriangle
	//Description: Charger un polygone à partir d'un fichier texte de nom filename
	//Entrée(s): chaine de charactères filename
	//Sortie(s): polygone , passage par variable
	//-------------------------------------------
    void centroidTriangle(QPointF a, QPointF b, QPointF c , QPointF& centre);
    
	
	


    //-----------------------------------------
	//Nom: linesAreCrossing
	//Description: retourne vrai si les deux lignes se coupent et faux sinon
	//Entrée(s): deux lignes
	//Sortie(s): boolééenne vrai ou faux
	//-------------------------------------------
	bool linesAreCrossing(QLineF line1 , QLineF line2);

	
	



	

	//-----------------------------------------
	//Nom: minMaxPoly
	//Description: Chercher les extremums d'un polygone
	//Entrée(s): polygne poly
	//Sortie(s): maxx maxy miny minx , passages par variable
	//-------------------------------------------
    void minMaxPoly(QPolygonF poly, float& minx, float& maxx, float& miny, float& maxy);



}

#endif // POLYGONPROCESSING_H
