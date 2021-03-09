#ifndef CARD_H
#define CARD_H
#include <iostream>
#include <vector>
#include <memory>
#include <string>

class Card {
    private:
    int val;
    //these are the variables used to display status, the int val will always be converted to the string sym universally
    std::string suit;
    std::string sym;            //int values will be converted to string and stored as sym
    bool isJokerAssigned;

    public:
    Card(int val, std::string suit);        //constructor
    ~Card();                                //destructor

    //normal card functions
    int getVal();
    std::string getSuit();
    std::string getSym();
    void setVal(int setTo);
    void setSuit(std::string setTo);
    void setSym(std::string setTo);

    //joker card actions
    bool isAssignedJokerCard();
    void assignJoker();
    void unassignJoker();
};

#endif
