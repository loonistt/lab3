#pragma once
#include <iostream>
#include <vector>
using namespace std;

class Connection {
private:
    static int nextId;
    int id;
    int startCS;
    int endCS;
    int pipeId;
    int diameter;

public:
    Connection(int start, int end, int pipe, int diam);

    int getId() const { return id; }
    int getStartCS() const { return startCS; }
    int getEndCS() const { return endCS; }
    int getPipeId() const { return pipeId; }
    int getDiameter() const { return diameter; }

    friend ostream& operator<<(ostream& out, const Connection& conn);
};

inline const vector<int> ALLOWED_DIAMETERS = { 500, 700, 1000, 1400 };