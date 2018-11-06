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
//destructor
Table::~Table() {
    clear();
}
//clear sources out of the customers vector
void Table::clear(){
    for(Customer *customer:customersList){
        delete[] customer;
        customer= nullptr;
    }
}
