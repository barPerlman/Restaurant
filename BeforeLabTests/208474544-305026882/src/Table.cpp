#include "../include/Restaurant.h"
#include "../include/Dish.h"
#include "../include/Customer.h"
#include "../include/Table.h"

/////////////////////////Table Class functions/////////////////////////////////////
//constructor
Table::Table(int t_capacity) : capacity(t_capacity), open(false),customersList(),orderList() {}

//getter of table capacity
int Table::getCapacity() const {
    return capacity;
}

//add a customer to the table as last element in vector
void Table::addCustomer(Customer *customer) {
    //it's possible to add a customer to the table
    int custListSize= customersList.size();
    if (custListSize < capacity) {
        customersList.push_back(customer->getCustomerInstance());//add customer to table at end of list
    } else {
        std::cout << "Cannot add a Customer to the table. it's open or full!" << std::endl;
    }

}

//remove customer with 'id' from customers list
//remove orders of customer with 'id' from orders list
void Table::removeCustomer(int id) {
    int i = 0;
    bool found = false;   //a flag tells if the customer removed
    for(Customer * currCustomer: customersList){
        if(!found && currCustomer->getId()==id)
        {
            customersList.erase(customersList.begin()+i);
            found=true;
            delete currCustomer;
        }
        i++;
    }

   //if a customer is removed, remove his orders out of the table
    if (found) {
        vector<OrderPair> orderListCopy(orderList);  //deep copy orders to temp list orders
        orderList.clear();
        for (OrderPair op:orderListCopy) {    //get back to origin vector the orders without orders attributed to the 'id'
            if (op.first != id) {
                orderList.push_back(op);
            }
        }

    }
}

//return a customer with matching id from the list
Customer *Table::getCustomer(int id) {
    for (Customer *c:customersList) {
        if (c->getId() == id) {   //found such customer
            return c;
        }
    }
    return nullptr;
}


//return a ref. for the customers list
std::vector<Customer *> &Table::getCustomers() {
    vector<Customer *> &customersRef = customersList;
    return customersRef;
}

//return a ref. for the orders list
std::vector<OrderPair> &Table::getOrders() {
    vector<OrderPair> &orderRef = orderList;
    return orderRef;
}

//this function takes orders from the table customers
void Table::order(const std::vector<Dish> &menu) {
    //allocate a vector for the current customers orders
    vector<OrderPair> currOrdersById;   //vector of orders with first value that holds the customer id for concate with old orders list
    vector<CurrOrderPair> currOrdersByName;     //vector of orders with first value that holds the customer name for print
    for (Customer *&c:customersList) {
        //get current customer orders vector by id
        vector<int> customerOrders = c->order(menu);
        //push customer orders into current orders from table vector
        for (int dishId:customerOrders) {
            OrderPair op(c->getId(), (menu.at(dishId)));
            currOrdersById.push_back(op);
            CurrOrderPair cop(c->getName(), menu.at(dishId));
            currOrdersByName.push_back(cop);
        }
    }
    //update the orders list with the last made orders
    for (const OrderPair &lastOrder:currOrdersById) {
        orderList.push_back(lastOrder);
    }
    //print the last orders made in the table
    for (CurrOrderPair lastOrder:currOrdersByName) {
        std::cout << lastOrder.first + " ordered " + lastOrder.second.getName() << std::endl;
    }
}

//open table by change its status
void Table::openTable() {
    open = true;
}

//close table by change its status
void Table::closeTable() {
    //remove customers from table
    while (!customersList.empty()) {
        removeCustomer(customersList.at(0)->getId());
    }

    open = false;
}

//get the sum of the dishes ordered together
int Table::getBill() {
    int billSum = 0;
    for (OrderPair op:orderList) {
        billSum += op.second.getPrice();
    }
    return billSum;
}

//get the table status (open or closed)
bool Table::isOpen() {
    return open;
}

//destructor
Table::~Table() {
    clear();
}

//clear sources out of the customers vector
void Table::clear() {

   if(!customersList.empty()) {

       for (Customer *customer:customersList) {
           if(customer!=nullptr){
           delete customer;
           customer = nullptr;}
       }
       customersList.clear();
   }

}

//copy constructor
Table::Table(const Table &other):capacity(other.capacity),open(other.open),customersList(),orderList() {

    //deep copy
    //copy the customers
    for(Customer* c:other.customersList){
        customersList.push_back(c->getCustomerInstance()); //get copied instances of customers
    }
    //copy the orders list
    for(const OrderPair &op:other.orderList){
        orderList.push_back(op);
    }



}

//copy assignment
Table& Table::operator=(const Table &other)
{
    if (this != &other)
    {

        clear();  //remove old data

       //deep copy
        //copy the customers
        for(Customer* c:other.customersList){
            customersList.push_back(c->getCustomerInstance()); //get copied instances of customers
        }
        //copy the orders list
        for(const OrderPair &op:other.orderList){
            orderList.push_back(op);
        }

        open=other.open;
        capacity=other.capacity;
    }

    return *this;
}
