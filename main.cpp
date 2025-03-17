#include <iostream>
#include <algorithm>
/*                          0          1          2        3           4      */
std::string suitfy[5] = {"Hearts", "Diamonds", "Clubs", "Spades", "all Suits"};
std::string valuefy[14] = {"Joker", "Ace", "2", "3", "4", "5", "6", "7", "8", "9", "10", "Jack", "Queen", "King"};
/*                      unused0       1     2    3    4    5    6    7    8    9    10     11      12       13   */
class Card {
    int value;
    int suit;
public:
    int getValue() {return value;}
    int getSuit() {return suit;}
    Card() {value = 0; suit = 4;}
    Card(int v, int s) {value = v; suit = s;}
    };

class Deck {
    Card cards[52];
    // 0 -> Top of the deck
    // Using a card puts it at the bottom of the deck
public:
    void shuffle() {
        std::random_shuffle(std::begin(cards), std::end(cards));
        std::cout << "Shuffled" << std::endl;
    };
    void print() {
        for (int i = 0; i < 52; i++) {
            std::cout << valuefy[cards[i].getValue()] << " of " << suitfy[cards[i].getSuit()] << std::endl;
        }
    }
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
        isPlaying = true;
        itsTurn = false;
    }
    void printHand() {
        std::cout << "First card is a " << valuefy[hand[0].getValue()] << " of " << suitfy[hand[0].getSuit()] << std::endl;
        std::cout << "Second card is a " << valuefy[hand[1].getValue()] << " of " << suitfy[hand[1].getSuit()] << std::endl;
    }
};

int main() {
    Deck deck;
    deck.shuffle();
    Player player1(deck);
    player1.printHand();
    Player player2(deck);
    player2.printHand();
}