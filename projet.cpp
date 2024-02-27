#include <iostream>
#include <vector>
#include <cmath>

// Structure pour représenter un point dans une trajectoire
struct Point {
    double x;
    double y;
};

// Fonction pour calculer la distance euclidienne entre deux points
double distanceEuclidienne(const Point& p1, const Point& p2) {
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

// Fonction pour calculer la distance de Fréchet entre deux trajectoires
double distanceFrechet(const std::vector<Point>& trajectoire1, const std::vector<Point>& trajectoire2) {

 std::vector<std::vector<double>> matriceDistances(trajectoire1.size(), std::vector<double>(trajectoire2.size(), 0.0));
 
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

int main() {
    // Exemple d'utilisation
    std::vector<Point> trajectoire1 = {{0.2, 2.0}, {1.5, 2.8}, {2.3, 1.6}, {2.9, 1.8}, {4.1, 3.1}, {5.6, 2.9}, {7.2, 1.3},{8.2,1.1}};
    std::vector<Point> trajectoire2 = {{0.3, 1.6}, {3.2, 3}, {3.8, 1.8}, {5.2, 3.1}, {6.5, 2.8}, {7, 0.8},{8.9,0.6}};

    double dist = distanceFrechet(trajectoire1, trajectoire2);
    std::cout << "Distance de Frechet : " << dist << std::endl;

    return 0;
}


