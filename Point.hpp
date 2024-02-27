
#ifndef POINT_H
#define POINT_H

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>

class Point {
	private :  
	    int x;
	    int y;
	public :    
	    Point();  // constructeur par défaut
	    Point(int x, int y);  // constructeur 
	    ~Point();  // destructeur
	    double distanceEuclidienne(Point& cible); // permet de calculer la distance euclidienne entre deux points
	   
};

#endif

