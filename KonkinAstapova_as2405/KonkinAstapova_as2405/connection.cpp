#include "connection.h"
#include <iostream>
using namespace std;
static int nextConnectionId = 1;

Connection::Connection(int start, int end, int pipe, int diam)
    : id(nextConnectionId++), startCS(start), endCS(end), pipeId(pipe), diameter(diam) {
}

ostream& operator<<(ostream& out, const Connection& conn) {
    out << "Connection ID: " << conn.id << "\n"
        << "From CS: " << conn.startCS << " -> To CS: " << conn.endCS << "\n"
        << "Pipe ID: " << conn.pipeId << " (Diameter: " << conn.diameter << " mm)\n\n";
    return out;
}