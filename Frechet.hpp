
#ifndef FRECHET_H
#define FRECHET_H

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>

class Frechet {
	private :  
	    Point* t1; // données de l'utilisateur
	    Point* t2; // données machine
	    int res;
	public :    
	    Frechet();  // constructeur par défaut
	    Frechet(Point* t1, Point* t2);  // constructeur 
	    ~Frechet();  // destructeur
	    int distanceFrechet();  // calcule + instanciation attribut tabledeFrechet
	    void afficherResultat(); //afficher resultat 
};

#endif

