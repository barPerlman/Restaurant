#include "Restaurant.h"
#include "Table.h"//
// Created by barper on 11/3/18.
// The following classes are implemented in this file:
//Restaurant,Table,Dish
//
#include "Restaurant.h"
#include <iostream>
#include "Dish.h"
#include "Customer.h"
#include <string>
#include <fstream>
using namespace std;
/////////////////////////Dish Class functions/////////////////////////////////////

//constructor
Dish::Dish(int d_id, std::string d_name, int d_price, DishType d_type):id(d_id),name(d_name),price(d_price),type(d_type) {}

//getter for dish id
 int Dish::getId() const{
    return id;
}

//getter for the dish name
std::string Dish::getName()const {
    return name;
}

//getter for the dish price
 int Dish::getPrice()const{
    return price;
}
//getter for the dish type
 DishType Dish::getType()const{
    return type;
}

//Dish print in the required format
string Dish::toString() const {
    //convert to string in aim to return concat string
    string typeStr;
    switch(type){
        case VEG:{
            typeStr="VEG";
            break;}
        case SPC:{
            typeStr="SPC";
            break;}
        case BVG:{
            typeStr="BVG";
            break;}
        case ALC:{
            typeStr="ALC";
            break;}
    }
    string priceStr=to_string(price);
    string dishString=to_string(id)+" "+name+" "+typeStr+" "+priceStr+"NIS";
    return dishString;

}

/////////////////////////Table Class functions/////////////////////////////////////
//constructor
Table::Table(int t_capacity):capacity(t_capacity){}

//getter of table capacity
int Table::getCapacity() const{
    return capacity;
}
//add a customer to the table as last element in vector
void Table::addCustomer(Customer* customer){
    //it's possible to add a customer to the table
    if(customersList.size()<capacity&&!open){
        customersList.push_back(customer);//add customer to table at end of list
    } else{
        std::cout<<"Cannot add a Customer to the table. it's open or full!"<<std::endl;
    }

}

//remove customer with 'id' from customers list
//remove orders of customer with 'id' from orders list
void Table::removeCustomer(int id){
    int    i=0;
    bool found=false;   //a flag tells if the customer removed
    while(i<customersList.size()&&!found){
        Customer *&currCustomer=customersList.at(i);
        if(currCustomer->getId()==id){   //found such customer
            customersList.erase(customersList.begin()+i);
           found=true;
        }
        i++;
    }
    //if a customer is removed, remove his orders out of the table
    if(found){
        vector<OrderPair> orderListCopy(orderList);  //deep copy orders to temp list orders
        orderList.clear();
        for(int i=0;i<orderListCopy.size();i++){    //get back to origin vetor the orders without orders attributed to the 'id'
            if(orderListCopy.at(i).first!=id){
                orderList.push_back(orderListCopy.at(i));
            }
        }

    }
}

//return a customer with matching id from the list
Customer* Table::getCustomer(int id){
    for(int i=0;i<customersList.size();i++){
        Customer*& currCustomer=customersList.at(i);
        if(currCustomer->getId()==id){   //found such customer
            return currCustomer;
        }
    }
    return nullptr;
}
///////////למחוק את כל אלה.. לבדיקה בלבד ומניעת שגיאות קומפילציה!!!!////
int Customer::getId() const{
    return 1;
}



///////////////////////////////////////////////////


//return a ref. for the customers list
std::vector<Customer*>& Table::getCustomers(){
    vector<Customer*>& customersRef=customersList;
    return customersRef;
}
//return a ref. for the orders list
std::vector<OrderPair>& Table::getOrders(){
    vector<OrderPair>& orderRef=orderList;
    return orderRef;
}
///////////define after understanding what is it????
void Table::order(const std::vector<Dish> &menu){
    std::cout<<"Orders:"<<std::endl;
    for(int i=0;i<menu.size();i++){
        std::cout<<menu.at(i).toString()<<std::endl;
    }
}
//open table by change its status
void Table::openTable(){
    open=true;
}
//close table by change its status
void Table::closeTable(){
    open=false;
}
//get the sum of the dishes ordered together
int Table::getBill(){
    int billSum=0;
    for(int i=0;i<orderList.size();i++){
        billSum+=orderList.at(i).second.getPrice();
    }
    return billSum;
}
//get the table status (open or closed)
bool Table::isOpen(){
    return open;
}

/////////////////////////Restaurant Class functions/////////////////////////////////////
//default empty constructor
Restaurant::Restaurant(){}

//constructor
Restaurant::Restaurant(const std::string &configFilePath):open(true) {
    std::ifstream file(configFilePath);//open file to read
    string readLine; //holds the current read line from the file
    do{
        std::getline(file,readLine);   //get the current read line from file
                    readConfigFile(file, readLine); //go to function that read the parameters out of the file
    }while (!file.eof());   //read from file till we red all of it
    file.close();   //close file
}
//function that read the parameters out of the file
void Restaurant::readConfigFile(ifstream &file, string &readLine) {

    string delimiter = ",";    //use , as delimeter
    size_t pos = 0;   //start position for read section
    string token;   //holds the read section
    //get tables amount
    int tablesAnount;   //holds the amount of tables from file
    vector<int> tablesDescription;  //vector of capacities of each table
    getline(file, readLine);    //read next line
    tablesAnount = stoi(readLine);
    //send to function which get the capacities parameters of the tables
    readTablesDescription(delimiter, pos, tablesDescription, file, readLine, token);
    getline(file,readLine); //next line
    getline(file,readLine); //next line
    readMenu(file, readLine, delimiter, pos, token);    //call to function which read the menu from file

    //initialization of tables in the restaurant
    for(int i=0;i<tablesAnount;i++){
        Table *t_table=new Table(tablesDescription.at(i));
        tables.push_back(t_table);
    }
}
//this function initialize the Dishes vector
void Restaurant::readMenu(ifstream &file, string &readLine, const string &delimiter, size_t pos, string &token) {
    int iter=0; //id of the dish
    while ((pos = readLine.find(delimiter)) != std::string::npos) {
                        token = readLine.substr(0, pos);    //split
                        string d_name=token;
                        readLine.erase(0, d_name.length()+1);
                        token = readLine.substr(0, readLine.find(delimiter));    //split
                        DishType d_type= stringToDishType(token);
                        readLine.erase(0, token.length()+1);
                        token = readLine.substr(0, readLine.find(delimiter));    //split
                        string d_price=token;
                        readLine.erase(0, d_price.length());
                        Dish dish(iter,d_name,stoi(d_price),d_type);
                        //push Dish into vector
                        menu.push_back(dish);
                        iter++;
                        pos=0;
                        getline(file,readLine);
                    }
}


void Restaurant::readTablesDescription(const string &delimiter, size_t pos, vector<int> &tablesDescription, ifstream &file,
                                  string &readLine, string &token) const {
    getline(file, readLine);    //read next line
    getline(file, readLine);    //read next line


//read and split as long as we are not in the end of the line
    while ((pos = readLine.find(delimiter)) != std::string::npos) {
                        token = readLine.substr(0, pos);    //split
                        //push capacity into vector
                        tablesDescription.push_back(stoi(token));
                        //std::cout << token << std::endl;
                        readLine.erase(0, pos + delimiter.length());
                    }
//push last capacity from file to vector of capacities
    tablesDescription.push_back(stoi(readLine));
}
//this function get eat strategy as string and returns its representation as enum
DishType Restaurant::stringToDishType(std::string s_d_type) {
    if(s_d_type=="VEG"){
        return VEG;
    }
    else if(s_d_type=="SPC"){
        return SPC;
    }
    else if(s_d_type=="BVG"){
        return BVG;
    }
    else if(s_d_type=="ALC"){
        return ALC;
    }
}
//this function responsible for the restaurant management
void Restaurant::start() {
    std::cout<<"Restaurant is now open!"<<std::endl;
}
//get the amount of tables in restaurant
int Restaurant::getNumOfTables() const{
    return tables.size();
}
//return the table in the required index or nullptr if doesn't exist
Table* Restaurant::getTable(int ind){
    if(ind<getNumOfTables()) {
        Table *&requiredTable = tables.at(ind);
        return requiredTable;
    }
    return nullptr;
}
// Return a reference to the history of actions
const std::vector<BaseAction*>& Restaurant::getActionsLog() const{
    const vector<BaseAction*>& log=actionsLog; //define a const copy of actions log with read only permission
    return log;
}
//return a ref. to the menu
std::vector<Dish>& Restaurant::getMenu(){
    vector<Dish>& menuCopy=menu;
    return menuCopy;
}
