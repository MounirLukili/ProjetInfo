#include "Frechet.hpp"

using namespace std;

Frechet:: Frechet(Point* t1, Point* t2){
	this->t1= new Point(t1.size());
	this->t2= new Point(t2.size());
	
	for(int i = 0;i<t1.size();i++){
		this->t1[i]=t1[i];
	}
	for(int j = 0;j<t2.size();j++){
		this->t2[j]=t2[j];
	}
}

//FALSE
// Fonction pour calculer la distance de Fréchet entre deux trajectoires
double distanceFrechet() {

 // Calcul de la distance entre chaque paire de points
    for (size_t i = 0; i < trajectoire1.size(); ++i) {
        for (size_t j = 0; j < trajectoire2.size(); ++j) {
            matriceDistances[i][j] = distanceEuclidienne(trajectoire1[i], trajectoire2[j]);
        }
    }
    
    // Calcul de la distance de Fréchet
    std::vector<std::vector<double>> memoization(trajectoire1.size(), std::vector<double>(trajectoire2.size(), 0.0));
    
    // Cas de base pour le premier point
    memoization[0][0] = matriceDistances[0][0];
    
    // Remplissage de la première colonne
    for (size_t i = 1; i < trajectoire1.size(); ++i) {
        memoization[i][0] = std::max(memoization[i - 1][0], matriceDistances[i][0]);
    }
    // Calcul des autres valeurs
    for (size_t i = 1; i < trajectoire1.size(); ++i) {
        for (size_t j = 1; j < trajectoire2.size(); ++j) {
            double minVal = std::min(memoization[i - 1][j], memoization[i][j - 1]);
            minVal = std::min(minVal, memoization[i - 1][j - 1]);
            memoization[i][j] = std::max(minVal, matriceDistances[i][j]);

        }
    }

    // La distance de Fréchet est la valeur dans le coin inférieur droit de la matrice de mémoïzation
    return memoization[trajectoire1.size() - 1][trajectoire2.size() - 1];
}
