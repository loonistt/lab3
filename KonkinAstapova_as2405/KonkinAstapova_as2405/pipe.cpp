#include "pipe.h"
#include "logger.h"
#include <iostream>
#include <stdexcept>
using namespace std;

int Pipe::nextId = 1;

Pipe::Pipe() : id(nextId++), length(0), diameter(0), fixing(false) {}

void Pipe::editPipe() {
    cout << "Enter new pipe status (0 - in repair, 1 - working): ";
    bool newStatus = GetCorrectBool();
    fixing = newStatus;
}

void Pipe::updateNextId(int loadedId) {
    if (loadedId >= nextId) {
        nextId = loadedId + 1;
    }
}

ostream& operator<<(ostream& out, const Pipe& pipe) {
    out << "Pipe ID: " << pipe.id << "\n"
        << "Name: " << pipe.name << "\n"
        << "Length: " << pipe.length << "m\n"
        << "Diameter: " << pipe.diameter << "mm\n"
        << "Status: " << (pipe.fixing ? "Working" : "In repair") << "\n\n";
    return out;
}

istream& operator>>(istream& in, Pipe& pipe) {
    cout << "Enter name of the pipe: ";
    INPUT_LINE(in, pipe.name);

    cout << "Enter pipe length (in meters, 0-50): ";
    float len = GetCorrectNumber<float>(0, 50);
    pipe.length = len;

    cout << "Enter pipe diameter (in millimeters, 0-2500): ";
    float diam = GetCorrectNumber<float>(0, 2500);
    pipe.diameter = diam;

    cout << "Choose pipe status (0 - in repair, 1 - working): ";
    pipe.fixing = GetCorrectBool();

    return in;
}

void loadPipeFromFile(istream& in, Pipe& pipe) {
    in >> pipe.id;
    getline(in >> ws, pipe.name);
    in >> pipe.length >> pipe.diameter >> pipe.fixing;
    Pipe::updateNextId(pipe.id);
}