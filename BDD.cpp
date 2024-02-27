#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdexcept> // for std::invalid_argument
#include <iomanip>   // for std::fixed and std::setprecision

class Trajectory {
private:
    std::pair<double, double> firstPoint;
    std::pair<double, double> lastPoint;
    bool hasFirstPoint = false;

public:
    // Add a point to the trajectory
    void addPoint(double x, double y) {
        if (!hasFirstPoint) {
            firstPoint = {x, y};
            hasFirstPoint = true;
        }
        lastPoint = {x, y};
    }

    // Check if the trajectory has at least one point
    bool hasPoints() const {
        return hasFirstPoint;
    }

    // Display the trajectory
    void printFirstAndLastPoint() const {
        if (hasFirstPoint) {
            // Set precision to ensure accurate printing of double values
            std::cout << std::fixed << std::setprecision(6);
            std::cout << "First Point: (" << firstPoint.first << ", " << firstPoint.second << ")" << std::endl;
            std::cout << "Last Point: (" << lastPoint.first << ", " << lastPoint.second << ")" << std::endl;
        } else {
            std::cout << "No points in the trajectory." << std::endl;
        }
    }
};

int main() {
    std::ifstream file("birds.csv");
    if (!file.is_open()) {
        std::cerr << "Error opening file\n";
        return 1;
    }

    // Skip header
    std::string header;
    std::getline(file, header);

    // Skip empty lines
    while (header.empty() && std::getline(file, header)) {}

    std::vector<Trajectory> trajectories;

    std::string line;
    int lineCount = 0;
    while (std::getline(file, line)) {
        lineCount++;
        if (line.empty()) continue; // Skip empty lines

        std::istringstream iss(line);
        std::string field;
        int track_id;
        double x, y;
        try {
            Trajectory currentTrajectory;
            int fieldIndex = 0;
            while (std::getline(iss, field, ';')) {
                if (fieldIndex == 1) {
                    track_id = std::stoi(field);
                } else if (fieldIndex == 6) {
                    x = std::stod(field);
                } else if (fieldIndex == 7) {
                    y = std::stod(field);
                }
                fieldIndex++;
            }
            // Check if all necessary fields are obtained
            if (fieldIndex >= 8) {
                // Check if track_id already exists
                if (static_cast<std::vector<Trajectory>::size_type>(track_id) >= trajectories.size()) {
                    trajectories.resize(track_id + 1);
                }
                // Add point to current trajectory
                trajectories[track_id].addPoint(x, y);
            } else {
                std::cerr << "Error parsing line " << lineCount << ": " << line << std::endl;
            }
        } catch (const std::invalid_argument& e) {
            std::cerr << "Invalid argument in line " << lineCount << ": " << line << std::endl;
        } catch (...) {
            std::cerr << "Unknown error in line " << lineCount << ": " << line << std::endl;
        }
    }

    // Find the first trajectory with points and print its first and last points
    for (const auto& trajectory : trajectories) {
        if (trajectory.hasPoints()) {
            std::cout << "First and Last Point of the First Trajectory:" << std::endl;
            trajectory.printFirstAndLastPoint();
            break;
        }
    }

    file.close();
    return 0;
}

