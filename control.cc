#include "control.h"
using namespace std;

Control::Control(int numOfPlayers, bool testing) : testing{testing}, startGame{true}, numOfPlayers{numOfPlayers}, curPlayer{1}, numOfHeads{1}, numOfActiveHeads{1}, remainingDecrementor{numOfActiveHeads} {
    //make players
    for (int p = 1; p <= numOfPlayers; p++) {
		players[p] = make_shared<Player>();
	}
}

Control::~Control() {}

std::string Control::testingModeCard(bool createHead) {
    if (createHead) {
        cout << "\nCard value for new head? Player " << curPlayer << endl;
    } else {
        cout << "\nCard value? Player " << curPlayer << endl;
    }
    string cardVal;
    
    while (true) {
        if (!(cin >> cardVal)) {
            if (cin.eof()) {
                exit(0);
            } else {
                cin.clear();
                cin.ignore();
                std::cout << "Please enter a valid value!" << endl;
                continue;
            }
        } 
        if (cardVal == "A" || cardVal == "J" || cardVal == "Q" || cardVal == "K" || cardVal == "Joker" ||
            cardVal == "2" || cardVal == "3" || cardVal == "4" || cardVal == "5" || cardVal == "6" || cardVal == "7" || cardVal == "8" || cardVal == "9" || cardVal == "10") {
            break;
        } else {
            cout << "Invalid card value. Please select a valid card value from A, J, Q, K, 2-10 or Joker" << endl;
            continue;
        } 
    }
    if (cardVal == "A") {
        return "1";
    } else if (cardVal == "J") {
        return "11";
    } else if (cardVal == "Q") {
        return "12";
    } else if (cardVal == "K") {
        return "13";
    } else {
        return cardVal;
    }       
}

std::string Control::testingModeSuit(string cardVal, bool createHead) {
    string suit;   

    if (cardVal != "Joker") {
        if (createHead) {
            cout << "\nSuit value for new head? Player " << curPlayer << endl;
        } else {
            cout << "\nSuit value? Player " << curPlayer << endl;
        }
        
        while (true) {
            if (!(cin >> suit)) {
                if (cin.eof()) {
                    exit(0);
                } else {
                    cin.clear();
                    cin.ignore();
                    std::cout << "Please enter a valid value!" << endl;
                    continue;
                }
            } 
            if (suit == "S" || suit == "H" || suit == "C" || suit == "D") {
                break;
            } else {
                cout << "Invalid suit. Please select a valid suit from S, H, C, or D" << endl;
                continue;
            }
        }
    }
    return suit;
}

void Control::createDecks() {
    string suits[] = {"H", "D", "S", "C"};
    for (int p = 0; p < numOfPlayers; p++) {
        for(int s = 0; s < 4; s++) {
            for (int val = 1; val <= 13; val++) {
                deckOfAllCards.emplace_back(make_shared<Card>(val, suits[s]));
            } //values of cards from 1 - 13 (1:A and 11:J, 12:Q, 13:K)
        } //each card has a suit, this makes 52 cards
        for (int j = 0; j < 2; j++) {
            deckOfAllCards.emplace_back(make_shared<Card>(2, "J"));
        } //make 2 jokers for each deck (unassigned)
    } //there are as many decks as there are players
}

void Control::distributeCards() {
    createDecks();
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    shuffle (deckOfAllCards.begin(), deckOfAllCards.end(), std::default_random_engine(seed));

    //until deckOfAllCards is empty, distribute cards and each player should have 54 in their drawDeck
    while(!deckOfAllCards.empty()) {        
        for (int p = 1; p <= numOfPlayers; p++) {
            players[p]->addToDraw(deckOfAllCards.back());
            deckOfAllCards.pop_back();
        }
    }

    //make first head
    heads[numOfHeads] = make_shared<Head>();
    //add the top card off of player 1's drawDeck into the first head created
    heads[numOfHeads]->addToHeadPile(getPlayerTopCard());
    players[curPlayer]->playCard();
}

std::shared_ptr<Player> Control::getPlayer() {
    return players[curPlayer];
}

int Control::getPlayerInt() {
    return curPlayer;
}

std::shared_ptr<Card> Control::getPlayerTopCard(bool createHead) {
    if (!testing) {
        auto next = make_shared<Card>(0, "");
        next = players[curPlayer]->drawNext();
        return next;
    } else {
        string cardVal = testingModeCard(createHead);
        string suit = testingModeSuit(cardVal, createHead);
        if (cardVal.compare("Joker") == 0) {
            cardVal = "2";
            suit = "J";
        }
        auto next = make_shared<Card>(stoi(cardVal), suit);
        return next;
    }
}

void Control::switchPlayers() {
    curPlayer++;
    if (curPlayer > numOfPlayers) {
        curPlayer = 1;
    }
    remainingDecrementor = numOfActiveHeads;
}

std::shared_ptr<Card> Control::setJokerCard(std::shared_ptr<Card> card, string val) {
    return players[curPlayer]->setJokerCard(card, val);
}

int Control::getNumOfHeads() {
    return numOfHeads;
}

int Control::getNumOfActiveHeads() {
    return numOfActiveHeads;
}

int Control::getLowestHead() {
    return heads.begin()->first;
}

bool Control::checkHeadMoveToEndTurn(int whichHead, std::shared_ptr<Card> playerTopCard) {
    shared_ptr<Card> headTopCard = heads[whichHead]->getTopCard();
    //if the head and player card are the same
    if (headTopCard->getVal() == playerTopCard->getVal()) {
        heads[whichHead]->addToHeadPile(playerTopCard);
        players[curPlayer]->playCard();
        if (players[curPlayer]->getReserveCard() != nullptr) {
            players[curPlayer]->addReserveBackToDraw();
        }
        return true;
    //if the head is less than player card, must cut the oldest head
    } else if (headTopCard->getVal() < playerTopCard->getVal() && headTopCard->getVal() != 1) {
        //check if there are playable moves before cutting
        if (playableMoves(playerTopCard->getVal())) {
            cout << "With the card you are holding, you do not have to cut a head. You have playable moves! Your move?" << endl;
            int move = getPlayerMove();
            while (move == 0) {
                cout << "You don't really have to use your reserve, so don't use it! You have playable moves! Your move?" << endl;
                move = getPlayerMove();
            } 
            while (move > numOfHeads || move < getLowestHead() || move < 0) {
                cout << "Invalid input! You're way out of range! Please try again." << endl;
                move = getPlayerMove();
            }
            checkHeadMoveToEndTurn(move, playerTopCard);
            return false;
        } else {
            //check to see that player is cutting oldest head, otherwise keep prompting
            while (whichHead != getLowestHead()) {
                cout << "You must cut the oldest head!" << endl;
                whichHead = getPlayerMove();
            } 
            heads[whichHead]->cutHeads(players[curPlayer]);
            heads.erase(whichHead);
            createNewHeads();
            return true;
        }
    //otherwise continue if head is greater than player card
    } else {
        heads[whichHead]->addToHeadPile(playerTopCard);
        players[curPlayer]->playCard();
        return false;
    }
}

int Control::getPlayerMove() {
    int move;
    while(true) {
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
        break;
    }
    return move;
}

bool Control::playableMoves(int playerTopCardIntVal) {
    for (auto it = heads.begin(); it != heads.end(); it++) {
        if (it->second->getTopCardVal() >= playerTopCardIntVal) {
            return true;
        } else if (it->second->getTopCardVal() == 1) {
            return true;
        }
    }
    return false;
}

void Control::createNewHeads() {
    if (players[curPlayer]->drawDeckSize() == 0 && players[curPlayer]->discardDeckSize() > 0) {
        players[curPlayer]->shuffleDiscardIntoDraw();
    }
    for (int i = 1; i <= 2; i++) {
        heads[numOfHeads+i] = make_shared<Head>();
        heads[numOfHeads+i]->addToHeadPile(getPlayerTopCard(true));
        players[curPlayer]->playCard();
    }
    numOfHeads += 2;
    numOfActiveHeads += 1;
}

void Control::addToReserve() {
    //get the next card to put into reserve
    auto next = make_shared<Card>(0, "");
    next = getPlayerTopCard();
    if (players[curPlayer]->drawDeckSize() == 1 && players[curPlayer]->discardDeckSize() == 0) {
        throw invalid_argument("You can't use the reserve with only 1 draw card left.");
    } else if (players[curPlayer]->drawDeckSize() > 1) {
        players[curPlayer]->addToReserve(next);             //addToReserve will pop_back of drawDeck
    }
    //get the next card after
    next = getPlayerTopCard();
    if (next->getSuit() == "J") {
        cout << "Player " << curPlayer << ", you used your reserve. You are now holding a Joker. Your move?" << endl;
    } else {
        cout << "Player " << curPlayer << ", you used your reserve. You are now holding a " << next->getSym() << next->getSuit() << ". Your move?" << endl;
    }
}

void Control::swapWithReserve() {
    //get the next card to swap with reserve
    auto next = make_shared<Card>(0, "");
    next = getPlayerTopCard();
    players[curPlayer]->swapWithReserve(next);
    //get the card that was just swapped and placed back on top of draw
    next = getPlayerTopCard();
    if (next->getSuit() == "J") {
        cout << "Player " << curPlayer << ", you used your reserve. You are now holding a Joker. Your move?" << endl;
    } else {
        cout << "Player " << curPlayer << ", you swapped with your reserve. You are now holding a " << next->getSym() << next->getSuit() << ". Your move?" << endl;
    }
}

void Control::emptyReserve() {
    players[curPlayer]->emptyReserve();
}

void Control::printInfoStatus() {
    cout << "\nHeads: " << endl;
    for (auto it = heads.begin(); it != heads.end(); it++) {
        cout << it->first << ": " << it->second->topCard() << "(" << it->second->headPileSize() << ")" << endl;
    }
    cout << "\nPlayers: " << endl;
    for (auto it = players.begin(); it != players.end(); it++) {
        cout << "Player " << it->first << ": " << it->second->drawDeckSize()+it->second->discardDeckSize() << " (" << it->second->drawDeckSize() << " draw, " << it->second->discardDeckSize() << " discard)" << endl;
    }
    if (startGame) {
        curPlayer++;
        startGame = false;
    }
    cout << "\nPlayer " << curPlayer << ", it is your turn" << endl;
    
    printPlayerStatus();
}

void Control::printPlayerStatus() {
    remainingDecrementor -= 1;
    
    cout << "\nHeads: " << endl;
    for (auto it = heads.begin(); it != heads.end(); it++) {
        cout << it->first << ": " << it->second->topCard() << "(" << it->second->headPileSize() << ")" << endl;
    }
    cout << "\nPlayers: " << endl;
    for (auto it = players.begin(); it != players.end(); it++) {
        if (it->first == curPlayer) {
            cout << "Player " << it->first << ": " << it->second->drawDeckSize()+it->second->discardDeckSize() << " (" << it->second->drawDeckSize()-1 << " draw, " << it->second->discardDeckSize() << " discard)"
            << " + 1 in hand, " << remainingDecrementor << " remaining, " << it->second->isReserveEmpty() << " in reserve" << endl;
        } else {        
            cout << "Player " << it->first << ": " << it->second->drawDeckSize()+it->second->discardDeckSize() << " (" << it->second->drawDeckSize() << " draw, " << it->second->discardDeckSize() << " discard)" << endl;
        }
    }
    if (!testing) {
        auto next = make_shared<Card>(0, "");
        next = getPlayerTopCard();
        if (next->getSuit() == "J") {
            cout << "\nPlayer " << curPlayer << ", you are holding a Joker. Your move?" << endl;
        } else {
            cout << "\nPlayer " << curPlayer << ", you are holding a " << next->getSym() << next->getSuit() << ". Your move?" << endl;
        }
    }   
}
