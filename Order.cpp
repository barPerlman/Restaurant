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

Order::Order(int id):BaseAction(),tableId(id){}
void Order::act(Restaurant &restaurant){}
std::string Order::toString() const{}