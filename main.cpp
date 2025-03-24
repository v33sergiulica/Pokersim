#include <iostream>
#include <algorithm>
#include <array>
#include <random>
#include <vector>
#include <tuple>
#include <map>
#include <set>
/*                                                 0          1          2        3           4      */
const std::array<std::string,5> suitfy = {"Hearts", "Diamonds", "Clubs", "Spades", "all Suits"};
const std::array<std::string,14> valuefy = {"Joker", "Ace", "2", "3", "4", "5", "6", "7", "8", "9", "10", "Jack", "Queen", "King"};
/*                                             unused0       1     2    3    4    5    6    7    8    9    10     11      12       13   */

enum HandRank {
    HIGH_CARD, ONE_PAIR, TWO_PAIR, THREE_OF_A_KIND, STRAIGHT,
    FLUSH, FULL_HOUSE, FOUR_OF_A_KIND, STRAIGHT_FLUSH, ROYAL_FLUSH
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
    // 0 -> Top of the deck
    // Using a card puts it at the bottom of the deck
public:
    void shuffle() {
        static std::random_device rd;
        static std::mt19937 g(rd());
        std::shuffle(std::begin(cards), std::end(cards), g);
        std::cout << "Shuffled" << std::endl;
    };
   /* void print() {
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
    const Card* getHand() const { return hand; }
    void printHand() {
        std::cout << "First card is a " << valuefy[hand[0].getValue()] << " of " << suitfy[hand[0].getSuit()] << std::endl;
        std::cout << "Second card is a " << valuefy[hand[1].getValue()] << " of " << suitfy[hand[1].getSuit()] << std::endl;
    }
};

class Table {
    Card tableHand[5];
    bool betFaze;
    bool flop;
    bool turn;
    bool river;
public:
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
    const Card* getTableHand() const { return tableHand; }
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
    ~Table() = default;
    void printTable() {
        if (betFaze) { std::cout << "No cards on table" << std::endl; }
        else if (flop) {
            std::cout << "First table card is a " << valuefy[tableHand[0].getValue()] << " of " << suitfy[tableHand[0].getSuit()] << std::endl;
            std::cout << "Second table card is a " << valuefy[tableHand[1].getValue()] << " of " << suitfy[tableHand[1].getSuit()] << std::endl;
            std::cout << "Third table card is a " << valuefy[tableHand[2].getValue()] << " of " << suitfy[tableHand[2].getSuit()] << std::endl;
        }
        else if (turn) {
            std::cout << "First table card is a " << valuefy[tableHand[0].getValue()] << " of " << suitfy[tableHand[0].getSuit()] << std::endl;
            std::cout << "Second table card is a " << valuefy[tableHand[1].getValue()] << " of " << suitfy[tableHand[1].getSuit()] << std::endl;
            std::cout << "Third table card is a " << valuefy[tableHand[2].getValue()] << " of " << suitfy[tableHand[2].getSuit()] << std::endl;
            std::cout << "Fourth table card is a " << valuefy[tableHand[3].getValue()] << " of " << suitfy[tableHand[3].getSuit()] << std::endl;
        }
        else {
            std::cout << "First table card is a " << valuefy[tableHand[0].getValue()] << " of " << suitfy[tableHand[0].getSuit()] << std::endl;
            std::cout << "Second table card is a " << valuefy[tableHand[1].getValue()] << " of " << suitfy[tableHand[1].getSuit()] << std::endl;
            std::cout << "Third table card is a " << valuefy[tableHand[2].getValue()] << " of " << suitfy[tableHand[2].getSuit()] << std::endl;
            std::cout << "Fourth table card is a " << valuefy[tableHand[3].getValue()] << " of " << suitfy[tableHand[3].getSuit()] << std::endl;
            std::cout << "Fifth table card is a " << valuefy[tableHand[4].getValue()] << " of " << suitfy[tableHand[4].getSuit()] << std::endl;
        }
    }

};

struct HandEvaluation {
    int rank;
    std::vector<int> tiebreakers;

    // Correct comparison: FIRST check rank, THEN tiebreakers
    bool operator>(const HandEvaluation &other) const {
        if (rank != other.rank) {
            return rank > other.rank; // Higher rank wins
        }
        // If ranks are equal, compare tiebreakers one by one
        for (size_t i = 0; i < tiebreakers.size(); ++i) {
            if (tiebreakers[i] != other.tiebreakers[i]) {
                return tiebreakers[i] > other.tiebreakers[i];
            }
        }
        return false; // Hands are equal
    }
};

HandEvaluation evaluateHand(const std::vector<Card>& cards) {
    std::map<int, int> valueCount;
    std::map<int, std::vector<int>> suitValues;
    std::vector<int> sortedValues;

    for (const auto &card : cards) {
        valueCount[card.getValue()]++;
        suitValues[card.getSuit()].push_back(card.getValue());
        sortedValues.push_back(card.getValue());
    }
    std::sort(sortedValues.rbegin(), sortedValues.rend()); // Sort high to low

    // --- Flush Detection ---
    bool isFlush = false;
    std::vector<int> flushCards;
    for (const auto &[suit, values] : suitValues) {
        if (values.size() >= 5) {
            isFlush = true;
            flushCards = values;
            std::sort(flushCards.rbegin(), flushCards.rend());
            flushCards.resize(5);
            break;
        }
    }

    // --- Straight Detection (including Ace-low) ---
    bool isStraight = false;
    std::vector<int> straightCards;
    std::set<int> uniqueValues(sortedValues.begin(), sortedValues.end());
    // Check normal straights (e.g., 5-6-7-8-9)
    for (auto it = uniqueValues.rbegin(); it != uniqueValues.rend(); ++it) {
        if (uniqueValues.count(*it - 1) && uniqueValues.count(*it - 2) &&
            uniqueValues.count(*it - 3) && uniqueValues.count(*it - 4)) {
            isStraight = true;
            straightCards = {*it, *it - 1, *it - 2, *it - 3, *it - 4};
            break;
        }
    }
    // Check Ace-low straight (A-2-3-4-5)
    if (!isStraight && uniqueValues.count(14) && uniqueValues.count(2) &&
        uniqueValues.count(3) && uniqueValues.count(4) && uniqueValues.count(5)) {
        isStraight = true;
        straightCards = {5, 4, 3, 2, 1}; // Ace as low
    }

    // --- Straight Flush / Royal Flush ---
    if (isFlush && isStraight) {
        bool isRoyal = (flushCards[0] == 14 && flushCards[4] == 10);
        if (isRoyal) return {9, flushCards}; // Royal Flush
        return {8, flushCards};             // Straight Flush
    }

    // --- Four of a Kind ---
    for (const auto &[val, count] : valueCount) {
        if (count == 4) {
            int kicker = (val == sortedValues[0]) ? sortedValues[4] : sortedValues[0];
            return {7, {val, kicker}};
        }
    }

    // --- Full House ---
    int trio = -1, pair = -1;
    for (const auto &[val, count] : valueCount) {
        if (count == 3 && val > trio) trio = val;
        else if (count >= 2 && val > pair) pair = val;
    }
    if (trio != -1 && pair != -1) return {6, {trio, pair}};

    // --- Flush ---
    if (isFlush) return {5, flushCards};

    // --- Straight ---
    if (isStraight) return {4, straightCards};

    // --- Three of a Kind ---
    if (trio != -1) {
        std::vector<int> kickers;
        for (int val : sortedValues) {
            if (val != trio) kickers.push_back(val);
            if (kickers.size() == 2) break; // Need 2 kickers
        }
        return {3, {trio, kickers[0], kickers[1]}};
    }

    // --- Two Pair ---
    std::vector<int> pairs;
    for (const auto &[val, count] : valueCount) {
        if (count == 2) pairs.push_back(val);
    }
    if (pairs.size() >= 2) {
        std::sort(pairs.rbegin(), pairs.rend());
        int kicker = -1;
        for (int val : sortedValues) {
            if (val != pairs[0] && val != pairs[1]) {
                kicker = val;
                break;
            }
        }
        return {2, {pairs[0], pairs[1], kicker}}; // [high_pair, low_pair, kicker]
    }

    // --- One Pair ---
    if (pairs.size() == 1) {
        std::vector<int> kickers;
        for (int val : sortedValues) {
            if (val != pairs[0]) {
                kickers.push_back(val);
                if (kickers.size() == 3) break; // Need 3 kickers
            }
        }
        return {1, {pairs[0], kickers[0], kickers[1], kickers[2]}};
    }

    // --- High Card ---
    return {0, {sortedValues[0], sortedValues[1], sortedValues[2], sortedValues[3], sortedValues[4]}};
}

Player* determineWinner(std::vector<Player> &players, const Table &table) {
    if (players.size() < 2) return players.empty() ? nullptr : &players[0];

    Player* bestPlayer = &players[0];
    auto bestHand = {bestPlayer->getHand()[0], bestPlayer->getHand()[1],
                   table.getTableHand()[0], table.getTableHand()[1],
                   table.getTableHand()[2], table.getTableHand()[3],
                   table.getTableHand()[4]};
    HandEvaluation bestEval = evaluateHand(bestHand);

    std::cout << "\n=== Player 1 ===\n";
    bestPlayer->printHand();
    std::cout << "Rank: " << bestEval.rank << " (";
    const char* ranks[] = {"High","Pair","Two Pair","Three Kind","Straight",
                          "Flush","Full House","Four Kind","Straight Flush","Royal Flush"};
    std::cout << ranks[bestEval.rank] << ")\nTiebreakers: ";
    for (int v : bestEval.tiebreakers) std::cout << v << " ";
    std::cout << "\n";

    for (size_t i = 1; i < players.size(); ++i) {
        auto currentHand = {players[i].getHand()[0], players[i].getHand()[1],
                           table.getTableHand()[0], table.getTableHand()[1],
                           table.getTableHand()[2], table.getTableHand()[3],
                           table.getTableHand()[4]};
        HandEvaluation currentEval = evaluateHand(currentHand);

        std::cout << "\n=== Player " << i+1 << " ===\n";
        players[i].printHand();
        std::cout << "Rank: " << currentEval.rank << " (" << ranks[currentEval.rank] << ")\n";
        std::cout << "Tiebreakers: ";
        for (int v : currentEval.tiebreakers) std::cout << v << " ";
        std::cout << "\n";

        if (currentEval > bestEval) {
            bestPlayer = &players[i];
            bestEval = currentEval;
            std::cout << "--> New leader!\n";
        }
    }

    std::cout << "\n==== FINAL RESULT ====\n";
    std::cout << "Winner: Player " << (bestPlayer == &players[0] ? 1 : 2) << "\n";
    std::cout << "Winning Hand:\n";
    bestPlayer->printHand();
    std::cout << "Rank: " << bestEval.rank << " (" << ranks[bestEval.rank] << ")\n";
    std::cout << "Tiebreakers: ";
    for (int v : bestEval.tiebreakers) std::cout << v << " ";
    std::cout << "\n=====================\n";

    return bestPlayer;
}

int main() {
    Deck deck;
    deck.shuffle();
    Table table(deck);

    // Initialize 4 players
    std::vector<Player> players;
    for (int i = 0; i < 4; ++i) {
        players.emplace_back(deck);
    }

    // Print all hands
    for (size_t i = 0; i < players.size(); ++i) {
        std::cout << "Player " << i+1 << ":\n";
        players[i].printHand();
    }
    table.printTable();

    // Find winner
    Player* winner = determineWinner(players, table);

    // Identify winner index
    auto winnerIt = std::find_if(players.begin(), players.end(),
        [&](const Player& p) { return &p == winner; });

    if (winnerIt != players.end()) {
        int winnerNum = winnerIt - players.begin() + 1;
        std::cout << "\nPlayer " << winnerNum << " wins!\n";
    } else {
        std::cerr << "Error: Winner not found!\n";
    }

    return 0;
}
