#include <iostream>
#include <algorithm>
#include <random>
#include <vector>
#include <array>
#include <ostream>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <memory>
#include <cstring>
using std::cout;
using std::cin;
using std::endl;

const std::array<std::string,5> suitfy = {"Hearts", "Diamonds", "Clubs", "Spades", "all Suits"};
const std::array<std::string,14> valuefy = {"Joker", "2", "3", "4", "5", "6", "7", "8", "9", "10", "Jack", "Queen", "King", "Ace"};

class InvalidPlayerNameException : public std::runtime_error {
public:
    InvalidPlayerNameException(const std::string& message) : std::runtime_error(message) {}
};

class PlayerNameValidator {
public:
    static void validate(const std::string& name) {
        if (name.empty()) {throw InvalidPlayerNameException("Player name cannot be empty.");}
        if (name.length() > 10) {throw InvalidPlayerNameException("Player name cannot be longer than 10 characters.");}
        for (char c : name) {
            if (!std::isalnum(c) && c != ' ') {
                throw InvalidPlayerNameException("Player name can only contain letters, numbers, and spaces.");
            }
        }
    }
};

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
    int betted;
    int PlayerID;
//protected:
//    char name[10];
public:
    explicit Player(Deck &deck, int id = -1) {
        hand[0] = deck.popFCard();
        deck.cycleFCard();
        hand[1] = deck.popFCard();
        deck.cycleFCard();
        money = 1000;
        betted = 0;
        bestHand[0] = 0;
        PlayerID = id;
    }
    void newMatch(Deck &deck) {
        hand[0] = deck.popFCard();
        deck.cycleFCard();
        hand[1] = deck.popFCard();
        deck.cycleFCard();
        betted = 0;
        for (int i = 0; i < 4; i++){bestHand[i] = 0;};
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
    int getBestHandI(int i) {
        return bestHand[i];
    }
    Card getHandI(int i) {
        return hand[i];
    }
    virtual int chooseAction(int tableState, int maxBetted) = 0;
    const int getMoney() const { return money; }
    const int getBetted() const { return betted; }
    const int getPlayerID() const { return PlayerID; }
    void setMoney(int m) { money = m; }
    void setBetted(int b) { betted = b; }
    virtual int chooseBet(int maxBetted) = 0;
    virtual void setName() = 0;
    void printMoney() {
        std::cout << "You have " << money << " money" << std::endl;
    }
    void printHand() {
        std::cout << "First card is a " << valuefy[hand[0].getValue()] << " of " << suitfy[hand[0].getSuit()] << std::endl;
        std::cout << "Second card is a " << valuefy[hand[1].getValue()] << " of " << suitfy[hand[1].getSuit()] << std::endl;
        std::cout << std::endl;
    }
    friend std::ostream & operator<<(std::ostream &os, const Player &obj) {
        return os << "money: " << obj.money;
    }
    virtual ~Player() = default;
    virtual const char* getName() = 0;
};

class Human : public Player {
    char name[10];
public:
    Human(Deck &deck, int id = -1) : Player(deck, id) {}
    int chooseAction(int tableState, int maxBetted) override {
        //Maybe adding tips based on vars
        std::cout << "Choose an action: 0-Fold   1-Bet betAmount   2-Raise moneyPutDown   3-Check   4-Call" << std::endl;
        int action = -1;
        std::cin >> action;
        switch (action) {
            case 1: std::cout /*<< "Betting" */<< std::endl; break;
            case 2: std::cout /*<< "Raising" */<< std::endl; break;
            case 3: std::cout /*<< "Checking" */<< std::endl; break;
            case 4: std::cout /*<< "Calling" */<< std::endl; break;
            case 0: std::cout /*<< "Folding" */<< std::endl; break;
            default: std::cout << "Invalid action" << std::endl; action = -1; break;
        }
        return action;
    }
    int chooseBet(int maxBetted) override {
        int bet = -1;
        if (!maxBetted) {
            std::cout << "Choose bet amount" << std::endl;
        }
        else {
            std::cout << "Choose raise amount higher than " << maxBetted << std::endl;
        }
        std::cin >> bet;
        return bet;
    }
    void setName() override {
        std::cout << "Enter Player " << getPlayerID() << "'s name: (No longer than 10 characters, only letters, numbers and spaces)" << std::endl;
        char n[10];
        std::cin >> n;
        strcpy(name, n);
    }
    const char* getName() override {
        return name;
    }
    ~Human() override = default;
};

class Bot : public Player {
    int strenght;
    int m;
    int bluff;
    std::array<int, 18> bluffBias;
    int power;
    int cardsMap[18];
    char name[10];
public:
    const int setBluff (std::array<int, 18> &bluffBias) {
        int r = rand() % 18;
        int plus = rand() % 11;
        return int(bluffBias[r] + plus);
    }
    Bot(Deck &deck, int id = -1) : Player(deck, id) {
        strenght = 45;
        m = 0;
        bluffBias = {0, 10, 20, 30, 40, 40, 50, 50, 50, 50, 60, 60, 60, 70, 70, 75, 80, 90};
        bluff = setBluff(bluffBias);
        power = 50;
        strcpy(name, "Bot");
        for (int i = 0; i < 18; i++) {cardsMap[i] = 0;}
        //cout<< "Bot created with bluff of " << bluff << endl;
    }
    int chooseAction(int tableState, int maxBetted) override {
    // less Raises More low bet calls Register the others bluff
        double inc, f_, a, b;
        bool action1, action2;
        tableState = tableState % 10;
        int value1 = getHandI(0).getValue(), value2 = getHandI(1).getValue(),suit1 = getHandI(0).getSuit(), suit2 = getHandI(1).getSuit();
        inc = getMoney() / 100;
        if (tableState == 1){
            strenght = value1 + value2;
            if (value2 == 13) std::swap(value1, value2);
            if (value1 == value2) {
                strenght += 35;
            } else {
                if (suit1 == suit2) {
                    strenght += 20;
                }
                if (value1 != 13 && abs(value1 - value2) <= 4) {
                    strenght += (5 - abs(value1 - value2)) * 5;
                }
                else if (value1 == 13 && std::min(abs(value1 - value2),abs(1 - value2)) <= 4) {
                    strenght += (5 - std::min(abs(value1 - value2),abs(1 - value2))) * 5;
                }
            }
        }
        else {
            strenght = 0;
            double a, b, c, d;
            a = getBestHandI(0); b = getBestHandI(1);
            c = getBestHandI(2); d = getBestHandI(3);
            if (a == 1) {
                strenght = 3*b/4;
            }
            else if (a == 2) {
                strenght = 10 + 5*b/4 + c/4;
            }
            else if (a == 3) {
                strenght = 30 + 11*b/12 + c/2 + d/6;
            }
            else if (a == 4) {
                strenght = 50 + 2*b/3 + c/9;
            }
            else if (a == 5) {
                strenght = 60 + 3*b/4;
            }
            else if (a == 6) {
                strenght = 70 + 3*b/8;
            }
            else if (a == 7) {
                strenght = 75 + 3*b/4 + 3*c/8;
            }
            else if (a == 8) {
                strenght = 90 + 3*b/4 + c/50;
            }
            else if (a == 9) {
                strenght = 100;
            }
        }
        m = maxBetted / inc;
        f_ = strenght + power / 5 - 1 - sqrt(m) + pow(1.025, m);
        a = double(100-f_) / 1000;
        b = std::max(double(((100-f_)/1000)), std::min(double((3*(100-f_))/250-double(bluff)/300), 0.95));
        if (f_ >= 0 && f_ < 70 - double(2 * bluff) / 5 - strenght / 5)
            action1 = true, action2 = false; // Weaker hand
        else if (f_ >= 70 - double(2 * bluff) / 5 - strenght / 5 && f_ <= 100)
                action2 = true, action1 = false; // Stronger hand
        double random_float = (double)rand() / RAND_MAX;
        if (m <= 2) action2 = true, action1 = false;
        //cout << "[0," << a << "," << b << ",1] " << strenght;
        //cout << (action1 ? " Weak hand " : " Strong hand ");
        //cout << random_float << std::endl;
        if (random_float < a) {
            /*if (action1){
                return 0;
            }*/
            random_float += a;
        }
        if (random_float >= a && random_float < b) {
            if (!maxBetted) {
                cout << "Player " << getPlayerID() << " checked" << std::endl;
                return 3;
            }
            if (action1) {
                if (m <= 2) {
                    cout << "Player " << getPlayerID() << " called" << std::endl;
                    return 4;
                }
                else return 0;
            }
            else {
                cout << "Player " << getPlayerID() << " called" << std::endl;
                return 4;
            }
        }
        else if (random_float >= b) {
            if (!maxBetted) {
                cout << "Player " << getPlayerID() << " betted" << std::endl;
                return 1;
            }
            else {
                if (action1) {
                cout << "Player " << getPlayerID() << " called" << std::endl;
                return 4;
                }
                else {
                    if (m <= 4) {
                        cout << "Player " << getPlayerID() << " raised" << std::endl;
                        return 2;
                    }
                    else {
                        cout << "Player " << getPlayerID() << " called" << std::endl;
                        return 4;
                    }
                }
            }
            }
        }
    int chooseBet(int maxBetted) override {
        cout << "Bot " << getPlayerID() << " betted: " << getMoney() / 20 << std::endl;
        return maxBetted ? maxBetted + getMoney() / 20 : getMoney() / 20; //?
    }
    void setCardMap(int i, int v) {cardsMap[i] = v;}
    void setName() override {
        strcpy(name, "Bot ");
        strcat(name, std::to_string(getPlayerID()).c_str());
    }
    const char* getName() override {
        return name;
    }
    ~Bot() override = default;
};

class UnfairBot : public Bot {
//same as bot, one per match unknown, knows the cards during the river
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
    void newTable(Deck &deck) {
        for (int i = 0; i < 5; i++) {
            tableHand[i] = deck.popFCard();
            deck.cycleFCard();
        }
        betFaze = true;
        flop = false;
        turn = false;
        river = false;
    }
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
    static void generateNHands(int k, const std::vector<Card>& Ncard, int x[], int allTimeBestHand[],int n){
        for(int i = x[k-1] + 1; i <= n; ++i) {
            x[k] = i;
            if(k == 5) {
                std::vector<Card> Five(5);
                for (int j = 1; j <= 5; j++) {
                    Five[j-1] = Ncard[x[j]-1];
                }
                createHandMap(Five, allTimeBestHand);
            }
            else {
                generateNHands(k + 1, Ncard, x, allTimeBestHand, n);
            }
        }
    }
public:
    static void determineBestHands(std::vector<Player*> &playerVector, const std::vector<Card> &communityCards,int n = 7) {
        for (auto &player : playerVector) {
            std::vector<Card> Seven = communityCards;
            Seven.push_back(player->getHandI(0));
            Seven.push_back(player->getHandI(1));

            int x[6] = {0}, allTimeBestHand[4] = {0};
            generateNHands(1, Seven, x, allTimeBestHand,n);
            player->setBestHand(allTimeBestHand);
            //player->printBestHand();
        }
    }
    static void determineWinner(std::vector<Player*> &playerVector, int pot) {
        int Winner = 0;
        std::vector<int> tiers;
        //test
        for (auto &player : playerVector) {
            std::cout << "Player " << player->getPlayerID() << endl;
            cout << player->getBestHandI(0) << " " << player->getBestHandI(1) << " " << player->getBestHandI(2) << " " << player->getBestHandI(3) << endl;
        }
        int currentBestHand[4] = {0}, allTimeBestHand[4] = {0};
        for (int i = 0; i < playerVector.size(); i++){
            for (int j = 0; j < 4; j++) {
                currentBestHand[j] = playerVector[i]->getBestHandI(j);
            }
            if (i == 0) {
                for (int j = 0; j < 4; j++) {
                    allTimeBestHand[j] = currentBestHand[j];
                }
                Winner = playerVector[i]->getPlayerID();
                tiers.push_back(Winner);
            }
            else {
                bool tie = true;
                for (int j = 0; j < 4; j++) {
                    if (currentBestHand[j] > allTimeBestHand[j]) {
                        tie = false;
                        for (int k = 0; k < 4; k++) {
                            allTimeBestHand[k] = currentBestHand[k];
                        }
                        Winner = playerVector[i]->getPlayerID();
                        tiers.clear();
                        tiers.push_back(Winner);
                        break;
                    }
                    else if (currentBestHand[j] < allTimeBestHand[j]) {
                        tie = false;
                        break;
                    }
                }
                if (tie) {
                    tiers.push_back(playerVector[i]->getPlayerID());
                }
            }
        }
        int tsize = tiers.size();
        if (tsize == 1) {
            std::cout << "Player " << Winner << " wins" << std::endl;
            for (auto &player : playerVector) {
                if (player->getPlayerID() == Winner) {
                    player->setMoney(player->getMoney() + pot);
                }
            }
        }
        else {
            float nr = 0;
            std::cout << "It's a tie between players: ";
            for (const auto &playerID : tiers) {
                std::cout << playerID << " ";
                nr++;
            }
            nr = pot / nr;
            for (auto &player : playerVector) {
                for (const auto &playerID : tiers) {
                    if (player->getPlayerID() == playerID) {
                        player->setMoney(player->getMoney() + nr);
                    }
                }
            }
            std::cout << std::endl;
        }
    }
};

class GameManager {
    static void getQueuePStates(const std::vector<int>& gameQueue, std::vector<int> &results) {
        results.clear();
        for (const int i : gameQueue) {
            if (i / 10 != 9) {
                results.push_back(i % 10);
            }
        }
    }
    static void resetMaxBetted(std::vector<Player*> &playerVector) {
        for (auto &player : playerVector) {
            player->setBetted(0);
        }
    }
    static int getMaxBetted(const std::vector<Player*> &playerVector) {
        int maxBetted = 0;
        for (const auto &player : playerVector) {
            if (player->getBetted() > maxBetted) {
                maxBetted = player->getBetted();
            }
        }
        return maxBetted;
    }
    static void endMatch(std::vector<Player*> &playerVector, std::vector<Player*> &foldedPlayers) {
        for (auto &player : foldedPlayers) {
            playerVector.push_back(player);
        }
        // sort by how they started match, maybe by cycling by round % 4; private round
        std::sort(playerVector.begin(), playerVector.end(), [](const Player* a, const Player* b) {
            return a->getPlayerID() < b->getPlayerID();
        });
        for (auto &player : playerVector) {
            player->printHand();
        }
        for (int i = 0; i < playerVector.size(); i++) {
            if (playerVector[i]->getMoney() == 0) {
                std::cout << "Player " << playerVector[i]->getPlayerID() << " is out of the game" << std::endl;
                playerVector.erase(playerVector.begin() + i);
                i--;
            }
        }
        for (auto &player : playerVector) {
            std::cout << player->getName() << " has " << player->getMoney() << " money" << std::endl;
        }
    }
public:
    static void runGame(std::vector<Player*> &playerVector, Table &table) {
        int TC = 90;
        int pot = 0;
        std::vector<int> foledPlayersIDs;
        std::vector<Player*> foldedPlayers;
        std::vector<int> gameQueue;
        for (auto &player : playerVector) {gameQueue.push_back(player->getPlayerID() * 10);}
        gameQueue.push_back(TC);
        std::vector<int> results;

        while (gameQueue.size() > 2 && TC % 10 < 4) {
            /*for (const int i : gameQueue) {cout << i << "  ";}*/
            /*for (auto &player : playerVector) {cout << player->getPlayerID() << " ";}*/
            /*for (auto &player : playerVector) {cout << player->getBetted() << " "; }*/
            getQueuePStates(gameQueue, results);
            int currentPlayer = gameQueue[0] / 10 - 1;
            int qsize = static_cast<int>(gameQueue.size());
            if (currentPlayer == 8) {
                cout << "Current pot is " << pot << endl;
                resetMaxBetted(playerVector);
                int cr = table.getTC() % 10;
                int active_players;
                active_players = playerVector.size();
                for (auto& players : playerVector) {
                    if (players->getMoney() == 0) active_players--;
                }
                if (active_players <= 1)
                {cr=4;table.setBetFaze(false);table.setFlop(false);table.setTurn(false);table.setRiver(true);}
                if (cr == 1) {
                    table.setFlop(true);
                    table.setBetFaze(false);
                    std::vector<Card> First3Cards;
                    for (int i = 0; i < 3; i++) {First3Cards.push_back(table.getHandI(i));}
                    HandEvaluator::determineBestHands(playerVector, First3Cards, 5);
                }
                else if (cr == 2) {
                    table.setTurn(true);
                    table.setFlop(false);
                    std::vector<Card> First4Cards;
                    for (int i = 0; i < 4; i++) {First4Cards.push_back(table.getHandI(i));}
                    HandEvaluator::determineBestHands(playerVector, First4Cards, 6);
                }
                else if (cr == 3) {
                    table.setRiver(true);
                    table.setTurn(false);
                    std::vector<Card> First7Cards;
                    for (int i = 0; i < 5; i++) {First7Cards.push_back(table.getHandI(i));}
                    HandEvaluator::determineBestHands(playerVector, First7Cards, 7);
                }
                else {
                    //std::vector<Card> communityCards;
                    //for (int i = 0; i < 5; i++) {communityCards.push_back(table.getHandI(i));}
                    /*for (int ids : foledPlayersIDs) {
                        for (int i = 0; i < playerVector.size(); i++) {
                            if (playerVector[i]->getPlayerID() == ids) {
                                playerVector.erase(playerVector.begin() + i);
                                break;
                            }
                        }
                    }*/
                    //HandEvaluator::determineBestHands(playerVector, communityCards);
                    HandEvaluator::determineWinner(playerVector, pot);
                    std::cout << "Match Over" << std::endl;
                    std::cout << std::endl;
                    endMatch(playerVector, foldedPlayers);
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
                // possible problem here
                cout << playerVector[currentPlayer]->getName() << "'s turn" << endl;
                int position;
                for (int i = 0; i < playerVector.size(); i++) {
                    if (playerVector[i]->getPlayerID() == currentPlayer + 1) {
                        position = i;
                        break;
                    }
                }
                playerVector[position]->printMoney();
                if (playerVector[position]->getMoney()){
                invalid:
                action = playerVector[position]->chooseAction(table.getTC() ,getMaxBetted(playerVector));
                if (action == 0) {
                    cout << "Player " << currentPlayer + 1 << " folded" << endl;
                    gameQueue.erase(gameQueue.begin());
                    foldedPlayers.push_back(playerVector[position]);
                    playerVector.erase(playerVector.begin() + position);
                    foledPlayersIDs.push_back(currentPlayer + 1);
                    if (gameQueue.size() == 2) {
                        //int pl;
                        //pl = std::min(gameQueue[0]/10, gameQueue[1]/10) - 1;
                        std::cout << "Player " << playerVector[0]->getPlayerID() << " wins" << std::endl;
                        playerVector[0]->setMoney(playerVector[0]->getMoney() + pot);
                        endMatch(playerVector, foldedPlayers);
                        break;
                    }
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
                        //pot; add side pot later
                        int maxBetted = getMaxBetted(playerVector);
                        int call = maxBetted - playerVector[position]->getBetted();
                        if (call > playerVector[position]->getMoney()) {
                            call = playerVector[position]->getMoney();
                            playerVector[position]->setMoney(0);
                            pot += call;
                            playerVector[position]->setBetted(call);
                            //handle side pot
                        }
                        else {
                            playerVector[position]->setMoney(playerVector[position]->getMoney() - call);
                            pot += call;
                            playerVector[position]->setBetted(maxBetted);
                        }
                        //order
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
                        //pot
                        ///cout << "Choose bet amount" << endl;
                        int bet;
                        bet = playerVector[position]->chooseBet(getMaxBetted(playerVector));
                        if (bet > playerVector[position]->getMoney()) {
                            cout << "Not enough money" << endl;
                            goto invalid;
                        }
                        else {
                            playerVector[position]->setMoney(playerVector[position]->getMoney() - bet);
                            pot += bet;
                            playerVector[position]->setBetted(bet);
                        }
                        //order
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
                    //pot
                    ///cout << "Choose raise amount" << endl;
                    int raise;
                    raise = playerVector[position]->chooseBet(getMaxBetted(playerVector));
                    if (raise - playerVector[position]->getBetted() > playerVector[position]->getMoney()) {
                        cout << "Not enough money" << endl;
                        goto invalid;
                    }
                    else {int maxBetted = getMaxBetted(playerVector);
                    if (raise <= maxBetted) {
                        cout << "Raise must be higher than the max bet" << endl;
                        goto invalid;
                    }
                    else {
                        int currBet = playerVector[position]->getBetted();
                        playerVector[position]->setMoney(playerVector[position]->getMoney() - raise + currBet);
                        pot += raise - currBet;
                        playerVector[position]->setBetted(raise);
                    }}
                    //order
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
                cout << endl;
                }
                else {
                    gameQueue[0] = (gameQueue[0] / 10) * 10 + action;
                    gameQueue.push_back(gameQueue[0]);
                    gameQueue.erase(gameQueue.begin());
            }
            }
        }
    }
};

int main() {
    // To implement: split pots, small/big blinds. daca dau toti all in
    srand(time(0));
    Deck deck;
    Table table(deck, true, false, false, false);
    Player *player1Ptr, *player2Ptr, *player3Ptr, *player4Ptr;
    Human player1(deck, 1);
    player1Ptr = &player1;
    Bot player2(deck, 2);
    player2Ptr = &player2;
    Bot player3(deck, 3);
    player3Ptr = &player3;
    Bot player4(deck, 4);
    player4Ptr = &player4;
    std::vector<Player*> playerVector = {player1Ptr, player2Ptr, player3Ptr, player4Ptr};
    int round = 1;
    for (auto &player : playerVector) {
        player->setName();
        try {
        PlayerNameValidator::validate(player->getName());
        std::cout << "Player name '" << player->getName() << "' is valid." << std::endl;
        }
        catch (const InvalidPlayerNameException& e) {
        std::cerr << "Invalid player name: " << e.what() << std::endl;
        }
    }
    cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
    cout << "Game started" << endl;
    while (playerVector.size() > 1) {
        deck.shuffle();
        for (auto &player : playerVector) {
            //cout <<player->getPlayerID() << endl;
            player->newMatch(deck);
            //player->printHand();
        }
        table.newTable(deck);
        playerVector[0]->printHand();
        GameManager::runGame(playerVector, table);
        /*for (int i = 0; i < round % 4; i++) {
            std::rotate(playerVector.begin(), playerVector.begin() + 1, playerVector.end());
        }*/
        round++;
        cout << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl;
    }
    return 0;
}