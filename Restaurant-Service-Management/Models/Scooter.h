#pragma once

#include <iostream>
using namespace std;

class Scooter
{
    int ID;

public:
    Scooter(int id = 0) : ID(id) {}

    int GetID() const { return ID; }

    void Print() const
    {
        cout << "S" << ID;
    }
};

inline ostream& operator<<(ostream& out, const Scooter* s)
{
    if (!s)
        return out << "NULL";
    return out << "S" << s->GetID();
}
