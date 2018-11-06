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
        menu=other.menu;
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
        menu=other.menu;
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