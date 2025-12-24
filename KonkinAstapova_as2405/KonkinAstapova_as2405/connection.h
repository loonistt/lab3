#pragma once
#include <iostream>
#include <vector>
using namespace std;

class Pipe;

class Connection {
private:
    static int nextId;
    int id;
    int startCS;
    int endCS;
    int pipeId;

public:
    Connection(int start, int end, int pipe);

    int getId() const { return id; }
    int getStartCS() const { return startCS; }
    int getEndCS() const { return endCS; }
    int getPipeId() const { return pipeId; }

    friend ostream& operator<<(ostream& out, const Connection& conn);
};
