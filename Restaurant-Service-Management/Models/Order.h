#pragma once

#include <iostream>

using namespace std;

class Order
{
private:
    int ID;

public:
    Order(int id = 0) : ID(id) {}

    int GetID() const
    {
        return ID;
    }

    void Print() const
    {
        cout << ID;
    }
};
