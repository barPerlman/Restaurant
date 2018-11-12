//
// Created by barper on 11/7/18.
//
#include "Action.h"
#include "Restaurant.h"
#include <iostream>
#include "Dish.h"
#include "Customer.h"
#include <string>
#include <fstream>
#include <vector>
using namespace std;

Order::Order(int id):BaseAction(),tableId(id){}

void Order::act(Restaurant &restaurant){
    //check that table with such id is exist and open
    if(tableId>=0&&tableId<restaurant.getNumOfTables() && restaurant.getTable(tableId)->isOpen()) {
        restaurant.getTable(tableId)->order(restaurant.getMenu());
        complete();
    }
    else{
        //check if should i print it to screen
        std::cout<<"Table does not exist or is not open"<<std::endl;
        error("Table does not exist or is not open");
    }
}
//create an order output string for log
std::string Order::toString() const{
    string orderStr="";
    if(getStatus()==COMPLETED){
        orderStr+="order "+to_string(tableId)+" Completed";
    }
    else{
        orderStr+=getErrorMsg();
    }
    return orderStr;
}

BaseAction* Order::getActionInstance() {    //return a pointer for a action instance copy

    BaseAction* actionCopy=new Order(*this); //instance holds the copy of the action order
    return actionCopy;
}