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
//constructor
OpenTable::OpenTable(int id, std::vector<Customer *> &customersList):BaseAction(),tableId(id),customers(customersList){}
//empty constructor

OpenTable::OpenTable():tableId(){

}

//destructor of openTable class
OpenTable::~OpenTable() {
    clear();
}


//copy constructor
OpenTable::OpenTable(const OpenTable &other):BaseAction(other),tableId(other.tableId) {
    //vector<Customer *> customerListCopy; //a copy of the customer list vector (deep)
    for (Customer *c:other.customers) {
        customers.push_back(c->getCustomerInstance()); //get copied instances of customers
    }
}

//move constructor
OpenTable::OpenTable(OpenTable &&other):customers(customers),tableId(tableId){

    for(Customer* c:other.customers){
        delete (c);
        c= nullptr;
    }
    customers.clear();
}

void OpenTable::clear(){
    for(Customer *customer:customers){
        delete (customer);
        customer= nullptr;
    }
}
//receive restaurant and perform the action open tabale on it
void OpenTable::act(Restaurant &restaurant){
    //can't open table
    if(tableId<0||tableId>=restaurant.getNumOfTables()||restaurant.getTable(tableId)->isOpen()||
    restaurant.getTable(tableId)->getCapacity()<customers.size()){
        error("Table does not exist or is already open");
        std::cout<<"Error: Table does not exist or is already open"<<std::endl;
    }
    else{      //can open table
        //add customers to the list
        for(Customer *c:customers){
            restaurant.getTable(tableId)->addCustomer(c);
        }
        restaurant.getTable(tableId)->openTable();
        complete(); //change status to complete
    }


}
//print a feedback to the open table command
std::string OpenTable::toString() const{
    string openStr=""; //holds the message to print
    if(getStatus()==COMPLETED) {    //command completed successfully
        openStr.append("open " + to_string(tableId));
        for (Customer *c:customers) {

            openStr=openStr+" " + c->toString();

        }
        openStr+=" Completed";
    }
    else{       //didn't complete successfully
        openStr=getErrorMsg();
    }
    return openStr;
}

BaseAction* OpenTable::getActionInstance() {    //return a pointer for a action instance copy

    BaseAction* actionCopy=new OpenTable(*this); //instance holds the copy of the action open table

    return actionCopy;
}