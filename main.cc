#include <iostream>
#include <string>
#include "control.h"
using namespace std;

int main(int argc, char *argv[]) {
    bool testing = false;
    if (argc > 1) {
        string arg1 = argv[1];
        if (arg1 == "-testing") {
            testing = true; 
            std::cout << "\nYou are currently in testing phase!\n" << endl;
        }
    }

    int numOfPlayers;
    while (true) {
		std::cout << "How many players?" << endl;
		cin >> numOfPlayers;
		if (numOfPlayers <= 1) {
            std::cout << "Please enter a positive number of players. There must be at least 2 players!" << endl;
		} else {
            break;
        }
    }

    Control gameControl(numOfPlayers, testing);
    gameControl.distributeCards();
    
    bool reserveUsed;
    int numOfActiveHeads;
    int numOfHeads;
    bool endTurn = false;
    bool winner = false;
    while (!winner) {
        numOfActiveHeads = gameControl.getNumOfActiveHeads();
        numOfHeads = gameControl.getNumOfHeads();
        reserveUsed = false;
        for (int heads = 1; heads <= numOfActiveHeads; heads++) {
            gameControl.printInfoStatus();
            shared_ptr<Card> playerTopCard = gameControl.getPlayerTopCard();
            
            if (testing) {
                if (playerTopCard->getSuit() == "J") {
                    cout << "\nPlayer " << gameControl.getPlayerInt() << ", you are holding a Joker. Your move?" << endl;
                } else {
                    cout << "\nPlayer " << gameControl.getPlayerInt() << ", you are holding a " << playerTopCard->getSym() << playerTopCard->getSuit() << ". Your move?" << endl;
                }
            }

            int move;
            //getting current player's move, current player is determined in control.cc
            while (true) {
                if (!(cin >> move)) {
                    if (cin.eof()) {
                        exit(0);
                    } else {
                        cin.clear();
                        cin.ignore();
                        std::cout << "Please enter a valid integer!" << endl;
                        continue;
                    }
                }

                //if the move is to ADD to reserve check if it is a valid move
                if (move == 0) {
                    if (gameControl.getNumOfActiveHeads() > 1 && reserveUsed == false) {
                        reserveUsed = true;
                        gameControl.addToReserve();
                        playerTopCard = gameControl.getPlayerTopCard();
                    //if move is to SWAP with reserve check if valid
                    } else if (gameControl.getNumOfActiveHeads() > 1 && reserveUsed == true) {
                        gameControl.swapWithReserve();
                        playerTopCard = gameControl.getPlayerTopCard();
                    } else {
                        std::cout << "Invalid input! You cannot use your reserve just yet, please try again!" << endl;
                    }
                //if placing on a head make sure it is a valid move
                } else if (move > 0) {
                    int lowestHead = gameControl.getLowestHead();
                    if (move > numOfHeads || move < lowestHead || move < 0) {
                        std::cout << "Invalid input, you're way out of range! please try again!" << endl;
                    } else {
                        if (playerTopCard->getSuit() == "J") {
                            std::cout << "Joker value?" << endl;
                            string jokerVal;
                            while (true) {
                                if (!(cin >> jokerVal)) {
                                    if (cin.eof()) {
                                        exit(0);
                                    } else {
                                        cin.clear();
                                        cin.ignore();
                                        std::cout << "Please enter a valid value!" << endl;
                                        continue;
                                    }
                                }
                                playerTopCard = gameControl.setJokerCard(playerTopCard, jokerVal);
                                break;
                            }
                        }

                        //the function will then decide whether or not to cut the head and end turn, continue
                        //or end turn bc of same val
                        endTurn = gameControl.checkHeadMoveToEndTurn(move, playerTopCard);       
                        break;
                    } //break outmost loop after getting player move and player played the card
                } 
            }//while loop: get player move
            if (gameControl.getPlayer()->drawDeckSize() + gameControl.getPlayer()->discardDeckSize() == 0 && 
                gameControl.getPlayer()->isReserveEmpty() == 0) {
                winner = true;
                break;
            } else if (endTurn) {
                break;
            }
        } //break for loop to end turn early
        gameControl.emptyReserve();
        if (gameControl.getPlayer()->drawDeckSize() + gameControl.getPlayer()->discardDeckSize() == 0 && 
            gameControl.getPlayer()->isReserveEmpty() == 0) {
                winner = true;
        } else {
            gameControl.switchPlayers();
        }
    }

    std::cout << "Player " << gameControl.getPlayerInt() << " wins!" << endl;
    
    return 0;
}
