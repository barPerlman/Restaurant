#include "../include/Action.h"
#include "../include/Restaurant.h"
#include "../include/Dish.h"
#include "../include/Customer.h"
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
    }
        //  if the restaurant is not exist or is not open
    else{
        error("Table does not exist or is not open");
    }
}

Close::~Close() {}  //destructor
std::string Close::toString() const {
    string closeStr; //holds the message to print
    if(getStatus()==COMPLETED) {    //command completed successfully
        closeStr="close"+to_string(tableId)+" Completed";

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
CloseAll::~CloseAll() {}  //destructor
// close a table in the restaurant with tableId
void CloseAll::act(Restaurant &restaurant) {
    for(int i=0; i<restaurant.getNumOfTables(); i++) {
        if (restaurant.getTable(i)->isOpen()) {
            Close close(i);
            close.act(restaurant);
        }
    }

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
PrintMenu::~PrintMenu() {}  //destructor
// close a table in the restaurant with tableId
void PrintMenu::act(Restaurant &restaurant) {
    for (const Dish &dish:restaurant.getMenu())
        std::cout<<dish.toString()<<std::endl;
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
PrintTableStatus::~PrintTableStatus() {} //destructor
// close a table in the restaurant with tableId
void PrintTableStatus::act(Restaurant &restaurant) {
    if(!(restaurant.getTable(tableId)->isOpen()))// if the table is close
        std::cout<<"Table "<< std::to_string(tableId)<< " status: closed"<<std::endl;// prints the status of the table

    else{// if the table is open
        std::cout<<"Table "<< std::to_string(tableId)<< " status: open"<<std::endl;// prints the status of the table
        std::cout<<"Customers:"<<std::endl;
        // prints the id and the name of the customers in the table
        for(Customer* customer: restaurant.getTable(tableId)->getCustomers())
            std::cout<<std::to_string(customer->getId())+" "+customer->getName()<<std::endl;
        std::cout<<"Orders:"<<std::endl;
        // prints the name of the dish, the price of the dish and the id of each customer in the table
        for(OrderPair orderPair : restaurant.getTable(tableId)->getOrders())
            if(nameToPrice(orderPair.second.getName(),restaurant)!=-1)
                std::cout<<orderPair.second.getName()<<" "<<std::to_string(nameToPrice(orderPair.second.getName(),restaurant))<<"NIS "<<
                    to_string(orderPair.first)<<std::endl;
        std::cout<<"Current Bill: "<< std::to_string(restaurant.getTable(tableId)->getBill())<<"NIS"<<std::endl;
    }
}

// returns the price of the dish
int PrintTableStatus::nameToPrice(std::string name, Restaurant &restaurant) {
    for(Dish &dish : restaurant.getMenu())
        if(dish.getName()==name)
            return dish.getPrice();
    return -1;
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
BackupRestaurant::~BackupRestaurant() {} //destructor
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
    return "backup Completed";
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
RestoreResturant::~RestoreResturant() {} //destructor
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
/*******************************************************
* BaseAction Implementation
*******************************************************/
//constructor
BaseAction::BaseAction():errorMsg(""),status(PENDING){}

//destructor
//BaseAction::~BaseAction(){};


//getter
ActionStatus BaseAction::getStatus() const {
    return status;
}
//change status to complete
void BaseAction::complete(){
    status=COMPLETED;
}
//update error message and change status to error
void BaseAction::error(std::string errorMsg){
    status=ERROR;
    (*this).errorMsg="Error: "+errorMsg;
}
//getter
std::string BaseAction::getErrorMsg() const{
    return errorMsg;
}

//copy constructor
BaseAction::BaseAction(const BaseAction &other):errorMsg(other.errorMsg),status(other.status){}

BaseAction::~BaseAction(){

}


/*******************************************************
* MoveCustomer Implementation
*******************************************************/

MoveCustomer::MoveCustomer(int src, int dst, int customerId)
        : BaseAction(), srcTable(src), dstTable(dst), id(customerId) {}

MoveCustomer::~MoveCustomer() {} //destructor
//copy constructor
MoveCustomer::MoveCustomer(const MoveCustomer &other) : BaseAction(other), srcTable(other.srcTable),
                                                        dstTable(other.dstTable), id(other.id) {}

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
                OrderPair op(tableOrder.first,tableOrder.second);
                dstOrderList.push_back(op); //push this dish to the new table
            }
        }
        //remove customer and its orders from source table
        restaurant.getTable(srcTable)->removeCustomer(id);
        //check if there is a need to close the src table (0 customers left)
        //there is no requirement to save this act in the actions log when it's part of the move act
        if (restaurant.getTable(srcTable)->getCustomers().empty()) {
            restaurant.getTable(srcTable)->closeTable();  //close table with src id
        }
        complete(); //close status change to COMPLETED
    } else {   //cannot move the customer
        std::cout << "Cannot move customer" << std::endl;
        error("Cannot move customer");
    }
}

//this function responsible to return the activation feedback of the move act
std::string MoveCustomer::toString() const {
    string moveStr;
    if (getStatus() == COMPLETED) { //completed successfully
        moveStr = "move " + to_string(srcTable) + " " + to_string(dstTable) + " " + to_string(id) + " Completed";
    } else { //ended with error
        moveStr = getErrorMsg();
    }
    return moveStr;
}

BaseAction *MoveCustomer::getActionInstance() {    //return a pointer for a action instance copy

    BaseAction *actionCopy = new MoveCustomer(*this); //instance holds the copy of the action order
    return actionCopy;
}


/*******************************************************
*  OpenTable Implementation
*******************************************************/

//constructor
OpenTable::OpenTable(int id, std::vector<Customer *> &customersList):BaseAction(),tableId(id),customers(customersList){
/*if(this!= nullptr){
        for (Customer* c:customersList) {
            delete c;
        }
        customersList.clear();
    }*/
}

//empty constructor
OpenTable::OpenTable():BaseAction(),tableId(),customers(){

}

//move constructor
OpenTable::OpenTable(OpenTable &&other):BaseAction(),tableId(other.tableId),customers(std::move(other.customers)) {
    //steal other customers pointers
}


//destructor of openTable class
OpenTable::~OpenTable() {
    clear();
}


//copy constructor
OpenTable::OpenTable(const OpenTable &other):BaseAction(other),tableId(other.tableId),customers() {
    for (Customer *c:other.customers) {
        customers.push_back(c->getCustomerInstance()); //get copied instances of customers
    }
}



void OpenTable::clear(){
    for(Customer *customer:customers){
        if(customer!=nullptr){
        delete customer;
        customer= nullptr;
        }
    }
    customers.clear();
}
//receive restaurant and perform the action open tabale on it
void OpenTable::act(Restaurant &restaurant){
    //can't open table
    if(restaurant.getTable(tableId)->isOpen()||tableId<0||tableId>=restaurant.getNumOfTables()||
            (restaurant.getTable(tableId)->getCapacity())< static_cast<int>(customers.size())){
        error("Table does not exist or is already open");
        std::cout<<"Error: Table does not exist or is already open"<<std::endl;
        clear();
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
    string openStr="open " + to_string(tableId); //holds the message to print
        for (Customer *c:customers) {

            openStr+= " " + c->toString();

        }

    if(getStatus()!=COMPLETED){       //didn't complete successfully
        openStr+=" "+getErrorMsg();
    } else{
        openStr+=" Completed";
    }
    return openStr;
}

BaseAction* OpenTable::getActionInstance() {    //return a pointer for a action instance copy

    BaseAction* actionCopy=new OpenTable(*this); //instance holds the copy of the action open table

    return actionCopy;
}




/*******************************************************
* Order Implementation
*******************************************************/

Order::Order(int id):BaseAction(),tableId(id){}
Order::~Order() {}  //destructor
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
    string orderStr;
    if(getStatus()==COMPLETED){
        orderStr="order "+to_string(tableId)+" Completed";
    }
    else{
        orderStr=getErrorMsg();
    }
    return orderStr;
}

BaseAction* Order::getActionInstance() {    //return a pointer for a action instance copy

    BaseAction* actionCopy=new Order(*this); //instance holds the copy of the action order
    return actionCopy;
}

/*******************************************************
* PrintActionsLog Implementation
*******************************************************/

//constructor
PrintActionsLog::PrintActionsLog():BaseAction(){}
PrintActionsLog::~PrintActionsLog() {}  //destructor
//prints all of the to strings of the actions were performed so far in the restaurant
void PrintActionsLog::act(Restaurant &restaurant){
    for(BaseAction* action:restaurant.getActionsLog()){
        std::cout<<action->toString()<<std::endl;
    }
}
//log action is not displayed according to the specifications document
std::string PrintActionsLog::toString() const{
    return "";
}

BaseAction* PrintActionsLog::getActionInstance() {    //return a pointer for a action instance copy

    BaseAction* actionCopy=new PrintActionsLog(*this); //instance holds the copy of the action order
    return actionCopy;
}

