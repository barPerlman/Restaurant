#include <utility>

#include "../include/Restaurant.h"
#include "../include/Table.h"//
// The following classes are implemented in this file:
//Restaurant,Table,Dish
//
#include "../include/Restaurant.h"
#include <iostream>
#include "../include/Dish.h"
#include "../include/Customer.h"
#include <string>
#include <fstream>
using namespace std;


/////////////////////////Dish Class functions/////////////////////////////////////

//constructor
Dish::Dish(int d_id, std::string d_name, int d_price, DishType d_type):id(d_id),name(std::move(d_name)),price(d_price),type(d_type) {}

//getter for dish id
int Dish::getId() const{
    return id;
}

//getter for the dish name
std::string Dish::getName()const {
    return name;
}

//getter for the dish price
int Dish::getPrice()const{
    return price;
}
//getter for the dish type
DishType Dish::getType()const{
    return type;
}

//Dish print in the required format
string Dish::toString() const {
    //convert to string in aim to return concat string
    string typeStr;
    switch(type){
        case VEG:{
            typeStr="VEG";
            break;}
        case SPC:{
            typeStr="SPC";
            break;}
        case BVG:{
            typeStr="BVG";
            break;}
        case ALC:{
            typeStr="ALC";
            break;}
    }
    string priceStr=to_string(price);
    string dishString=name+" "+typeStr+" "+priceStr+"NIS";
    return dishString;

}

