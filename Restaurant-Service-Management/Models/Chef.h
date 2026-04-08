#pragma once

#include <iostream>
using namespace std;

class Chef
{
    int ID;
    bool Senior;

public:
    Chef(int id = 0, bool isSenior = false) : ID(id), Senior(isSenior) {}

    int GetID() const { return ID; }
    bool IsSenior() const { return Senior; }

    void Print() const
    {
        cout << (Senior ? "CS" : "CN") << ID;
    }
};
