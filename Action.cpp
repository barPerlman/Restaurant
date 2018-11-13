#include "Action.h"
#include "Restaurant.h"
#include <iostream>
#include "Dish.h"
#include "Customer.h"
#include <string>
#include <fstream>
#include <vector>
using namespace std;
extern Restaurant* backup;
/**
**
* A Close action is the class that closes a table.
 */

/*******************************************************
* Close Implementation
*******************************************************/

//Constructor
Close::Close(int id) : BaseAction(), tableId(id) {}

// close a table in the restaurant with tableId
void Close::act(Restaurant &restaurant) {
    //  if the restaurant is open and exist
    if(tableId>=0&&tableId<restaurant.getNumOfTables() && restaurant.getTable(tableId)->isOpen()) {
        std::cout << "Table " << std::to_string(tableId) << " was closed. Bill " << std::to_string(restaurant.getTable(tableId)->getBill()) << "NIS"<<std::endl;
        //close table
        restaurant.getTable(tableId)->closeTable();// update the boolean status of the table
	complete();
    }
        //  if the restaurant is not exist or is not open
    else{
        error("Table does not exist or is not open");
    }
}


std::string Close::toString() const {
    string closeStr; //holds the message to print
    if(getStatus()==COMPLETED) {    //command completed successfully
        closeStr="closed"+to_string(tableId)+" Completed";

    }
    else{       //didn't complete successfully
        closeStr=getErrorMsg();
    }
    return closeStr;
}

BaseAction* Close::getActionInstance() {    //return a pointer for a action instance copy

    BaseAction* actionCopy=new Close(*this); //instance holds the copy of the action order
    return actionCopy;
}

/**
* A CloseAll action is the class that closes all the tables.
 */

/*******************************************************
* CloseAll Implementation
*******************************************************/

//Constructor
CloseAll::CloseAll():BaseAction(){}

// close a table in the restaurant with tableId
void CloseAll::act(Restaurant &restaurant) {
    for(int i=1; i<=restaurant.getNumOfTables(); i++) {
        if (restaurant.getTable(i)->isOpen()) {
            Close close(i);
            close.act(restaurant);
        }
    }
complete();
}


std::string CloseAll::toString() const {
    return "closeall Completed";
}

BaseAction* CloseAll::getActionInstance() {    //return a pointer for a action instance copy

    BaseAction* actionCopy=new CloseAll(*this); //instance holds the copy of the action order
    return actionCopy;
}

/**
* A PrintMenu action is the class that prints the menu of the restaurant.
 */

/*******************************************************
* PrintMenu Implementation
*******************************************************/

//Constructor
PrintMenu::PrintMenu():BaseAction(){}

// close a table in the restaurant with tableId
void PrintMenu::act(Restaurant &restaurant) {
    for (const Dish &dish:restaurant.getMenu()){
	std::cout<<dish.toString()<<std::endl;
	}
complete();
}

std::string PrintMenu::toString() const {
    return "Print Menu Completed";
}

BaseAction* PrintMenu::getActionInstance() {    //return a pointer for a action instance copy

    BaseAction* actionCopy=new PrintMenu(*this); //instance holds the copy of the action order
    return actionCopy;
}

/**
* A PrintTableStatus action is the class that prints the status report of a given table.
 */

/*******************************************************
* PrintTableStatus Implementation
*******************************************************/

//Constructor
PrintTableStatus::PrintTableStatus(int ind):BaseAction(),tableId(ind){}

// close a table in the restaurant with tableId
void PrintTableStatus::act(Restaurant &restaurant) {
    if(!(restaurant.getTable(tableId)->isOpen()))// if the table is close
        std::cout<<"Table "<< std::to_string(tableId)<< " status: close"<<std::endl;// prints the status of the table

    else{// if the table is open
        std::cout<<"Table "<< std::to_string(tableId)<< " status: open"<<std::endl;// prints the status of the table
        std::cout<<"Customers:"<<std::endl;
        // prints the id and the name of the customers in the table
        for(Customer* customer: restaurant.getTable(tableId)->getCustomers())
            std::cout<<std::to_string(customer->getId())+" "+customer->getName()<<std::endl;
        std::cout<<"Orders:"<<std::endl;
        // prints the name of the dish, the price of the dish and the id of each customer in the table
        for(OrderPair orderPair : restaurant.getTable(tableId)->getOrders())
            std::cout<<orderPair.second.getName()<<" "<<std::to_string(nameToPrice(orderPair.second.getName(),restaurant))<<"NIS "<<
                to_string(orderPair.first)<<std::endl;
        std::cout<<"Current Bill: "<< std::to_string(restaurant.getTable(tableId)->getBill())<<"NIS"<<std::endl;
    }
complete();
}

// returns the price of the dish
int PrintTableStatus::nameToPrice(std::string name, Restaurant &restaurant) {
    for(Dish &dish : restaurant.getMenu())
        if(dish.getName()==name)
            return dish.getPrice();
}

std::string PrintTableStatus::toString() const {
    return "status "+std::to_string(tableId)+ " Completed";
}

BaseAction* PrintTableStatus::getActionInstance() {    //return a pointer for a action instance copy

    BaseAction* actionCopy=new PrintTableStatus(*this); //instance holds the copy of the action order
    return actionCopy;
}

/**
* A BackupRestaurant action is the class that saves all the restaurant information
 */

/*******************************************************
* BackupRestaurant Implementation
*******************************************************/

//Constructor
BackupRestaurant::BackupRestaurant():BaseAction(){}

// saves all the restaurant information
void BackupRestaurant::act(Restaurant &restaurant) {
    if(backup== nullptr) // if is the first time of the backup
        backup=new Restaurant(restaurant);//use the copy constructor
    else {
        *backup = restaurant; //copy assignment
    }
    complete();
}

std::string BackupRestaurant::toString() const {
    return "Backup Completed";
}

BaseAction* BackupRestaurant::getActionInstance() {    //return a pointer for a action instance copy

    BaseAction* actionCopy=new BackupRestaurant(*this); //instance holds the copy of the action order
    return actionCopy;
}

/**
* A RestoreResturant action is the class that saves all the restaurant information
 */

/*******************************************************
* RestoreResturant Implementation
*******************************************************/

//Constructor
RestoreResturant::RestoreResturant():BaseAction(){}

// saves all the restaurant information
void RestoreResturant::act(Restaurant &restaurant) {
    if(backup== nullptr){
        error("No backup available");
    }
    else{
        restaurant=*backup;
        complete();
    }
}

std::string RestoreResturant::toString() const {
    string closeStr;// holds the message to print
    if(getStatus()==COMPLETED) {// command completed successfully
        closeStr="restore Completed";
    }
    else{// didn't complete successfully
        closeStr=getErrorMsg();
    }
    return closeStr;
}

BaseAction* RestoreResturant::getActionInstance() {    //return a pointer for a action instance copy

    BaseAction* actionCopy=new RestoreResturant(*this); //instance holds the copy of the action order
    return actionCopy;
}

