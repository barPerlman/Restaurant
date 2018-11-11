#ifndef TABLE_H_
#define TABLE_H_

#include <vector>
#include "Customer.h"
#include "Dish.h"

typedef std::pair<int, Dish> OrderPair;
typedef std::pair<std::string, Dish> CurrOrderPair;     //define new vector of pairs for orders string

class Table{
public:
    void buildCustomersPointersVector(std::string CustomerName,int customerId,std::string CustomerType,std::vector<Customer*> &customersList);
        virtual ~Table(); //destructor
    Table(int t_capacity);
    int getCapacity() const;
    void addCustomer(Customer* customer);
    void removeCustomer(int id);
    Customer* getCustomer(int id);
    std::vector<Customer*>& getCustomers();
    std::vector<OrderPair>& getOrders();
    void order(const std::vector<Dish> &menu);
    void openTable();
    void closeTable();
    int getBill();
    bool isOpen();
    Table(const Table &other);// copy constructor
    Table& operator=(const Table &other);
    Table& operator=(Table &&other);

private:
    void clear();
    int capacity;
    bool open;
    std::vector<Customer*> customersList;
    std::vector<OrderPair> orderList; //A list of pairs for each order in a table - (customer_id, Dish)

};


#endif