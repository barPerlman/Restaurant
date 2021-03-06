#ifndef CUSTOMER_H_
#define CUSTOMER_H_

#include <vector>
#include <string>
#include "Dish.h"
class Customer{
public:
	virtual Customer* getCustomerInstance()=0;	//returns a pointer to a copy of the actual customer
	Customer(Customer &other); // copyConstructor
	bool findInMenu(std::string type,std::vector<Dish> menu);

	Customer(std::string c_name, int c_id);
    virtual std::vector<int> order(const std::vector<Dish> &menu)=0;
    virtual std::string toString() const = 0;
    std::string getName() const;
    int getId() const;
    std::string castDishType(DishType dishType);
    virtual ~Customer(); // Destructor
private:
    const std::string name;
    const int id;
};


class VegetarianCustomer : public Customer {
public:
    VegetarianCustomer* getCustomerInstance();	//returns a pointer to a copy of the actual customer
    VegetarianCustomer(VegetarianCustomer &other); // copyConstructor

	std::string findType();
	VegetarianCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
	virtual ~VegetarianCustomer();
private:
};


class CheapCustomer : public Customer {
public:
    CheapCustomer* getCustomerInstance();	//returns a pointer to a copy of the actual customer
    CheapCustomer(CheapCustomer &other); // copyConstructor

	std::string findType();

	CheapCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
	virtual ~CheapCustomer();

private:
	bool flag; // check if the customer already ordered before
};


class SpicyCustomer : public Customer {
public:
    SpicyCustomer* getCustomerInstance();	//returns a pointer to a copy of the actual customer
    SpicyCustomer(SpicyCustomer &other); // copyConstructor

	std::string findType();

	SpicyCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
	virtual ~SpicyCustomer();

private:
	bool flag;// checks if this is the first time he order something
};


class AlchoholicCustomer : public Customer {
public:
    AlchoholicCustomer* getCustomerInstance();	//returns a pointer to a copy of the actual customer
    AlchoholicCustomer(AlchoholicCustomer &other); // copyConstructor

	std::string findType();

	AlchoholicCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
	virtual ~AlchoholicCustomer();

private:
	int numOfOrders;// count the number of times he ordered
	std::vector<Dish> sortAlcDish(const std::vector<Dish> &menu);// sort the alc dishes from the menu
	std::vector<int> exportParameter(std::vector<Dish> alcMenu,int para);// returns a vector with : prices if para=0,indexes if para=1
};

#endif
