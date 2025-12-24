#include "connection.h"
#include "pipe.h"
#include <iostream>
using namespace std;
int Connection::nextId = 1;

Connection::Connection(int start, int end, int pipe)
    : id(nextId++), startCS(start), endCS(end), pipeId(pipe) {
}


ostream& operator<<(ostream& out, const Connection& conn) {
    out << "Connection ID: " << conn.id << "\n"
        << "From CS: " << conn.startCS << " -> To CS: " << conn.endCS << "\n"
        << "Pipe ID: " << conn.pipeId << "\n\n";
    return out;
}
