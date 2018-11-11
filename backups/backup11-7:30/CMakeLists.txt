cmake_minimum_required(VERSION 3.12)
project(ass1)

set(CMAKE_CXX_STANDARD 14)

add_executable(ass1 main.cpp Customer.cpp Table.cpp Dish.cpp Restaurant.cpp OpenTable.cpp BaseAction.cpp Order.cpp Action.cpp MoveCustomer.cpp PrintActionsLog.cpp)