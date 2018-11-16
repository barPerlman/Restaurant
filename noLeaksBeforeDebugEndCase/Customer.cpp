
#include "Customer.h"

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
Customer::Customer(std::string c_name, int c_id):name(std::move(c_name)), id(c_id){}

std::string Customer::getName() const {return name;}

int Customer::getId() const {return id;}

std::string Customer::castDishType(DishType dishType) {
    std::string typeStr;
    if(dishType==ALC)
        typeStr="ALC";
    else if(dishType==SPC)
        typeStr="SPC";
    else if(dishType==BVG)
        typeStr="BVG";
    else typeStr="VEG";
    return typeStr;
}

// search if this type of dish exists in the menu
bool Customer::findInMenu(std::string type,std::vector<Dish> menu){
    for (Dish &dish:menu)
        if(castDishType(dish.getType())==type)
            return true;
    return false;
}



// Destructor
Customer::~Customer(){};


//copy constructor
Customer::Customer(Customer &other):name(other.name),id(other.id){}

/**
* A VegetarianCustomer is one type of a Customer in the restaurant.
* This type of customer always orders the vegetarian dish with the
smallest id in the menu, and the most expensive beverage
*/

/*******************************************************
* VegetarianCustomer Implementation
*******************************************************/

//Constructor
VegetarianCustomer::VegetarianCustomer(std::string name, int id): Customer(std::move(name),id) {}

VegetarianCustomer::~VegetarianCustomer() {}    //destructor

//copy constructor
VegetarianCustomer::VegetarianCustomer(VegetarianCustomer &other):Customer(other){}

std::vector<int> VegetarianCustomer::order(const std::vector<Dish> &menu){
    // choosing the vegetarian dish with the smallest id in the menu
    std::vector<int> vec;
    if(findInMenu("VEG",menu) && findInMenu("BVG",menu)) {
        int dishId(0);
        for (const Dish &dish:menu) {
            if (castDishType(dish.getType())=="VEG") {
                dishId = dish.getId();
                break;
            }
        }
        vec.push_back(dishId);
        // choosing the most expensive beverage in the menu
        int maxPrice(0);
        int bvgId(0);
        for (const Dish &dish:menu) {
            if (castDishType(dish.getType()) == "BVG")
                if (dish.getPrice() >= maxPrice)
                    if (dish.getPrice() != maxPrice) {
                        maxPrice = dish.getPrice();
                        bvgId = dish.getId();
                    }
        }
        vec.push_back(bvgId);
    }
    return vec;
}

std::string VegetarianCustomer:: toString() const {
    return this->getName()+",veg";
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
SpicyCustomer::SpicyCustomer(std::string name, int id): Customer(std::move(name),id), flag(true) {}

SpicyCustomer::~SpicyCustomer() {}    //destructor

//copy constructor
SpicyCustomer::SpicyCustomer(SpicyCustomer &other):Customer(other),flag(other.flag) {}

std::vector<int> SpicyCustomer::order(const std::vector<Dish> &menu) {
    // choosing the most expensive spicy dish in the menu
    std::vector<int> vec;
    if (findInMenu("SPC", menu) && findInMenu("BVG", menu)) {
        // if this is the first order
        if (flag) {
            int maxPrice(0);
            int dishId(0);
            for (const Dish &dish:menu) {
                if (castDishType(dish.getType()) == "SPC")
                    if (dish.getPrice() >= maxPrice)
                        if (dish.getPrice() != maxPrice) {
                            maxPrice = dish.getPrice();
                            dishId = dish.getId();
                        }
            }
            flag = false;
            vec.push_back(dishId);
        }
        // choosing the cheapest non-alcoholic beverage in the menu
        else {
            int minPrice = menu[0].getPrice();
            int bvgId(0);
            for (const Dish &dish:menu) {
                if (castDishType(dish.getType()) == "BVG")
                    if (dish.getPrice() <= minPrice)
                        if (dish.getPrice() != minPrice) {
                            minPrice = dish.getPrice();
                            bvgId = dish.getId();
                        }
            }
            vec.push_back(bvgId);
        }

    }
    return vec;
}
std::string SpicyCustomer::toString() const {
    return this->getName()+",spc";
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
CheapCustomer::CheapCustomer(std::string name, int id): Customer(std::move(name),id), flag(true) {}
CheapCustomer::~CheapCustomer() {}    //destructor

//copy constructor
CheapCustomer::CheapCustomer(CheapCustomer &other):Customer(other),flag(other.flag){}


std::vector<int> CheapCustomer::order(const std::vector<Dish> &menu){
    // choosing the cheapest dish in the menu	.
    std::vector<int> vec;
    if(flag) {
        int minPrice = menu[0].getPrice();
        int dishId(0);
        for (const Dish &dish:menu) {
            if (dish.getPrice() <= minPrice)
                if (dish.getPrice() != minPrice) {
                    minPrice = dish.getPrice();
                    dishId = dish.getId();
                }
        }
        vec.push_back(dishId);
        flag=false;
    }
    return vec;
}

std::string CheapCustomer:: toString() const {
    return this->getName()+",chp";
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
AlchoholicCustomer::AlchoholicCustomer(std::string name, int id): Customer(std::move(name),id),numOfOrders(-1) {}
AlchoholicCustomer::~AlchoholicCustomer() {}        //destructor
//copy constructor
AlchoholicCustomer::AlchoholicCustomer(AlchoholicCustomer &other):Customer(other),numOfOrders(other.numOfOrders) {}

std::vector<int> AlchoholicCustomer::order(const std::vector<Dish> &menu) {
    // choosing the alcoholic beverages in the menu
    std::vector<int> vec;
    if (findInMenu("ALC", menu)) {
        int tempPrice(0), tempId(0);
        std::vector<Dish> alcMenu = sortAlcDish(menu);
        std::vector<int> prices = exportParameter(alcMenu, 0);
        std::vector<int> ids = exportParameter(alcMenu, 1);
        for (int i = 0; i < static_cast<int>(alcMenu.size()) - 1; i++) // according bubble sort
            for (int j = 0; j < static_cast<int>(alcMenu.size()) - i - 1; j++)// Last i elements are already in place
                if (prices[j] > prices[j + 1]) {
                    tempPrice = prices[j];
                    tempId = ids[j];
                    prices[j] = prices[j + 1];
                    ids[j] = ids[j + 1];
                    prices[j + 1] = tempPrice;
                    ids[j + 1] = tempId;
                }


        numOfOrders++;
        if (numOfOrders < static_cast<int>(ids.size())) {
            vec.push_back(ids[numOfOrders]);
        }
    }
    return vec;
}

// returns a vector which contains the alchoholic dishes in it
std::vector<Dish> AlchoholicCustomer::sortAlcDish(const std::vector<Dish> &menu) {
    std::vector<Dish> vec;
    std::string s="chs";
    for(const Dish &dish:menu)
        if (castDishType(dish.getType())=="ALC")
            vec.push_back(dish);
    return vec;
}

// returns a vector which contains the alchoholic dishes in it
std::vector<int> AlchoholicCustomer::exportParameter(std::vector<Dish> alcMenu,int para) {
    std::vector<int> vec;
    if(para==0)
        for(const Dish &dish:alcMenu)
            vec.push_back(dish.getPrice());
    else
        for(const Dish &dish:alcMenu)
            vec.push_back(dish.getId());
    return vec;
}

std::string AlchoholicCustomer:: toString() const {
    return this->getName()+",alc";
}

//the following functions returns a pointer to a copy of the actual customer
AlchoholicCustomer* AlchoholicCustomer::getCustomerInstance()	{
    //Customer* copyCustomer=
    return new AlchoholicCustomer(*this);
}

VegetarianCustomer* VegetarianCustomer::getCustomerInstance()	{
    //Customer* copyCustomer=
    return new VegetarianCustomer(*this);
}

CheapCustomer* CheapCustomer::getCustomerInstance()	{
    //Customer* copyCustomer=
    return new CheapCustomer(*this);
}

SpicyCustomer* SpicyCustomer::getCustomerInstance(){
    //Customer* copyCustomer=
    return new SpicyCustomer(*this);
}


