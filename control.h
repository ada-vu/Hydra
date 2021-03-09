#ifndef CONTROL_H
#define CONTROL_H
#include <iostream>
#include <vector>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include "player.h"
#include "card.h"
#include "head.h"

class Player;
class Card;
class Head;

class Control {
    private:
    std::map<int, std::shared_ptr<Player>> players;
    std::vector<std::shared_ptr<Card>> deckOfAllCards;
    std::map<int, std::shared_ptr<Head>> heads;
    bool testing;
    bool startGame;
    int numOfPlayers;
    int curPlayer;      
    int numOfHeads;
    int numOfActiveHeads;
    int remainingDecrementor;

    public:
    Control(int numOfPlayers, bool testing);
    ~Control();

    //testing mode
    std::string testingModeCard(bool createHead = false);
    std::string testingModeSuit(std::string cardVal, bool createHead = false);

    //initial setup phase
    void createDecks();
    void distributeCards();

    //player actions
    std::shared_ptr<Player> getPlayer();
    int getPlayerInt();
    std::shared_ptr<Card> getPlayerTopCard(bool createHead = false);
    void switchPlayers();
    std::shared_ptr<Card> setJokerCard(std::shared_ptr<Card> card, std::string val);

    //head actions
    int getNumOfHeads();
    int getNumOfActiveHeads();
    int getLowestHead();
    bool checkHeadMoveToEndTurn(int move, std::shared_ptr<Card> playerCardIntVal);
    bool playableMoves(int playerTopCardIntVal);
    int getPlayerMove();
    void createNewHeads();
    
    //reserve actions
    void addToReserve();
    void swapWithReserve();
    void emptyReserve();

    //printing functions
    void printInfoStatus();
    void printPlayerStatus();        

};

#endif
