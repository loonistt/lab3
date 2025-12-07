#pragma once
#include <iostream>
#include <string>
using namespace std;

class CS {
private:
    static int nextId;
    int id;
    string name;
    int workshop;
    int ActiveWorkshop;
    float score;

public:
    CS();

    // Геттеры
    int getId() const { return id; }
    string getName() const { return name; }
    int getWorkshop() const { return workshop; }
    int getActiveWorkshop() const { return ActiveWorkshop; }
    float getScore() const { return score; }

    // Методы
    float getUnusedPercentage() const;
    void editCS();

    // Дружественные функции
    friend ostream& operator<<(ostream& out, const CS& cs);
    friend istream& operator>>(istream& in, CS& cs);
    friend void loadCSFromFile(istream& in, CS& cs);

    // Статический метод
    static void updateNextId(int loadedId);
};