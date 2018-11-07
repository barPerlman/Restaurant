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
#include <vector>
using namespace std;

/////////////////////////Restaurant Class functions/////////////////////////////////////
//default empty constructor
Restaurant::Restaurant(){}

//constructor
Restaurant::Restaurant(const std::string &configFilePath):open(false) {
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
    open=true;
    std::cout<<"Restaurant is now open!"<<std::endl;
    string exeCommand;  //holds the current command to execute received by the user
    do{//get the input command to execute from user in loop
        std::getline(std::cin,exeCommand);
        size_t pos = exeCommand.find(" ");   //use space as delimeter and take the first word
        string firstWord= exeCommand.substr(0, pos);    //split

        //the command cases:
        if(firstWord=="open"){

            string delimeter=" ";
            size_t pos=exeCommand.find(delimeter);
            string tableIdStr=exeCommand.substr(0, pos);    //split
            exeCommand.erase(0, pos + delimeter.length());
            openTable(exeCommand);
        }
        else if(firstWord=="order"){
            orderFromTable(exeCommand);
        }
        else if(firstWord=="move"){
            moveCustomer(exeCommand);
        }
        else if(firstWord=="close"){
            closeTable(exeCommand);
        }
        else if(firstWord=="closeall"){
            closeAllTables();
        }
        else if(firstWord=="menu"){
            printMenu();
        }
        else if(firstWord=="status"){
            printTableStatus(exeCommand);
        }
        else if(firstWord=="log"){
            printActionsLog();
        }
        else if(firstWord=="backup"){
            backupRestaurant();
        }
        else if(firstWord=="restore"){
            restoreRestaurant();
        }
    }while(exeCommand!="closeall");
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
//destructor of restaurant class
Restaurant::~Restaurant() {
    clear();
}
//remove sources of restaurant
void Restaurant::clear() {
    //remove tables vector
    for(Table *table:tables){
        delete[] table;
        table= nullptr;
    }
    //remove base actions log
    for(BaseAction *baseAction:actionsLog){
        delete [] baseAction;
        baseAction= nullptr;
    }
}
//copy constructor
Restaurant::Restaurant(const Restaurant &other):
tables(other.tables),
menu(other.menu),
actionsLog(other.actionsLog){}

//move constructor
Restaurant::Restaurant(Restaurant &&other):tables(other.tables),menu(other.menu),actionsLog(other.actionsLog) {
    //assign nullptr in vectors values:
    //for tables:
    for (Table *table:other.tables) {
        delete[] table;
        table = nullptr;
    }
    //for actions log:
    for(BaseAction *action:other.actionsLog){
        delete[] action;
        action= nullptr;
    }

}

//copy assignment
Restaurant& Restaurant::operator=(const Restaurant &other) {
    if(this!=&other){
        clear();
        tables=other.tables;

        //assignment of menu
        menu.clear();
        for(Dish d:other.menu){
            menu.push_back(d);
        }
        actionsLog=other.actionsLog;
    }
    return *this;
}
//move assignment
//

Restaurant& Restaurant::operator=(Restaurant &&other) {
    if(this!=&other){
        clear();
        tables=other.tables;

        //assignment of menu
        menu.clear();
        for(Dish d:other.menu){
            menu.push_back(d);
        }
        actionsLog=other.actionsLog;
        //assign nullptr in vectors values:
        //for tables:
        for (Table *table:other.tables) {
            delete[] table;
            table = nullptr;
        }
        //for actions log:
        for(BaseAction *action:other.actionsLog){
            delete[] action;
            action= nullptr;
        }
    }
    return *this;
}

void Restaurant::openTable(string &exeCommand){

        //split the execution sentence to its parts:
        string command=exeCommand;  //copy of the command sentence
        string delimeter=" ";
        size_t pos=command.find(delimeter); //position in command sentence
        string tableIdStr=command.substr(0, pos);    //split

        command.erase(0, pos + delimeter.length());


        //read the customers with their strategyType
        string pairNameType;
        string CustomerName;    //name of customer
        string CustomerType;    //typr of customer
        //1.read till the pair space
        //2.split them and create instance of customer on heap
        //3.push pointer to instance into vector of pointer
        //send vector of customers and table id to open table constructor
        vector <Customer*> customersList;
        int customerId=0;
        size_t posPair;
        while((pos=command.find(delimeter))!=std::string::npos){

            pairNameType = command.substr(0, pos);    //get name with type

            command.erase(0, pos + delimeter.length()); //remove red part from sentence
            //separate pair
            pos=pairNameType.find(",");  //update last index in strin to read
            CustomerName=pairNameType.substr(0,pos);
            pairNameType.erase(0,pos+1); //remove red part from pair sentence

            pos=pairNameType.find('\0');    //tells where type ends
            CustomerType=pairNameType.substr(0,pos);    //read type
            pos=0;

            //create a customer and push into vector
            buildCustomersPointersVector(CustomerName,customerId,CustomerType,customersList);
            customerId++;   //raise the id of next customer

        }
        //read the last pair from command
        pairNameType = command.substr(0, pos);    //get name with type
        pos=pairNameType.find(",");  //update last index in string to read
        CustomerName=pairNameType.substr(0,pos);
        pairNameType.erase(0,pos+1); //remove red part from pair sentence
        pos=pairNameType.find('\0');    //tells where type ends
        CustomerType=pairNameType.substr(0,pos);    //read type
        //create a customer and push into vector
        buildCustomersPointersVector(CustomerName,customerId,CustomerType,customersList);

        //create instance of the action open table and save it to log
        BaseAction *open_table=new OpenTable(stoi(tableIdStr),customersList);
        actionsLog.push_back(open_table);   //push action to action log
        //perform open table and send the restaurant as parameter
        open_table->act(*this);
        //print feedback from operation
        std::cout<<open_table->toString()<<std::endl;


}
void Restaurant::orderFromTable(string &exeCommand){}
void Restaurant::moveCustomer(string &exeCommand){}
void Restaurant::closeTable(string &exeCommand){}
void Restaurant::closeAllTables(){}
void Restaurant::printMenu(){}
void Restaurant::printTableStatus(string &exeCommand){}
void Restaurant::printActionsLog(){}
void Restaurant::backupRestaurant(){}
void Restaurant::restoreRestaurant(){}


//this function creates and push the suit type of customer into customers list
void Restaurant::buildCustomersPointersVector(string CustomerName,int customerId,string CustomerType,vector<Customer*> &customersList){
       Customer *customer;
        if(CustomerType=="veg"){
            customer=new VegetarianCustomer(CustomerName,customerId);
        }
        else if(CustomerType=="chp"){
            customer=new CheapCustomer(CustomerName,customerId);
        }
        else if(CustomerType=="spc"){
            customer=new SpicyCustomer(CustomerName,customerId);
        }
        else if(CustomerType=="alc"){
            customer=new AlchoholicCustomer(CustomerName,customerId);
        }
        customersList.push_back(customer);
}
