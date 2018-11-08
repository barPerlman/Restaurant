#include "Action.h"
#include "Restaurant.h"
#include <vector>
#include <string>
/**
* A Action is an abstract class for the different Action classes.
 */

/*******************************************************
* Action Implementation
*******************************************************/

//Constructor
BaseAction::BaseAction(){}

ActionStatus BaseAction::getStatus() const {return status;}

void BaseAction::complete() { status=COMPLETED; }

void BaseAction::error(std::string errorMsg) {
    status=ERROR;
    errorMsg=errorMsg;
    std::cout<<errorMsg<<std::endl;
}

std::string BaseAction::getErrorMsg() const { return errorMsg; }


/**
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
    if(restaurant.getTable(tableId)!= nullptr && restaurant.getTable(tableId)->isOpen()) {
        restaurant.getTable(tableId)->closeTable();// update the boolean status of the table
        std::cout << "Table " << std::to_string(tableId) << " was closed. Bill " << std::to_string(restaurant.getTable(tableId)->getBill()) << "NIS"<<std::endl;

        // removes the old customers from the table and delete their orders
        for (Customer *customer:restaurant.getTable(tableId)->getCustomers())
            restaurant.getTable(tableId)->removeCustomer(customer->getId());
    }
    //  if the restaurant is not exist or is not open
    else if((!restaurant.getTable(tableId)->isOpen()) ||(restaurant.getTable(tableId)== nullptr))
        error("Table does not exist or is not open");
}


std::string Close::toString() const {
    std::string str = "Close"+std::to_string(tableId);
    if(getStatus()==COMPLETED)
        str+= "Completed";
    else str+= "Error" + getErrorMsg();
    return str;
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
    if(checkIfTablesOpen(restaurant)) // if at least one table is open
        for(int i=1; i<=restaurant.getNumOfTables(); i++) {
            Close close(i);
            close.act(restaurant);
        }
    //else לסגור את המסעדה
}

// checks if all the tables are open
bool CloseAll::checkIfTablesOpen(Restaurant restaurant) {
    for(int i=1; i<=restaurant.getNumOfTables(); i++)
        if(restaurant.getTable(i)->isOpen())
            return true;
    return false;
}

std::string CloseAll::toString() const {
    return "closeall Completed";
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
    for (Dish dish:restaurant.getMenu())
        std::cout<<dish.toString()<<std::endl;

}

std::string PrintMenu::toString() const {
    return "Print Menu Completed";
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
        std::cout<<"Table "<< std::to_string(tableId)<< " status:"
            <<std::to_string(restaurant.getTable(tableId)->isOpen())<<std::endl;// prints the status of the table

    else{// if the table is open
        std::string str = "Table "+ std::to_string(tableId)+ " status:";
        if(restaurant.getTable(tableId)->isOpen())
            str+="open";
        else str+="close";
        std::cout<<str<<std::endl; // prints the status of the table
        std::cout<<"Customers:"<<std::endl;
        int j=0;
        // prints the id of the customer and the name of the dish that he ordered
        for(size_t i=0; i<restaurant.getTable(tableId)->getOrders().size();i++){
            std::string str = std::to_string(restaurant.getTable(tableId)->getOrders()[j].first)+
                    restaurant.getTable(tableId)->getOrders()[j].second.getName();
        }
    }
}

std::string PrintTableStatus::toString() const {
    return "Print table "+std::to_string(tableId)+ " status Completed";
}