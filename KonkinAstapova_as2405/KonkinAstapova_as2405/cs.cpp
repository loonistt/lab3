#include "cs.h"
#include "logger.h"
#include <iostream>
#include <stdexcept>
using namespace std;

int CS::nextId = 1;

CS::CS() : id(nextId++), workshop(0), ActiveWorkshop(0), score(0) {}

void CS::updateNextId(int loadedId) {
    if (loadedId >= nextId) {
        nextId = loadedId + 1;
    }
}

float CS::getUnusedPercentage() const {
    if (workshop == 0) return 0.0f;
    return ((workshop - ActiveWorkshop) * 100.0f) / workshop;
}

void CS::editCS() {
    cout << "Enter new value of active workshops (0-" << workshop << "): ";
    int active = GetCorrectNumber<int>(0, workshop);

    if (active >= 0 && active <= workshop) {
        ActiveWorkshop = active;
    }
    else {
        throw invalid_argument("Active workshops must be between 0 and " + to_string(workshop));
    }
}

ostream& operator<<(ostream& out, const CS& cs) {
    out << "CS ID: " << cs.id << "\n"
        << "Name: " << cs.name << "\n"
        << "Workshops: " << cs.workshop << " (active: " << cs.ActiveWorkshop << ")\n"
        << "Unused workshops: " << cs.getUnusedPercentage() << "%\n"
        << "Score: " << cs.score << "\n\n";
    return out;
}

istream& operator>>(istream& in, CS& cs) {
    cout << "Enter name of the compressor station: ";
    INPUT_LINE(in, cs.name);

    cout << "Enter the number of workshops (1-50): ";
    int workshops = GetCorrectNumber<int>(1, 50);
    cs.workshop = workshops;

    cout << "Enter the number of active workshops (0-" << workshops << "): ";
    int active = GetCorrectNumber<int>(0, workshops);
    cs.ActiveWorkshop = active;

    cout << "Enter the compressor station score (0-10): ";
    float score_val = GetCorrectNumber<float>(0, 10);
    cs.score = score_val;

    return in;
}

void loadCSFromFile(istream& in, CS& cs) {
    in >> cs.id;
    getline(in >> ws, cs.name);
    in >> cs.workshop >> cs.ActiveWorkshop >> cs.score;
    CS::updateNextId(cs.id);
}