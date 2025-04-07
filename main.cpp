#include <iostream>
#include <algorithm>
#include <random>
#include <vector>
#include <array>
using std::cout;
using std::cin;
using std::endl;
//                                        0       1         2           3                   4           5           6           7           8                 9
const std::array<std::string,10> handfy = {"?","High Card", "One Pair", "Two Pair", "Three of a Kind", "Straight", "Flush", "Full House", "Four of a Kind", "Royal Flush"};
//                                          0          1         2           3           4
const std::array<std::string,5> suitfy = {"Hearts", "Diamonds", "Clubs", "Spades", "all Suits"};
const std::array<std::string,14> valuefy = {"Joker", "2", "3", "4", "5", "6", "7", "8", "9", "10", "Jack", "Queen", "King", "Ace"};
//                                            0             1       2       3       4       5       6       7       8       9         10          11         12         13

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
    // 0 -> Top of the deck
    // Using a card puts it at the bottom of the deck
public:
    void shuffle() {
        static std::random_device rd;
        static std::mt19937 g(rd());
        std::shuffle(std::begin(cards), std::end(cards), g);
        std::cout << "Shuffled" << std::endl;
    };
    /*void print() {
        for (int i = 0; i < 52; i++) {
            std::cout << valuefy[cards[i].getValue()] << " of " << suitfy[cards[i].getSuit()] << std::endl;
        }
    } */
    Card popFCard() {
        return cards[0];
    }
    void cycleFCard() {
        int su = cards[0].getSuit();
        int va = cards[0].getValue();
        for (int i = 0; i < 51; i++) {
            cards[i] = cards[i + 1]; // CC ?
        }
        cards[51] = Card(va, su); // CC ?
        //std::cout << valuefy[va] << " of " << suitfy[su] << " was turned" << std::endl;
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
    bool isPlaying;
    bool itsTurn;
public:
    explicit Player(Deck &deck) {
        hand[0] = deck.popFCard();
        deck.cycleFCard();
        hand[1] = deck.popFCard();
        deck.cycleFCard();
        money = 1000;
        bestHand[0] = 0;
        isPlaying = true;
        itsTurn = false;
    }
    void setBestHand(int bH[]) {
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
    [[nodiscard]] const Card* getHand() const { return hand; }
    void printHand() {
        std::cout << "First card is a " << valuefy[hand[0].getValue()] << " of " << suitfy[hand[0].getSuit()] << std::endl;
        std::cout << "Second card is a " << valuefy[hand[1].getValue()] << " of " << suitfy[hand[1].getSuit()] << std::endl;
        std::cout << std::endl;
    }
};

class Table {
    Card tableHand[5];
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
        betFaze = true;
        river = false;
        flop = false;
        turn = false;
    }
    explicit Table(Deck &deck, bool bf = false, bool f = false, bool t = false, bool r = true) {
        tableHand[0] = deck.popFCard();
        deck.cycleFCard();
        tableHand[1] = deck.popFCard();
        deck.cycleFCard();
        tableHand[2] = deck.popFCard();
        deck.cycleFCard();
        tableHand[3] = deck.popFCard();
        deck.cycleFCard();
        tableHand[4] = deck.popFCard();
        deck.cycleFCard();
        betFaze = bf;
        flop = f;
        turn = t;
        river = r;
    }
    [[nodiscard]] const Card* getTableHand() const { return tableHand; }
    Table(const Table &other)
        : betFaze(other.betFaze),
          flop(other.flop),
          turn(other.turn),
          river(other.river) {
    }
    Table(Table &&other) noexcept
        : betFaze(other.betFaze),
          flop(other.flop),
          turn(other.turn),
          river(other.river) {
    }
    Table & operator=(const Table &other) {
        if (this == &other)
            return *this;
        betFaze = other.betFaze;
        flop = other.flop;
        turn = other.turn;
        river = other.river;
        return *this;
    }
    Table & operator=(Table &&other) noexcept {
        if (this == &other)
            return *this;
        betFaze = other.betFaze;
        flop = other.flop;
        turn = other.turn;
        river = other.river;
        return *this;
    }
    friend std::ostream & operator<<(std::ostream &os, const Table &obj) {
        return os
               << "betFaze: " << obj.betFaze
               << " flop: " << obj.flop
               << " turn: " << obj.turn
               << " river: " << obj.river;
    }
    ~Table() = default;
    void printTable() {
        if (betFaze) { std::cout << "No cards on table" << std::endl; }
        else{
            std::cout << "First table card is a " << valuefy[tableHand[0].getValue()] << " of " << suitfy[tableHand[0].getSuit()] << std::endl;
            std::cout << "Second table card is a " << valuefy[tableHand[1].getValue()] << " of " << suitfy[tableHand[1].getSuit()] << std::endl;
            std::cout << "Third table card is a " << valuefy[tableHand[2].getValue()] << " of " << suitfy[tableHand[2].getSuit()] << std::endl;
        if (turn) {
            std::cout << "Fourth table card is a " << valuefy[tableHand[3].getValue()] << " of " << suitfy[tableHand[3].getSuit()] << std::endl;
        }
        else {
            std::cout << "Fourth table card is a " << valuefy[tableHand[3].getValue()] << " of " << suitfy[tableHand[3].getSuit()] << std::endl;
            std::cout << "Fifth table card is a " << valuefy[tableHand[4].getValue()] << " of " << suitfy[tableHand[4].getSuit()] << std::endl;
        }}
        std::cout << std::endl;
    }

};

void makeBestHand(int cardsMap[], int allTimeBestHand[]) {
    int bestHand[4] = {0};
    int strght = 0, flush = 0, quad = 0, triple = 0, pair = 0 ;
    // Flush
    for (int i = 14; i <= 17; i++) {
        if (cardsMap[i] >= 5) {
            flush = 1;
            break;
        }
    }
    // Straight
    if (cardsMap[13] == 1) cardsMap[0] = 1;
    for (int i = 0; i <= 9; i++) {
        if (cardsMap[i]*cardsMap[i+1]*cardsMap[i+2]*cardsMap[i+3]*cardsMap[i+4] == 1) {
            strght = 1;
            break;
        }
    }
    // Quad, Triple, Pairs
    for (int i = 1; i <= 13; i++) {
        if (cardsMap[i] == 4) {quad = 1; break;}
        if (cardsMap[i] == 3) triple = 1;
        if (cardsMap[i] == 2) pair++;
    }
    // Best Hand
    if (flush && strght) // Royal Flush
    {
        bestHand[0] = 9;
        if (cardsMap[13] == 1 && cardsMap[12] == 1) bestHand[1] = 13;
        else {
            for (int i = 12; i >= 1; i--)
            {
                if (cardsMap[i] == 1) {
                    bestHand[1] = i;
                    break;
                }
            }
        }
    }
    else if (quad) // Four of a Kind
    {
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
    else if (triple == 1 && pair == 1) // Full House
    {
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
    else if (flush) // Flush
    {
        bestHand[0] = 6;
        for (int i = 13; i >= 1; i--) {
            if (cardsMap[i] == 1) {
                bestHand[1] = i;
                break;
            }
        }
    }
    else if (strght) // Straight
    {
        bestHand[0] = 5;
        if (cardsMap[13] == 1 && cardsMap[12] == 1) bestHand[1] = 13;
        else {
            for (int i = 12; i >= 1; i--)
            {
                if (cardsMap[i] == 1) {
                    bestHand[1] = i;
                    break;
                }
            }
        }
    }
    else if (triple == 1) // Three of a Kind
    {
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
    else if (pair == 2) // Two Pair
    {
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
    else if (pair == 1) // One Pair
    {
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
    else // High Card
    {
        bestHand[0] = 1;
        for (int i = 13; i >= 1; i--) {
            if (cardsMap[i] == 1) {
                bestHand[1] = i;
                break;
            }
        }
    }
    int winner = 2; //bH = 1, aTBH = 0 tie = 2
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

void createHandMap(int xfake[], const std::vector<Card> &Seven, int allTimeBestHand[])
{
    std::vector<Card> Five(5);
    for (int i = 1; i <= 5; i++) {
      Five[i - 1] = Seven[xfake[i] - 1];
    }
    int cardsMap[18] = {0}; // 0 unused, 1-13 cards, 14-17 suits
    for (int i = 0; i < 5; i++) {
        cardsMap[Five[i].getValue()]++;
        cardsMap[14 + Five[i].getSuit()]++;
    }
    makeBestHand(cardsMap, allTimeBestHand);
}

void generateHands(int k, const std::vector<Card>& Seven, int x[], int allTimeBestHand[]) {
    for(int i = x[k-1] + 1 ; i <= 7 ; ++ i)
    {
        x[k] = i;
        if(k == 5)
            createHandMap(x, Seven, allTimeBestHand);
        else
            generateHands(k + 1, Seven, x, allTimeBestHand);
    }
}

void determineBestHands(std::vector<Player> &playerVector, std::vector<Card> allPSeven)
{
    for (auto &player : playerVector) {
        for (int i = 0; i < 2; i++) {
            allPSeven.push_back(player.getHandI(i));
        }
        int x[6] = {0}, allTimeBestHand[4] = {0};
        generateHands(1, allPSeven, x, allTimeBestHand);
        player.setBestHand(allTimeBestHand);
        player.printBestHand();
        allPSeven.pop_back();
        allPSeven.pop_back();
    }
}

int main(){
    Deck deck; deck.shuffle();
    Player player1(deck), player2(deck), player3(deck), player4(deck);
    std::vector<Player> playerVector = {player1, player2, player3, player4};
    for (auto &player : playerVector) {player.printHand();}
    Table table(deck); table.printTable();
    std::vector<Card> allPSeven;
    for (int i = 0; i < 5; i++) {allPSeven.push_back(table.getHandI(i));}
    determineBestHands(playerVector, allPSeven);
}