#include "Restaurant.h"

#include <iostream>
using namespace std;

Restaurant::Restaurant()
{
}

void Restaurant::PrintSummary() const
{
    cout << "Restaurant Lists Summary" << endl;
    cout << "Actions: " << ACTIONS_LIST.getCount() << endl;
    cout << "Pending ODG: " << PEND_ODG.getCount() << endl;
    cout << "Pending ODN: " << PEND_ODN.getCount() << endl;
    cout << "Pending OT: " << PEND_OT.getCount() << endl;
    cout << "Pending OVN: " << PEND_OVN.getCount() << endl;
    cout << "Pending OVC: " << PEND_OVC.getCount() << endl;
    cout << "Pending OVG: " << PEND_OVG.getCount() << endl;
    cout << "Free CS: " << Free_CS.getCount() << endl;
    cout << "Free CN: " << Free_CN.getCount() << endl;
    cout << "Cancelled orders: " << Cancelled_orders.getCount() << endl;
    cout << "Finished orders: " << Finished_orders.getCount() << endl;
    cout << "Cooking orders: " << Cooking_Orders.getCount() << endl;
    cout << "Ready OT: " << RDY_OT.getCount() << endl;
    cout << "Ready OV: " << RDY_OV.getCount() << endl;
    cout << "Ready OD: " << RDY_OD.getCount() << endl;
    cout << "In-service orders: " << InServ_Orders.getCount() << endl;
    cout << "Free scooters: " << Free_Scooters.getCount() << endl;
    cout << "Back scooters: " << Back_Scooters.getCount() << endl;
    cout << "Maint scooters: " << Maint_Scooters.getCount() << endl;
    cout << "Free tables: " << Free_Tables.getCount() << endl;
    cout << "Busy sharable tables: " << Busy_Sharable.getCount() << endl;
    cout << "Busy no-share tables: " << Busy_No_Share.getCount() << endl;
}
