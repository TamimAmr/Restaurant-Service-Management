#pragma once

#include <iostream>

using namespace std;

class Chef;
class Scooter;
class Table;

class Order
{
public:
    enum OrderType
    {
        OT,
        OV,
        OD
    };

private:
    int ID;
    OrderType Type;
    int SeatsNeeded;

    Chef* AssignedChef;
    Scooter* AssignedScooter;
    Table* AssignedTable;

public:
    Order(int id = 0, OrderType type = OT, int seatsNeeded = 0)
        : ID(id)
        , Type(type)
        , SeatsNeeded(seatsNeeded)
        , AssignedChef(nullptr)
        , AssignedScooter(nullptr)
        , AssignedTable(nullptr)
    {
    }

    int GetID() const
    {
        return ID;
    }

    OrderType GetType() const { return Type; }
    int GetSeatsNeeded() const { return SeatsNeeded; }
    void SetSeatsNeeded(int seats) { SeatsNeeded = seats; }

    Chef* GetChef() const { return AssignedChef; }
    void SetChef(Chef* c) { AssignedChef = c; }

    Scooter* GetScooter() const { return AssignedScooter; }
    void SetScooter(Scooter* s) { AssignedScooter = s; }

    Table* GetTable() const { return AssignedTable; }
    void SetTable(Table* t) { AssignedTable = t; }

    void Print() const
    {
        const char* t = (Type == OT) ? "OT" : (Type == OV) ? "OV" : "OD";
        cout << ID << "(" << t << ")";
        if (AssignedChef)
        {
            cout << "[";
            AssignedChef->Print();
            cout << "]";
        }
        if (AssignedScooter)
        {
            cout << "[";
            AssignedScooter->Print();
            cout << "]";
        }
        if (AssignedTable)
        {
            cout << "[";
            AssignedTable->Print();
            cout << "]";
        }
    }
};

inline ostream& operator<<(ostream& out, const Order* o)
{
    if (!o)
        return out << "NULL";
    const char* t = (o->GetType() == Order::OT) ? "OT" : (o->GetType() == Order::OV) ? "OV" : "OD";
    return out << o->GetID() << "(" << t << ")";
}
