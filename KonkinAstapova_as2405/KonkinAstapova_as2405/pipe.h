#pragma once
#include <iostream>
#include <string>
using namespace std;

class Pipe {
private:
    static int nextId;
    int id;
    string name;
    float length;
    float diameter;
    bool fixing;

public:
    Pipe();
    
    int getId() const { return id; }
    string getName() const { return name; }
    float getLength() const { return length; }
    float getDiameter() const { return diameter; }
    bool getFixing() const { return fixing; }
    void setFixing(bool new_fixing) { fixing = new_fixing; }
    // Методы
    void editPipe();

    // Дружественные функции
    friend ostream& operator<<(ostream& out, const Pipe& pipe);
    friend istream& operator>>(istream& in, Pipe& pipe);
    friend void loadPipeFromFile(istream& in, Pipe& pipe);

    // Статический метод для обновления nextId
    static void updateNextId(int loadedId);
};