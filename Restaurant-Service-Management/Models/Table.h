#pragma once

#include <iostream>

using namespace std;

class Table
{
private:
    int ID;
    int seats;

public:
    Table(int id = 0, int seatCount = 0) : ID(id), seats(seatCount) {}

    int GetID() const
    {
        return ID;
    }

    int GetSeats() const
    {
        return seats;
    }

    void SetSeats(int seatCount)
    {
        seats = seatCount;
    }

    void Print() const
    {
        cout << "T" << ID << "[" << seats << "]";
    }
};
