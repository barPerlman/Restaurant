#include "Customer.h"
#include <vector>
#include <string>

/**
* A Customer is an abstract class for the different customers classes.
* Each customer that arrives to the restaurant will get a number (id) that will
 * serve as an identifier as long as he is seating in the restaurant.
* Each customer has is own name.
*/

/*******************************************************
* Customer Implementation
*******************************************************/

//Constructor
Customer::Customer(std::string c_name, int c_id):name(c_name), id(c_id){}

std::string Customer::getName() const {return name;}

int Customer::getId() const {return id;}

std::string Customer::castDishType(DishType dishType) {
    std::string typeStr;
    switch(dishType){
        case VEG:
            typeStr="VEG";
        case SPC:
            typeStr="SPC";
        case BVG:
            typeStr="BVG";
        case ALC:
            typeStr="ALC";
    }
    return typeStr;
}

//checkkkkkk
bool Customer::checkMenu(const std::vector<Dish> &menu,std::string str){
    for(Dish dish:menu)
        if(castDishType(dish.getType()).compare(str) == 0)
            return true;
    return false;
}

// Destructor
Customer::~Customer() {

}

/**
* A VegetarianCustomer is one type of a Customer in the restaurant.
* This type of customer always orders the vegetarian dish with the
smallest id in the menu, and the most expensive beverage
*/

/*******************************************************
* VegetarianCustomer Implementation
*******************************************************/

//Constructor
VegetarianCustomer::VegetarianCustomer(std::string name, int id): Customer(name,id) {}

std::vector<int> VegetarianCustomer::order(const std::vector<Dish> &menu){
    // choosing the vegetarian dish with the smallest id in the menu
    int minId=static_cast<int>(menu.size());
    for(Dish dish:menu)
        if (castDishType(dish.getType()).compare("VEG") == 0)
            if (dish.getId() <= minId)
                minId = dish.getId();
    // choosing the most expensive beverage in the menu
    int maxPrice(0);
    int secId(0);
    for(Dish dish:menu)
        if(castDishType(dish.getType()).compare("BVG")==0)
            if(dish.getPrice()>=maxPrice)
                if (dish.getPrice() != maxPrice) {
                    maxPrice = dish.getPrice();
                    secId = dish.getId();
                }
    std::vector<int> vec (minId,secId);
    return vec;
}

std::string VegetarianCustomer:: toString() const {
    return this->getName();
}

/**
* A SpicyCustomer is one type of a Customer in the restaurant.
* This is a customer that orders the most expensive spicy dish in the menu.
 * He picks the cheapest non-alcoholic beverage in the menu.
 *
*/

/*******************************************************
* SpicyCustomer Implementation
*******************************************************/

//Constructor
SpicyCustomer::SpicyCustomer(std::string name, int id): Customer(name,id) {}

std::vector<int> SpicyCustomer::order(const std::vector<Dish> &menu) {
    // choosing the most expensive spicy dish in the menu
    int maxPrice(0);
    int firstId(0);
    for (Dish dish:menu)
        if (castDishType(dish.getType()).compare("VEG") == 0)
            if (dish.getPrice() >= maxPrice) {
                if (dish.getPrice() != maxPrice) {
                    maxPrice = dish.getPrice();
                    firstId = dish.getId();
                }
            }
    // choosing the cheapest non-alcoholic beverage in the menu
    int j(0);
    int minPrice;
    int secId(0);
    for (Dish dish:menu)
        if (castDishType(dish.getType()).compare("BVG")) {
            if (j == 0) {
                minPrice = dish.getPrice();
                secId = dish.getId();
                j++;
            }
            if (dish.getPrice() <= minPrice) {
                if (dish.getPrice() != maxPrice) {
                    minPrice = dish.getPrice();
                    secId = dish.getId();
                }
            }
            std::vector<int> vec(firstId, secId);

            return vec;
        }
}
std::string SpicyCustomer::toString() const {
    return this->getName();
}

/**
* A CheapCustomer –  is one type of a Customer in the restaurant.
* This is a customer that always orders the cheapest dish in the menu.
 * This customer orders only once.
*/

/*******************************************************
* CheapCustomer Implementation
*******************************************************/

//Constructor
CheapCustomer::CheapCustomer(std::string name, int id): Customer(name,id) {}

std::vector<int> CheapCustomer::order(const std::vector<Dish> &menu){
    // choosing the cheapest dish in the menu	.
    int j(0);
    int minPrice=menu[0].getPrice();
    int firstId=menu[0].getId();
    for(Dish dish:menu)
        if(dish.getPrice()<=minPrice)
            if (dish.getPrice() != minPrice) {
                minPrice = dish.getPrice();
                firstId = dish.getId();
            }
    std::vector<int> vec (firstId);

    return vec;
}

std::string CheapCustomer:: toString() const {
    return this->getName();
}

/**
* A AlchoholicCustomer –  is one type of a Customer in the restaurant.
* – This is a customer who only orders alcoholic beverages. He starts with
ordering the cheapest one, and in each further order, he picks the next expensive alcoholic beverage.
*/

/*******************************************************
* AlchoholicCustomer Implementation
*******************************************************/

//Constructor
AlchoholicCustomer::AlchoholicCustomer(std::string name, int id): Customer(name,id) {}

std::vector<int> AlchoholicCustomer::order(const std::vector<Dish> &menu) {
    // choosing the alcoholic beverages in the menu
    int flag(0);
    std::vector<int> vec;
    for (Dish dish:menu)
        if (castDishType(dish.getType()).compare("ALC")) {
            vec.push_back(dish.getId());
            //לבדוק עם בר על המיון גם פה מהקטן לגדול לפי מחיר מנה
        }
        else flag++;

}

std::string AlchoholicCustomer:: toString() const {
    return this->getName();
}














