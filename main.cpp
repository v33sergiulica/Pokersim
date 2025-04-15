#include <iostream>
#include <algorithm>
#include <random>
#include <vector>
#include <array>
#include <ostream>

using std::cout;
using std::cin;
using std::endl;

const std::array<std::string,5> suitfy = {"Hearts", "Diamonds", "Clubs", "Spades", "all Suits"};
const std::array<std::string,14> valuefy = {"Joker", "2", "3", "4", "5", "6", "7", "8", "9", "10", "Jack", "Queen", "King", "Ace"};

class Card {
    int value;
    int suit;
public:
    int getValue() const {return value;}
    int getSuit() const {return suit;}
    Card() {value = 0; suit = 4;}
    Card(int v, int s) {value = v; suit = s;}
};

class Deck {
    Card cards[52];
public:
    void shuffle() {
        static std::random_device rd;
        static std::mt19937 g(rd());
        std::shuffle(std::begin(cards), std::end(cards), g);
        std::cout << "Shuffled" << std::endl;
    };
    Card popFCard() {
        return cards[0];
    }
    void cycleFCard() {
        int su = cards[0].getSuit();
        int va = cards[0].getValue();
        for (int i = 0; i < 51; i++) {
            cards[i] = cards[i + 1];
        }
        cards[51] = Card(va, su);
    }
    Deck() {
        for (int i = 0; i < 52; i++) {
            cards[i] = Card(i % 13 + 1, i / 13);
        }
    }
};

class Player {
    Card hand[2];
    int bestHand[4];
    int money;
public:
    explicit Player(Deck &deck) {
        hand[0] = deck.popFCard();
        deck.cycleFCard();
        hand[1] = deck.popFCard();
        deck.cycleFCard();
        money = 1000;
        bestHand[0] = 0;
    }
    void setBestHand(const int bH[]) {
        for (int i = 0; i < 4; i++) {
            bestHand[i] = bH[i];
        }
    }
    void printBestHand() {
        for (int i : bestHand) {
            std::cout << i << " ";
        }
        std::cout << std::endl;
    }
    Card getHandI(int i) {
        return hand[i];
    }
    int chooseAction() {
        std::cout << "Choose an action:0-Fold 1-Bet 2-Raise 3-Check 4-Call" << std::endl;
        int action = -1;
        std::cin >> action;
        switch (action) {
            case 1: std::cout << "Betting" << std::endl; break;
            case 2: std::cout << "Raising" << std::endl; break;
            case 3: std::cout << "Checking" << std::endl; break;
            case 4: std::cout << "Calling" << std::endl; break;
            case 0: std::cout << "Folding" << std::endl; break;
            default: std::cout << "Invalid action" << std::endl; action = -1; break;
        }
        return action;
    }
    void printHand() {
        std::cout << "First card is a " << valuefy[hand[0].getValue()] << " of " << suitfy[hand[0].getSuit()] << std::endl;
        std::cout << "Second card is a " << valuefy[hand[1].getValue()] << " of " << suitfy[hand[1].getSuit()] << std::endl;
        std::cout << std::endl;
    }
    friend std::ostream & operator<<(std::ostream &os, const Player &obj) {
        return os << "money: " << obj.money;
    }
};

class Table {
    Card tableHand[5];
    int pot;
    bool betFaze;
    bool flop;
    bool turn;
    bool river;
public:
    Card getHandI(int i) {
        return tableHand[i];
    }
    Table() {
        for (int i = 0; i < 5; i++) { tableHand[i] = Card(); }
        pot = 0;
        betFaze = true;
        flop = false;
        turn = false;
        river = false;
    }
    explicit Table(Deck &deck, bool bf = false, bool f = false, bool t = false, bool r = true) {
        for (int i = 0; i < 5; i++) {
            tableHand[i] = deck.popFCard();
            deck.cycleFCard();
        }
        betFaze = bf;
        flop = f;
        turn = t;
        river = r;
        pot = 0;
    }
    void setBetFaze(bool bf) { betFaze = bf; }
    void setTurn(bool t) { turn = t; }
    void setRiver(bool r) { river = r; }
    void setFlop(bool f) { flop = f; }
    int getTC() const {
        if (betFaze) return 91;
        else if (flop) return 92;
        else if (turn) return 93;
        else if (river) return 94;
        else return 95;
    }
    void printTable() const {
        if (betFaze) { std::cout << "No cards on table" << std::endl; }
        else{
            std::cout << "First table card is a " << valuefy[tableHand[0].getValue()] << " of " << suitfy[tableHand[0].getSuit()] << std::endl;
            std::cout << "Second table card is a " << valuefy[tableHand[1].getValue()] << " of " << suitfy[tableHand[1].getSuit()] << std::endl;
            std::cout << "Third table card is a " << valuefy[tableHand[2].getValue()] << " of " << suitfy[tableHand[2].getSuit()] << std::endl;
        if (turn) {
            std::cout << "Fourth table card is a " << valuefy[tableHand[3].getValue()] << " of " << suitfy[tableHand[3].getSuit()] << std::endl;
        }
        else if (river) {
            std::cout << "Fourth table card is a " << valuefy[tableHand[3].getValue()] << " of " << suitfy[tableHand[3].getSuit()] << std::endl;
            std::cout << "Fifth table card is a " << valuefy[tableHand[4].getValue()] << " of " << suitfy[tableHand[4].getSuit()] << std::endl;
        }}
        std::cout << std::endl;
    }
};

class HandEvaluator {
private:
    static void makeBestHand(int cardsMap[], int allTimeBestHand[]) {
        int bestHand[4] = {0};
        int strght = 0, flush = 0, quad = 0, triple = 0, pair = 0;

        // Flush detection
        for (int i = 14; i <= 17; i++) {
            if (cardsMap[i] >= 5) {
                flush = 1;
                break;
            }
        }

        // Straight detection
        if (cardsMap[13] == 1) cardsMap[0] = 1;
        for (int i = 0; i <= 9; i++) {
            if (cardsMap[i]*cardsMap[i+1]*cardsMap[i+2]*cardsMap[i+3]*cardsMap[i+4] == 1) {
                strght = 1;
                break;
            }
        }

        // Quad, Triple, Pairs detection
        for (int i = 1; i <= 13; i++) {
            if (cardsMap[i] == 4) {quad = 1; break;}
            if (cardsMap[i] == 3) triple = 1;
            if (cardsMap[i] == 2) pair++;
        }

        // Hand ranking logic
        if (flush && strght) {
            bestHand[0] = 9;
            if (cardsMap[13] == 1 && cardsMap[12] == 1) bestHand[1] = 13;
            else {
                for (int i = 12; i >= 1; i--) {
                    if (cardsMap[i] == 1) {
                        bestHand[1] = i;
                        break;
                    }
                }
            }
        }
        else if (quad) {
            bestHand[0] = 8;
            for (int i = 1; i <= 13; i++) {
                if (cardsMap[i] == 4) {
                    bestHand[1] = i;
                }
                if (cardsMap[i] == 1) {
                    bestHand[2] = i;
                }
            }
        }
        else if (triple == 1 && pair == 1) {
            bestHand[0] = 7;
            for (int i = 1; i <= 13; i++) {
                if (cardsMap[i] == 3) {
                    bestHand[1] = i;
                }
                if (cardsMap[i] == 2) {
                    bestHand[2] = i;
                }
            }
        }
        else if (flush) {
            bestHand[0] = 6;
            for (int i = 13; i >= 1; i--) {
                if (cardsMap[i] == 1) {
                    bestHand[1] = i;
                    break;
                }
            }
        }
        else if (strght) {
            bestHand[0] = 5;
            if (cardsMap[13] == 1 && cardsMap[12] == 1) bestHand[1] = 13;
            else {
                for (int i = 12; i >= 1; i--) {
                    if (cardsMap[i] == 1) {
                        bestHand[1] = i;
                        break;
                    }
                }
            }
        }
        else if (triple == 1) {
            bestHand[0] = 4;
            for (int i = 1; i <= 13; i++) {
                if (cardsMap[i] == 3) {
                    bestHand[1] = i;
                }
                else if (cardsMap[i] == 1 && i > bestHand[2]) {
                    bestHand[2] = i;
                }
            }
        }
        else if (pair == 2) {
            bestHand[0] = 3;
            for (int i = 13; i >= 1; i--) {
                if (cardsMap[i] == 2) {
                    if (bestHand[1] == 0) bestHand[1] = i;
                    else bestHand[2] = i;
                }
            }
            for (int i = 13; i >= 1; i--) {
                if (cardsMap[i] == 1) {
                    bestHand[3] = i;
                    break;
                }
            }
        }
        else if (pair == 1) {
            bestHand[0] = 2;
            for (int i = 1; i <= 13; i++) {
                if (cardsMap[i] == 2) {
                    bestHand[1] = i;
                }
                else if (cardsMap[i] == 1 && i > bestHand[2]) {
                    bestHand[2] = i;
                }
            }
        }
        else {
            bestHand[0] = 1;
            for (int i = 13; i >= 1; i--) {
                if (cardsMap[i] == 1) {
                    bestHand[1] = i;
                    break;
                }
            }
        }

        int winner = 2;
        for (int i = 0; i < 4; i++) {
            if (bestHand[i] > allTimeBestHand[i]) {
                winner = 1;
                break;
            }
            else if (bestHand[i] < allTimeBestHand[i]) {
                winner = 0;
                break;
            }
        }
        if (winner == 1) {
            for (int i = 0; i < 4; i++) {
                allTimeBestHand[i] = bestHand[i];
            }
        }
    }

    static void createHandMap(const std::vector<Card> &Five, int allTimeBestHand[]) {
        int cardsMap[18] = {0};
        for (int i = 0; i < 5; i++) {
            cardsMap[Five[i].getValue()]++;
            cardsMap[14 + Five[i].getSuit()]++;
        }
        makeBestHand(cardsMap, allTimeBestHand);
    }

    static void generateHands(int k, const std::vector<Card>& Seven, int x[], int allTimeBestHand[]) {
        for(int i = x[k-1] + 1; i <= 7; ++i) {
            x[k] = i;
            if(k == 5) {
                std::vector<Card> Five(5);
                for (int j = 1; j <= 5; j++) {
                    Five[j-1] = Seven[x[j]-1];
                }
                createHandMap(Five, allTimeBestHand);
            }
            else {
                generateHands(k + 1, Seven, x, allTimeBestHand);
            }
        }
    }

public:
    static void determineBestHands(std::vector<Player> &playerVector, const std::vector<Card> &communityCards) {
        for (auto &player : playerVector) {
            std::vector<Card> Seven = communityCards;
            Seven.push_back(player.getHandI(0));
            Seven.push_back(player.getHandI(1));

            int x[6] = {0}, allTimeBestHand[4] = {0};
            generateHands(1, Seven, x, allTimeBestHand);
            player.setBestHand(allTimeBestHand);
            player.printBestHand();
        }
    }
};

class GameManager {
private:
    static void getQueuePStates(const std::vector<int>& gameQueue, std::vector<int> &results) {
        results.clear();
        for (const int i : gameQueue) {
            if (i / 10 != 9) {
                results.push_back(i % 10);
            }
        }
    }

public:
    static void runGame(std::vector<Player> &playerVector, Table &table) {
        int P1 = 10, P2 = 20, P3 = 30, P4 = 40, TC = 90;
        std::vector<int> gameQueue = {P1, P2, P3, P4, TC};
        std::vector<int> results;

        while (gameQueue.size() > 2 && TC % 10 < 4) {
            for (const int i : gameQueue) {
                cout << i << " ";
            }
            cout<<endl;
            getQueuePStates(gameQueue, results);
            for (const int result : results) {
                cout << result << " ";
            }
            cout << endl;
            int currentPlayer = gameQueue[0] / 10 - 1;
            int qsize = static_cast<int>(gameQueue.size());
            if (currentPlayer == 8) {
                int cr = table.getTC() % 10;
                if (cr == 1) {
                    table.setFlop(true);
                    table.setBetFaze(false);
                }
                else if (cr == 2) {
                    table.setTurn(true);
                    table.setFlop(false);
                }
                else if (cr == 3) {
                    table.setRiver(true);
                    table.setTurn(false);
                }
                else {
                    std::cout << "Game Over" << std::endl;
                    break;
                }
                TC++;
                table.printTable();
                gameQueue.push_back(gameQueue[0]);
                gameQueue.erase(gameQueue.begin());
                for (int i = 0; i < qsize - 1; i++) gameQueue[i] = (gameQueue[i] / 10) * 10;
                for (int i = 0; i < qsize - 1; i++) {
                    for (int j = i + 1; j < qsize - 1; j++) {
                        if (gameQueue[j] / 10 < gameQueue[i] / 10) {
                            std::swap(gameQueue[j], gameQueue[i]);
                        }
                }
                }
            }
            else {
                int action;
                cout<< "Player " << currentPlayer + 1 << " turn" << endl;
                invalid:
                action = playerVector[currentPlayer].chooseAction();
                if (action == 0) {
                    cout << "Player " << currentPlayer + 1 << " folded" << endl;
                    gameQueue.erase(gameQueue.begin());
                }
                if (action == 3) {
                    bool ok = true;
                    for (unsigned long i = 0; i < results.size(); i++){
                        if (results[i] == 1 || results[i] == 2) {
                            ok = false;
                            break;
                        }
                    }
                    if (ok) {
                        gameQueue[0] = (gameQueue[0] / 10) * 10 + action;
                        gameQueue.push_back(gameQueue[0]);
                        gameQueue.erase(gameQueue.begin());
                    }
                    else {cout<<"Can't check if there's a bet"<<endl; goto invalid;}
                }
                if (action == 4) {
                    bool ok = false;
                    for (unsigned long i = 0; i < results.size(); i++){
                        if (results[i] == 1 || results[i] == 2) {
                            ok = true;
                            break;
                        }
                    }
                    if (ok) {
                        gameQueue[0] = (gameQueue[0] / 10) * 10 + action;
                        gameQueue.push_back(gameQueue[0]);
                        gameQueue.erase(gameQueue.begin());
                    }
                    else {cout<<"Can't call when no bet"<<endl; goto invalid;}

                }
                if (action == 1) {
                    bool ok = true;
                    for (unsigned long i = 0; i < results.size(); i++){
                        if (results[i] == 2 || results[i] == 1) {
                            ok = false;
                            break;
                        }
                    }
                    if (ok) {
                        gameQueue[0] = (gameQueue[0] / 10) * 10 + action;
                        gameQueue.push_back(gameQueue[0]);
                        gameQueue.erase(gameQueue.begin());
                        for (int i = 0; i < qsize; i++) {
                            if (gameQueue[i]/10 == TC/10) {
                                gameQueue.erase(gameQueue.begin() + i);
                                gameQueue.insert(gameQueue.end() - 1, TC);
                                break;
                            }
                        }
                    }
                    else {cout<<"Can't bet when there's a raise/bet"<<endl; goto invalid;}
                    }
                if (action == 2) {
                    int tert = 0;
                    for (unsigned long i = 0; i < results.size(); i++){
                        if (results[i] == 1) {tert = 1; break;}
                    }
                    for (unsigned long i = 0; i < results.size(); i++){
                        if (results[i] == 2) {tert = 2; break;}
                    }
                    if (tert == 0) {cout<<"Can't raise when there's no bet"<<endl; goto invalid;}
                    if (tert == 1) {
                        gameQueue[0] = (gameQueue[0] / 10) * 10 + action;
                        gameQueue.push_back(gameQueue[0]);
                        gameQueue.erase(gameQueue.begin());
                        for (int i = 0; i < qsize; i++) {
                            if (gameQueue[i]/10 == TC/10) {
                                gameQueue.erase(gameQueue.begin() + i);
                                gameQueue.insert(gameQueue.end() - 1, TC);
                                break;
                            }
                        }
                    }
                    if (tert == 2) {
                        for (int i = 0; i < qsize; i++) {
                            if (gameQueue[i] % 10 == 2 || gameQueue[i] % 10 == 1) gameQueue[i] = gameQueue[i] - 1;
                        }
                        gameQueue[0] = (gameQueue[0] / 10) * 10 + action;
                        gameQueue.push_back(gameQueue[0]);
                        gameQueue.erase(gameQueue.begin());
                        for (int i = 0; i < qsize; i++) {
                            if (gameQueue[i]/10 == TC/10) {
                                gameQueue.erase(gameQueue.begin() + i);
                                gameQueue.insert(gameQueue.end() - 1, TC);
                                break;
                            }
                        }
                    }
                    }
            }
        }
    }
};

int main() {
    Deck deck;
    deck.shuffle();

    Player player1(deck), player2(deck), player3(deck), player4(deck);
    std::vector<Player> playerVector = {player1, player2, player3, player4};

    for (auto &player : playerVector) {
        player.printHand();
    }

    Table table(deck, true, false, false, false);
    table.printTable();

    GameManager::runGame(playerVector, table);

    std::vector<Card> communityCards;
    for (int i = 0; i < 5; i++) {
        communityCards.push_back(table.getHandI(i));
    }

    HandEvaluator::determineBestHands(playerVector, communityCards);

    return 0;
}