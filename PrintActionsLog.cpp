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
PrintActionsLog::PrintActionsLog():BaseAction(){}
//prints all of the to strings of the actions were performed so far in the restaurant
void PrintActionsLog::act(Restaurant &restaurant){
    for(BaseAction* action:restaurant.getActionsLog()){
        std::cout<<action->toString()<<std::endl;
    }
}
//log action is not displayed according to the specifications document
std::string PrintActionsLog::toString() const{}
