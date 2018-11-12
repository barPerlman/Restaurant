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
#include "Action.h"

using namespace std;

/////////////////////////Restaurant Class functions/////////////////////////////////////
/////////////////////////Restaurant Class functions/////////////////////////////////////
//default empty constructor
Restaurant::Restaurant():lastId(0),open(false){}

//constructor
Restaurant::Restaurant(const std::string &configFilePath):lastId(0),open(false) {
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
    //start read file after amount of tables
    while (readLine!="#number of tables\r"){
        getline(file,readLine);
    }
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
            //remove the part which is not relevant anymore of the sentence
            string delimeter=" ";
            size_t pos=exeCommand.find(delimeter);
            string tableIdStr=exeCommand.substr(0, pos);    //split
            exeCommand.erase(0, pos + delimeter.length());
            openTable(exeCommand);  //call for a function to start the openTable process with command string
        }
        else if(firstWord=="order"){
            //remove the part which is not relevant anymore of the sentence
            string delimeter=" ";
            size_t pos=exeCommand.find(delimeter);
            string tableIdStr=exeCommand.substr(0, pos);    //split
            exeCommand.erase(0, pos + delimeter.length());
            orderFromTable(exeCommand); //call for a function to start the order process with command string
        }
        else if(firstWord=="move"){
            //remove the part which is not relevant anymore of the sentence
            string delimeter=" ";
            size_t pos=exeCommand.find(delimeter);
            string tableIdStr=exeCommand.substr(0, pos);    //split
            exeCommand.erase(0, pos + delimeter.length());
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
            string delimeter=" ";
            size_t pos=exeCommand.find(delimeter);
            string tableIdStr=exeCommand.substr(0, pos);    //split
            exeCommand.erase(0, pos + delimeter.length());
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
    if(ind>=0 && ind<getNumOfTables()) {
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
        delete table;
        table= nullptr;
    }
    tables.clear();
    //remove base actions log
    for(BaseAction *baseAction:actionsLog){
        delete  baseAction;
        baseAction= nullptr;
    }
    actionsLog.clear();

    menu.clear();
}
//copy constructor
Restaurant::Restaurant(const Restaurant &other): lastId(lastId),open(other.open),menu(other.menu){

    //initialization of tables in the restaurant
    for(int i=0;i<other.getNumOfTables();i++) {
        Table *t_table = new Table(other.tables.at(i)->getCapacity());
        tables.push_back(t_table);
    }

    //copy the actions log and take the relevant data from
    for(int i=0;i<other.getActionsLog().size();i++){
        BaseAction* actionCopy=other.getActionsLog().at(i)->getActionInstance();    //get a pointer to new copy of action instance
        actionsLog.push_back(actionCopy);   //push action copy instance to actions log
    }
    //copy the data into tables
    for(Table* table:other.tables)//int i=0;i<other.getNumOfTables();i++) {
    {
        Table t=*table;
        tables.push_back(new Table(t));
    }


}


//move constructor
Restaurant::Restaurant(Restaurant &&other):tables(other.tables),menu(other.menu),actionsLog(other.actionsLog) {
    //assign nullptr in vectors values:
    //for tables:
    for (Table *table:other.tables) {
        delete table;
        table = nullptr;
    }
    //for actions log:
    for(BaseAction *action:other.actionsLog){
        delete action;
        action= nullptr;
    }

}

//copy assignment
Restaurant& Restaurant::operator=(const Restaurant &other) {
    if(this!=&other){
        clear();

        //////////////////////////////copy constructor part///////////////////////////////////
        //copy the actions log and take the relevant data from
        for(int i=0;i<other.getActionsLog().size();i++){
            BaseAction* actionCopy=other.getActionsLog().at(i)->getActionInstance();    //get a pointer to new copy of action instance
            actionsLog.push_back(actionCopy);   //push action copy instance to actions log
        }
        //copy the data into tables
        for(Table* table:other.tables)//int i=0;i<other.getNumOfTables();i++) {
        {
            Table t=*table;
            tables.push_back(new Table(t));
        }
        ///////////////////////////////////////////////////////////////////////////////////
        //assignment of menu
        for(Dish d:other.menu){
            menu.push_back(d);
        }
        lastId=other.lastId;
        open=other.open;
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
            delete table;
            table = nullptr;
        }
        //for actions log:
        for(BaseAction *action:other.actionsLog){
            delete action;
            action= nullptr;
        }
    }
    return *this;
}
//this function make instance of action that response of opening a table in the restaurant
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
        int customerId=lastId;
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
        //save the last id for next customer
        lastId=customerId+1;
        //create instance of the action open table and save it to log
        BaseAction *open_table=new OpenTable(stoi(tableIdStr),customersList);
        actionsLog.push_back(open_table);   //push action to action log
        //perform open table and send the restaurant as parameter
        open_table->act(*this);



}
//this function make instance of action that response of order from a table in the restaurant
void Restaurant::orderFromTable(string &exeCommand){
    BaseAction *order_from_table=new Order(stoi(exeCommand));
    actionsLog.push_back(order_from_table);
    order_from_table->act(*this);
}

//this function make instance of action that response of moving a customer from table to table
void Restaurant::moveCustomer(string &exeCommand){
    int originTable,destTable,customerId;   //those are the initialize fields for the move constructor
    size_t pos;

    //read and split as long as we are not in the end of the line
  //  while ((pos = exeCommand.find(" ")) != std::string::npos) {
   //     token = readLine.substr(0, pos);    //split
   //     //push capacity into vector
   //     tablesDescription.push_back(stoi(token));
    //    readLine.erase(0, pos + delimiter.length());
    //}

    pos=exeCommand.find(" ");   //position of end character of current splitted part
    originTable=stoi(exeCommand.substr(0,pos));  //get the origin table from command and convert to int
    exeCommand.erase(0,pos+1);
    pos=exeCommand.find(" ");   //position of end character of current splitted part
    destTable=stoi(exeCommand.substr(0,pos));
    exeCommand.erase(0,pos+1);
    customerId=stoi(exeCommand);
    //create action instance type of move customer
    BaseAction *move_customer=new MoveCustomer(originTable,destTable,customerId);
    actionsLog.push_back(move_customer);    //push into actions log
    move_customer->act(*this);  //perform the activation of move customer

}

//this function make instance of action that response of close table

void Restaurant::closeTable(string &exeCommand) {


    string delimeter = " ";
    size_t pos = exeCommand.find(delimeter);
    exeCommand.erase(0, pos + delimeter.length());
    pos = exeCommand.find('\r');
    string tableIdStr = exeCommand.substr(0, pos);    //split

    BaseAction *close_table = new Close(stoi(tableIdStr));
    close_table->act(*this);
    actionsLog.push_back(close_table);
    std::cout<<close_table->toString()<<std::endl;




}
//this function make instance of action that response of close all tables in restaurant
void Restaurant::closeAllTables(){

    BaseAction *close_all=new CloseAll();
    actionsLog.push_back(close_all);
    close_all->act(*this);



    open=false; //close restaurant
}
//this function make instance of action that response of printing the restaurant menu
void Restaurant::printMenu(){
    BaseAction *print_menu=new PrintMenu();
    actionsLog.push_back(print_menu);
    print_menu->act(*this);
}
//this function make instance of action that response of printing table status
void Restaurant::printTableStatus(string &exeCommand){
    BaseAction * print_table_status=new PrintTableStatus(stoi(exeCommand));
    actionsLog.push_back(print_table_status);
    print_table_status->act(*this);
}
//this function make instance of action that response of displaying the action log
void Restaurant::printActionsLog(){
    BaseAction * log=new PrintActionsLog();
    log->act(*this);
}
//this function make instance of action that response of back up restaurant states
void Restaurant::backupRestaurant(){
    BaseAction * backup=new BackupRestaurant();
    actionsLog.push_back(backup);
    backup->act(*this);
}
//this function make instance of action that response of restoring to last restaurant backup
void Restaurant::restoreRestaurant(){
    BaseAction * restore=new RestoreResturant();
    actionsLog.push_back(restore);
    restore->act(*this);
}


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
