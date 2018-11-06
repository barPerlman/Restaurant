#ifndef RESTAURANT_H_
#define RESTAURANT_H_

#include <vector>
#include <string>
#include "Dish.h"
#include "Table.h"
#include "Action.h"


class Restaurant{		
public:
	Restaurant();
    Restaurant(const std::string &configFilePath);
    void start();
    int getNumOfTables() const;
    Table* getTable(int ind);
	const std::vector<BaseAction*>& getActionsLog() const; // Return a reference to the history of actions
    std::vector<Dish>& getMenu();


    //rule of 5
    virtual ~Restaurant();                  //destructor
    Restaurant(const Restaurant &other);    //copy constructor
    Restaurant(Restaurant &&other);         //move constructor
    Restaurant& operator=(const Restaurant &other); //copy assignment
    Restaurant& operator=(Restaurant&& other);      //move assignment

private:
    bool open;
    std::vector<Table*> tables;
    std::vector<Dish> menu;
    std::vector<BaseAction*> actionsLog;
    DishType stringToDishType(std::string s_d_type); //this convert string to my dishType enum

    void readConfigFile(std::ifstream &file, std::string &readLine);    //read config file
    //read capacities for the restaurant tables from conf. file
    void readTablesDescription(const std::string &delimiter, size_t pos, std::vector<int> &tablesDescription,
                               std::ifstream &file, std::string &readLine, std::string &token) const;
    //read the menu from the config file
    void readMenu( std::ifstream &file, std::string &readLine, const std::string &delimiter, size_t pos,
                  std::string &token);
    void clear(); //clear addresses safely
};

#endif