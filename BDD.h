#ifndef BDD_H
#define BDD_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm> // for std::sort
#include <stdexcept> // for std::runtime_error
#include <iomanip>   // for std::fixed and std::setprecision

// Define a struct to represent a point
struct Point {
    double x;
    double y;
    double t;
};

// Define a class to represent a trajectory
class Trajectory {
private:
    std::vector<Point> points;

public:
    // Add a point to the trajectory
    void addPoint(double x, double y, double t) {
        points.push_back({x, y, t});
    }

    // Sort points by increasing t
    void sortPointsByT() {
        std::sort(points.begin(), points.end(), [](const Point& a, const Point& b) {
            return a.t < b.t;
        });
    }

    // Check if the trajectory has at least one point
    bool hasPoints() const {
        return !points.empty();
    }

    // Display the trajectory
    void printPoints() const {
        for (const auto& point : points) {
            std::cout << std::fixed << std::setprecision(6);
            std::cout << "Point: (" << point.x << ", " << point.y << ", " << point.t << ")" << std::endl;
        }
    }
};

// Function to create trajectories from a CSV file
std::map<int, Trajectory> createTrajectories(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Error opening file");
    }

    // Skip header
    std::string header;
    std::getline(file, header);

    // Skip empty lines
    while (header.empty() && std::getline(file, header)) {}

    std::map<int, Trajectory> trajectories;

    std::string line;
    int lineCount = 0;
    while (std::getline(file, line)) {
        lineCount++;
        if (line.empty()) continue; // Skip empty lines

        std::istringstream iss(line);
        std::string field;
        int track_id;
        double x, y, t;
        try {
            int fieldIndex = 0;
            while (std::getline(iss, field, ';')) {
                if (fieldIndex == 1) {
                    track_id = std::stoi(field);
                } else if (fieldIndex == 6) {
                    x = std::stod(field);
                } else if (fieldIndex == 7) {
                    y = std::stod(field);
                } else if (fieldIndex == 8) {
                    t = std::stod(field);
                }
                fieldIndex++;
            }
            // Check if all necessary fields are obtained
            if (fieldIndex >= 9) {
                // Add point to current trajectory
                trajectories[track_id].addPoint(x, y, t);
            } else {
                throw std::runtime_error("Error parsing line " + std::to_string(lineCount) + ": " + line);
            }
        } catch (const std::invalid_argument& e) {
            throw std::runtime_error("Invalid argument in line " + std::to_string(lineCount) + ": " + line);
        } catch (...) {
            throw std::runtime_error("Unknown error in line " + std::to_string(lineCount) + ": " + line);
        }
    }

    // Sort points for each trajectory by increasing t
    for (auto& pair : trajectories) {
        pair.second.sortPointsByT();
    }

    file.close();
    return trajectories;
}

#endif // BDD_H

