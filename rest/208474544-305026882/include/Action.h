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
    BaseAction();									//empty constructor
    BaseAction(const BaseAction &other);        //copy constructor
    virtual ~BaseAction(); // Destructor

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
    BaseAction* getActionInstance() override;   //this return a pointer to actual instance of action
    //destructor
    virtual ~OpenTable() ; //destructor
    OpenTable(const OpenTable &other);    //copy constructor
    OpenTable(OpenTable &&other);         //move constructor
    OpenTable();                            //empty constructor




    OpenTable(int id, std::vector<Customer *> &customersList);
    void act(Restaurant &restaurant) override;
    std::string toString() const override;
private:
    void clear();
	const int tableId;
	std::vector<Customer *> customers;
};


class Order : public BaseAction {
public:
	BaseAction* getActionInstance() override;
	virtual ~Order() ; //destructor

	explicit Order(int id);
    void act(Restaurant &restaurant) override;
    std::string toString() const override;
private:
    const int tableId;
};


class MoveCustomer : public BaseAction {
public:
	virtual ~MoveCustomer(); //destructor

	BaseAction* getActionInstance() override;
    MoveCustomer(const MoveCustomer &other);

        MoveCustomer(int src, int dst, int customerId);
    void act(Restaurant &restaurant) override;
    std::string toString() const override;
private:
    const int srcTable;
    const int dstTable;
    const int id;
};


class Close : public BaseAction {
public:
	virtual ~Close() ; //destructor

	BaseAction* getActionInstance() override;

	explicit Close(int id);
    void act(Restaurant &restaurant) override;
    std::string toString() const override;
private:
    const int tableId;
};


class CloseAll : public BaseAction {
public:
	BaseAction* getActionInstance() override;
	virtual ~CloseAll() ; //destructor

	CloseAll();
    void act(Restaurant &restaurant) override;
    std::string toString() const override;
private:
};


class PrintMenu : public BaseAction {
public:
	virtual ~PrintMenu() ; //destructor

	BaseAction* getActionInstance() override;

	PrintMenu();
    void act(Restaurant &restaurant) override;
    std::string toString() const override;
private:
};


class PrintTableStatus : public BaseAction {
public:
	BaseAction* getActionInstance() override;
	virtual ~PrintTableStatus() ; //destructor

	explicit PrintTableStatus(int id);
    void act(Restaurant &restaurant) override;
    std::string toString() const override;
private:
    const int tableId;
    int nameToPrice(std::string name, Restaurant &restaurant); // returns the price of the dish
};


class PrintActionsLog : public BaseAction {
public:
	virtual ~PrintActionsLog() ; //destructor

	BaseAction* getActionInstance() override;

	PrintActionsLog();
    void act(Restaurant &restaurant) override;
    std::string toString() const override;
private:
};


class BackupRestaurant : public BaseAction {
public:
	virtual ~BackupRestaurant() ; //destructor

	BaseAction* getActionInstance() override;

	BackupRestaurant();
    void act(Restaurant &restaurant) override;
    std::string toString() const override;
private:
};


class RestoreResturant : public BaseAction {
public:
	virtual ~RestoreResturant() ; //destructor

	BaseAction* getActionInstance() override;

	RestoreResturant();
    void act(Restaurant &restaurant) override;
    std::string toString() const override;

};


#endif
