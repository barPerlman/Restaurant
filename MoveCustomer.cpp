#include "Action.h"
#include "Restaurant.h"
#include <iostream>
#include "Dish.h"
#include "Customer.h"
#include <string>
#include <fstream>
#include <vector>

using namespace std;


MoveCustomer::MoveCustomer(int src, int dst, int customerId)
        : BaseAction(), srcTable(src), dstTable(dst), id(customerId) {}

void MoveCustomer::act(Restaurant &restaurant) {

//those assigned for readability of the code
    Table *src_table = restaurant.getTable(srcTable);
    Table *dst_table = restaurant.getTable(dstTable);

//save the pointer to the customer that will be moved
    Customer *customer = restaurant.getTable(srcTable)->getCustomer(id);
    //all the terms are ok and it's possible to move the customer
    if (src_table->isOpen() && dst_table->isOpen() &&
        srcTable < restaurant.getNumOfTables() && srcTable >= 0 && dstTable < restaurant.getNumOfTables() &&
        dstTable >= 0 &&
        customer != nullptr && (dst_table->getCapacity() - dst_table->getCustomers().size()) > 0) {
        //access to the src table order list
        vector<OrderPair> &srcOrderList = restaurant.getTable(srcTable)->getOrders();
        //access to the dst table order list
        vector<OrderPair> &dstOrderList = restaurant.getTable(dstTable)->getOrders();


        //add the customer to the new table
        restaurant.getTable(dstTable)->addCustomer(customer);
        //add the orders belongs to the moved customer to the destination table
        for (OrderPair tableOrder:srcOrderList) {
            if (customer->getId() == tableOrder.first) {//this dish should be removed to the other table
                dstOrderList.push_back(tableOrder); //push this dish to the new table
            }
        }
        //remove customer and its orders from source table
        restaurant.getTable(srcTable)->removeCustomer(id);

        //check if there is a need to close the src table (0 customers left)
        //there is no requirement to save this act in the actions log when it's part of the move act
        if (restaurant.getTable(srcTable)->getCustomers().size() <= 0) {
            Close close_src_table(srcTable);
            close_src_table.act(restaurant);
        }
        complete();
    } else {   //cannot move the customer
        std::cout << "Cannot move customer" << std::endl;
        error("Cannot move customer");
    }
}

//this function responsible to return the activation feedback of the move act
std::string MoveCustomer::toString() const {
    string moveStr = "";
    if (getStatus() == COMPLETED) { //completed successfully
        moveStr += "move " + to_string(srcTable) + " " + to_string(dstTable) + " " + to_string(id) + " Completed";
    } else { //ended with error
        moveStr += getErrorMsg();
    }
    return moveStr;
}

BaseAction* MoveCustomer::getActionInstance() {    //return a pointer for a action instance copy

    BaseAction* actionCopy=new MoveCustomer(srcTable,dstTable,id); //instance holds the copy of the action order
    return actionCopy;
}