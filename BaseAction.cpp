//
// Created by barper on 11/7/18.
//

#include "Action.h"
#include "Restaurant.h"
#include <iostream>
#include "Dish.h"
#include "Customer.h"
#include <string>
#include <fstream>
#include <vector>
using namespace std;
//constructor
BaseAction::BaseAction():status(PENDING) {}

//destructor
//BaseAction::~BaseAction(){};


//getter
ActionStatus BaseAction::getStatus() const {
    return status;
}
//change status to complete
void BaseAction::complete(){
    status=COMPLETED;
}
//update error message and change status to error
void BaseAction::error(std::string errorMsg){
    status=ERROR;
    (*this).errorMsg="Error: "+errorMsg;
}
//getter
std::string BaseAction::getErrorMsg() const{
    return errorMsg;
}

//copy constructor
BaseAction::BaseAction(const BaseAction &other):errorMsg(other.errorMsg),status(other.status){}

