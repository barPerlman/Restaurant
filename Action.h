#ifndef ACTION_H_
#define ACTION_H_

#include <string>
#include <iostream>
#include "Customer.h"

enum ActionStatus{
    PENDING, COMPLETED, ERROR
};

//Forward declaration
class Restaurant;

class BaseAction{
public:
    virtual BaseAction* getActionInstance()=0;    //return pointer to new instance of the actual action class
    BaseAction();
    ActionStatus getStatus() const;
    virtual void act(Restaurant& restaurant)=0;
    virtual std::string toString() const=0;
protected:
    void complete();
    void error(std::string errorMsg);
    std::string getErrorMsg() const;
private:
    std::string errorMsg;
    ActionStatus status;
};


class OpenTable : public BaseAction {
public:
    BaseAction* getActionInstance();
    //destructor
    virtual ~OpenTable(); //destructor
    OpenTable(const OpenTable &other);    //copy constructor
    OpenTable(OpenTable &&other);         //move constructor
    OpenTable();                            //empty constructor



    OpenTable(int id, std::vector<Customer *> &customersList);
    void act(Restaurant &restaurant);
    std::string toString() const;
private:
    void clear();
	const int tableId;
	std::vector<Customer *> customers;
};


class Order : public BaseAction {
public:
	BaseAction* getActionInstance();

	Order(int id);
    void act(Restaurant &restaurant);
    std::string toString() const;
private:
    const int tableId;
};


class MoveCustomer : public BaseAction {
public:
	BaseAction* getActionInstance();

	MoveCustomer(int src, int dst, int customerId);
    void act(Restaurant &restaurant);
    std::string toString() const;
private:
    const int srcTable;
    const int dstTable;
    const int id;
};


class Close : public BaseAction {
public:
	BaseAction* getActionInstance();

	Close(int id);
    void act(Restaurant &restaurant);
    std::string toString() const;
private:
    const int tableId;
};


class CloseAll : public BaseAction {
public:
	BaseAction* getActionInstance();

	CloseAll();
    void act(Restaurant &restaurant);
    std::string toString() const;
private:
};


class PrintMenu : public BaseAction {
public:
	BaseAction* getActionInstance();

	PrintMenu();
    void act(Restaurant &restaurant);
    std::string toString() const;
private:
};


class PrintTableStatus : public BaseAction {
public:
	BaseAction* getActionInstance();

	PrintTableStatus(int id);
    void act(Restaurant &restaurant);
    std::string toString() const;
private:
    const int tableId;
    int nameToPrice(std::string name, Restaurant &restaurant); // returns the price of the dish
};


class PrintActionsLog : public BaseAction {
public:
	BaseAction* getActionInstance();

	PrintActionsLog();
    void act(Restaurant &restaurant);
    std::string toString() const;
private:
};


class BackupRestaurant : public BaseAction {
public:
	BaseAction* getActionInstance();

	BackupRestaurant();
    void act(Restaurant &restaurant);
    std::string toString() const;
private:
};


class RestoreResturant : public BaseAction {
public:
	BaseAction* getActionInstance();

	RestoreResturant();
    void act(Restaurant &restaurant);
    std::string toString() const;

};


#endif