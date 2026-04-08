#include <iostream>
#include <cstdlib>
#include <ctime>

#include "../DataStructures/ArrayStack.h"
#include "../DataStructures/Fit_Tables.h"
#include "../DataStructures/LinkedQueue.h"
#include "../DataStructures/Pend_OVC.h"
#include "../DataStructures/RDY_OV.h"
#include "../DataStructures/priQueue.h"
#include "../Models/Chef.h"
#include "../Models/Order.h"
#include "../Models/Scooter.h"
#include "../Models/Table.h"

using namespace std;

static int RandInt(int lo, int hi)
{
    if (hi < lo)
        return lo;
    return lo + (rand() % (hi - lo + 1));
}

static bool Chance(int percent)
{
    return RandInt(1, 100) <= percent;
}

static void PrintAllLists(
    int timestep,
    int totalGenerated,
    const LinkedQueue<Order*>& PEND_ODG,
    const LinkedQueue<Order*>& PEND_ODN,
    const LinkedQueue<Order*>& PEND_OT,
    const LinkedQueue<Order*>& PEND_OVN,
    const Pend_OVC& PEND_OVC,
    const priQueue<Order*>& PEND_OVG,
    const LinkedQueue<Chef*>& Free_CS,
    const LinkedQueue<Chef*>& Free_CN,
    const Pend_OVC& Cooking_OVC,
    const LinkedQueue<Order*>& Cooking_OT,
    const LinkedQueue<Order*>& Cooking_OD,
    const LinkedQueue<Order*>& RDY_OT,
    const RDY_OV& RDY_OVC,
    const LinkedQueue<Order*>& RDY_OD,
    const priQueue<Order*>& InServ_Orders,
    const priQueue<Scooter*>& Free_Scooters,
    const priQueue<Scooter*>& Back_Scooters,
    const LinkedQueue<Scooter*>& Maint_Scooters,
    const Fit_Tables& Free_Tables,
    const LinkedQueue<Order*>& Cancelled_orders,
    const ArrayStack<Order*>& Finished_orders)
{
    cout << "\n==================== Time Step " << timestep << " ====================\n";
    cout << "Generated: " << totalGenerated
         << " | Finished: " << Finished_orders.getCount()
         << " | Cancelled: " << Cancelled_orders.getCount() << "\n\n";

    cout << "---- Pending Orders ----\n";
    cout << "PEND_ODG: "; PEND_ODG.Print(); cout << "\n";
    cout << "PEND_ODN: "; PEND_ODN.Print(); cout << "\n";
    cout << "PEND_OT:  "; PEND_OT.Print(); cout << "\n";
    cout << "PEND_OVN: "; PEND_OVN.Print(); cout << "\n";
    PEND_OVC.Print();
    cout << "PEND_OVG: "; PEND_OVG.Print(); cout << "\n";

    cout << "\n---- Free Chefs ----\n";
    cout << "Free_CS: "; Free_CS.Print(); cout << "\n";
    cout << "Free_CN: "; Free_CN.Print(); cout << "\n";

    cout << "\n---- Cooking Orders ----\n";
    cout << "COOK_OT:  "; Cooking_OT.Print(); cout << "\n";
    cout << "COOK_OD:  "; Cooking_OD.Print(); cout << "\n";
    Cooking_OVC.Print();

    cout << "\n---- Ready Orders ----\n";
    cout << "RDY_OT: "; RDY_OT.Print(); cout << "\n";
    RDY_OVC.Print();
    cout << "RDY_OD: "; RDY_OD.Print(); cout << "\n";

    cout << "\n---- In-Service Orders ----\n";
    cout << "INSERV: "; InServ_Orders.Print(); cout << "\n";

    cout << "\n---- Scooters ----\n";
    cout << "FREE_S: "; Free_Scooters.Print(); cout << "\n";
    cout << "BACK_S: "; Back_Scooters.Print(); cout << "\n";
    cout << "MAINT_S:"; Maint_Scooters.Print(); cout << "\n";

    cout << "\n---- Tables ----\n";
    Free_Tables.Print();

    cout << "\n---- History ----\n";
    cout << "CANCEL: "; Cancelled_orders.Print(); cout << "\n";
    cout << "FINISH: "; Finished_orders.Print(); cout << "\n";
}

static void RandomSimulator()
{
    srand((unsigned)time(nullptr));

    const int TOTAL_ORDERS = 500;

    // Pending orders
    LinkedQueue<Order*> PEND_ODG;
    LinkedQueue<Order*> PEND_ODN;
    LinkedQueue<Order*> PEND_OT;
    LinkedQueue<Order*> PEND_OVN;
    Pend_OVC PEND_OVC;
    priQueue<Order*> PEND_OVG;

    // Free chefs
    LinkedQueue<Chef*> Free_CS;
    LinkedQueue<Chef*> Free_CN;

    // Cooking
    LinkedQueue<Order*> Cooking_OT;
    LinkedQueue<Order*> Cooking_OD;
    Pend_OVC Cooking_OVC; // reuse CancelOrder logic

    // Ready
    LinkedQueue<Order*> RDY_OT;
    RDY_OV RDY_OVC;       // reuse CancelOrder logic
    LinkedQueue<Order*> RDY_OD;

    // In-service
    priQueue<Order*> InServ_Orders;

    // Scooters
    priQueue<Scooter*> Free_Scooters;
    priQueue<Scooter*> Back_Scooters;
    LinkedQueue<Scooter*> Maint_Scooters;

    // Tables
    Fit_Tables Free_Tables;

    // History
    LinkedQueue<Order*> Cancelled_orders;
    ArrayStack<Order*> Finished_orders;

    // Init chefs
    const int NUM_SENIOR = 15;
    const int NUM_NORMAL = 35;
    int chefId = 1;
    for (int i = 0; i < NUM_SENIOR; i++)
        Free_CS.enqueue(new Chef(chefId++, true));
    for (int i = 0; i < NUM_NORMAL; i++)
        Free_CN.enqueue(new Chef(chefId++, false));

    // Init scooters
    const int NUM_SCOOTERS = 25;
    for (int i = 1; i <= NUM_SCOOTERS; i++)
    {
        Scooter* s = new Scooter(i);
        Free_Scooters.enqueue(s, RandInt(1, 100));
    }

    // Init tables
    const int NUM_TABLES = 40;
    for (int i = 1; i <= NUM_TABLES; i++)
    {
        int seats = (RandInt(0, 2) == 0) ? 2 : (RandInt(0, 1) == 0 ? 4 : 6);
        Table* t = new Table(i, seats);
        Free_Tables.enqueue(t, -t->GetSeats());
    }

    // Generate orders and distribute on pending lists
    for (int id = 1; id <= TOTAL_ORDERS; id++)
    {
        int typePick = RandInt(1, 100);
        Order::OrderType type = (typePick <= 20) ? Order::OT : (typePick <= 55) ? Order::OV : Order::OD;

        int seatsNeeded = 0;
        if (type == Order::OD)
            seatsNeeded = RandInt(1, 6);

        Order* o = new Order(1000 + id, type, seatsNeeded);

        if (type == Order::OT)
        {
            PEND_OT.enqueue(o);
        }
        else if (type == Order::OD)
        {
            if (Chance(50))
                PEND_ODG.enqueue(o);
            else
                PEND_ODN.enqueue(o);
        }
        else // OV
        {
            int sub = RandInt(1, 3); // N / C / G
            if (sub == 1)
                PEND_OVN.enqueue(o);
            else if (sub == 2)
                PEND_OVC.enqueue(o);
            else
                PEND_OVG.enqueue(o, RandInt(1, 100));
        }
    }

    int timestep = 0;
    while (Finished_orders.getCount() + Cancelled_orders.getCount() < TOTAL_ORDERS)
    {
        timestep++;

        // 3.1 Repeat 30 times: move from pending -> cooking with random chef
        for (int k = 0; k < 30; k++)
        {
            // Pick a non-empty pending list
            int tries = 0;
            bool moved = false;
            while (tries++ < 10 && !moved)
            {
                int choice = RandInt(1, 6);
                Order* o = nullptr;

                if (choice == 1 && !PEND_ODG.isEmpty())
                    PEND_ODG.dequeue(o);
                else if (choice == 2 && !PEND_ODN.isEmpty())
                    PEND_ODN.dequeue(o);
                else if (choice == 3 && !PEND_OT.isEmpty())
                    PEND_OT.dequeue(o);
                else if (choice == 4 && !PEND_OVN.isEmpty())
                    PEND_OVN.dequeue(o);
                else if (choice == 5 && !PEND_OVC.isEmpty())
                    PEND_OVC.dequeue(o);
                else if (choice == 6 && !PEND_OVG.isEmpty())
                {
                    int pri = 0;
                    PEND_OVG.dequeue(o, pri);
                }
                else
                    continue;

                // pick a random chef (top of a random free list)
                Chef* c = nullptr;
                bool gotChef = false;
                if (Chance(50))
                {
                    if (!Free_CS.isEmpty())
                        gotChef = Free_CS.dequeue(c);
                    else if (!Free_CN.isEmpty())
                        gotChef = Free_CN.dequeue(c);
                }
                else
                {
                    if (!Free_CN.isEmpty())
                        gotChef = Free_CN.dequeue(c);
                    else if (!Free_CS.isEmpty())
                        gotChef = Free_CS.dequeue(c);
                }

                if (!gotChef)
                {
                    // no chef available, return order back to a reasonable pending list
                    if (o->GetType() == Order::OT)
                        PEND_OT.enqueue(o);
                    else if (o->GetType() == Order::OD)
                        PEND_ODN.enqueue(o);
                    else
                        PEND_OVN.enqueue(o);
                    continue;
                }

                o->SetChef(c);

                if (o->GetType() == Order::OT)
                    Cooking_OT.enqueue(o);
                else if (o->GetType() == Order::OD)
                    Cooking_OD.enqueue(o);
                else
                    Cooking_OVC.enqueue(o);

                moved = true;
            }
        }

        // 3.2 Repeat 15 times:
        // With probability 75%, pick a random order from cooking lists and move it to ready lists (release chef).
        for (int k = 0; k < 15; k++)
        {
            if (!Chance(75))
                continue;

            int tries = 0;
            bool moved = false;
            while (tries++ < 10 && !moved)
            {
                int choice = RandInt(1, 3);
                Order* o = nullptr;
                if (choice == 1 && !Cooking_OT.isEmpty())
                    Cooking_OT.dequeue(o);
                else if (choice == 2 && !Cooking_OD.isEmpty())
                    Cooking_OD.dequeue(o);
                else if (choice == 3 && !Cooking_OVC.isEmpty())
                    Cooking_OVC.dequeue(o);
                else
                    continue;

                // release chef
                Chef* c = o->GetChef();
                if (c)
                {
                    if (c->IsSenior())
                        Free_CS.enqueue(c);
                    else
                        Free_CN.enqueue(c);
                    o->SetChef(nullptr);
                }

                // move to ready
                if (o->GetType() == Order::OT)
                    RDY_OT.enqueue(o);
                else if (o->GetType() == Order::OD)
                    RDY_OD.enqueue(o);
                else
                    RDY_OVC.enqueue(o);

                moved = true;
            }
        }

        // 3.3 Repeat 10 times: move ready -> finish or in-service (assign scooter/table)
        for (int k = 0; k < 10; k++)
        {
            int tries = 0;
            while (tries++ < 10)
            {
                int choice = RandInt(1, 3);
                Order* o = nullptr;

                if (choice == 1 && !RDY_OT.isEmpty())
                    RDY_OT.dequeue(o);
                else if (choice == 2 && !RDY_OVC.isEmpty())
                    RDY_OVC.dequeue(o);
                else if (choice == 3 && !RDY_OD.isEmpty())
                    RDY_OD.dequeue(o);
                else
                    continue;

                if (o->GetType() == Order::OT)
                {
                    Finished_orders.push(o);
                    break;
                }
                else if (o->GetType() == Order::OV)
                {
                    Scooter* s = nullptr;
                    int pri = 0;
                    if (Free_Scooters.dequeue(s, pri))
                    {
                        o->SetScooter(s);
                        InServ_Orders.enqueue(o, RandInt(1, 100));
                    }
                    else
                    {
                        // no scooter, return to ready
                        RDY_OVC.enqueue(o);
                    }
                    break;
                }
                else // OD
                {
                    Table* t = nullptr;
                    int needed = o->GetSeatsNeeded();
                    if (needed <= 0)
                        needed = RandInt(1, 6);

                    if (Free_Tables.getBest(needed, t))
                    {
                        o->SetTable(t);
                        InServ_Orders.enqueue(o, RandInt(1, 100));
                    }
                    else
                    {
                        // no table, return to ready
                        RDY_OD.enqueue(o);
                    }
                    break;
                }
            }
        }

        // 3.4 Cancel from pending OVC -> cancelled
        {
            int randomID = RandInt(1001, 1000 + TOTAL_ORDERS);
            Order* removed = nullptr;
            if (PEND_OVC.CancelOrder(randomID, removed) && removed)
                Cancelled_orders.enqueue(removed);
        }

        // 3.5 Cancel from ready OVC -> cancelled
        {
            int randomID = RandInt(1001, 1000 + TOTAL_ORDERS);
            Order* removed = nullptr;
            if (RDY_OVC.CancelOrder(randomID, removed) && removed)
                Cancelled_orders.enqueue(removed);
        }

        // 3.6 Cancel from cooking OV -> cancelled and release chef
        {
            int randomID = RandInt(1001, 1000 + TOTAL_ORDERS);
            Order* removed = nullptr;
            if (Cooking_OVC.CancelOrder(randomID, removed) && removed)
            {
                Chef* c = removed->GetChef();
                if (c)
                {
                    if (c->IsSenior())
                        Free_CS.enqueue(c);
                    else
                        Free_CN.enqueue(c);
                    removed->SetChef(nullptr);
                }
                Cancelled_orders.enqueue(removed);
            }
        }

        // 3.7 With probability 25%: pick the top order from in-service -> finish and return scooter/table
        if (Chance(25))
        {
            Order* o = nullptr;
            int pri = 0;
            if (InServ_Orders.dequeue(o, pri) && o)
            {
                if (o->GetType() == Order::OV)
                {
                    Scooter* s = o->GetScooter();
                    if (s)
                    {
                        o->SetScooter(nullptr);
                        Back_Scooters.enqueue(s, RandInt(1, 100));
                    }
                }
                else if (o->GetType() == Order::OD)
                {
                    Table* t = o->GetTable();
                    if (t)
                    {
                        o->SetTable(nullptr);
                        Free_Tables.enqueue(t, -t->GetSeats());
                    }
                }

                Finished_orders.push(o);
            }
        }

        // 3.8 With probability 50%: scooter from back -> free or maintenance
        if (Chance(50))
        {
            Scooter* s = nullptr;
            int pri = 0;
            if (Back_Scooters.dequeue(s, pri) && s)
            {
                if (Chance(50))
                    Free_Scooters.enqueue(s, RandInt(1, 100));
                else
                    Maint_Scooters.enqueue(s);
            }
        }

        // 3.9 With probability 50%: scooter from maintenance -> free
        if (Chance(50))
        {
            Scooter* s = nullptr;
            if (Maint_Scooters.dequeue(s) && s)
                Free_Scooters.enqueue(s, RandInt(1, 100));
        }

        // 3.10 Interface (print all lists each timestep)
        PrintAllLists(
            timestep,
            TOTAL_ORDERS,
            PEND_ODG,
            PEND_ODN,
            PEND_OT,
            PEND_OVN,
            PEND_OVC,
            PEND_OVG,
            Free_CS,
            Free_CN,
            Cooking_OVC,
            Cooking_OT,
            Cooking_OD,
            RDY_OT,
            RDY_OVC,
            RDY_OD,
            InServ_Orders,
            Free_Scooters,
            Back_Scooters,
            Maint_Scooters,
            Free_Tables,
            Cancelled_orders,
            Finished_orders);
    }

    cout << "\nSimulation ended after " << timestep << " timesteps.\n";
}

int main()
{
    RandomSimulator();
    return 0;
}
