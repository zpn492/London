#ifndef __LONDON__
#define __LONDON__

#include "../../SimpleJust.h"

#include "../../lib/logger/logger.h"
#include "../../lib/filehandler/filehandler.h"


/**
 * LONDON consists of two namespaces, London & London::View.
 * In London you'll find som helper function and models for the game mechanics.
 * In London::View, you'll find I/O mechanics, listing for user input & printing states of the models.
 * Functions in London::View, ending at Loop contains listening functionality.
 * 
 * London
 *  Helper functions
 *  Class Game
 *  Class Card
 *  Class Player
 *  Class Loan
 * 
 * London::View
 *  View functions (without need of input)
 *  View functions (with need of input)
 */

namespace London
    {
    class Card;
    class Loan;
    class Player;
    
    /**
     * 2018-07-11
     * The actuacl game London.
     * 
     * Start a new game, by initializing a new object.
     * 
     * Setup:
     *  Create a deck of cards, equal to decksize
     *  Each enemy & player (there is only one):
     *  1) Draw 6 cards from deck
     *  2) Gain 5 gold
     *  3) Gain 5 poverty
     * Gameplay:
     *  Each enemey & player takes one turn at a time:
     *  1) Draw a card
     *  2) Choose an action:
     *      - (Build) Put one card at display, discard one of equal color & level
     *      - (Run city) All cards at display benefit to players gold deposit
     *      - (Take loan) Get 10 gold (You'll have to pay 15 back)
     *      - (Buy district) price 10 + pow(5, owned_district-1) gold
     *      - (End turn) you'll get an extra card
     *  3) Interest (you get interest from your gold deposit)
     */
    class Game
        {
        /**
         * This method is called from within the constructor
         * It waits for an user input &
         * runs the chosen action. 
         *
         * Actions is started with a call,
         * to London::View::__
         */
        void turn(Logger &log, Player &p);
        
        std::vector<Card> deck;
        std::vector<Player> players;

    public:
        /**
         * @enemies  (int)  # Number of enemies 
         * @decksize (int)  # Number of cards within this game
         */
        Game(int enemies=0, int decksize=30);
        };
    };

/**
 * LONDON
 */
namespace London
    {
    /**
     * Defines spaces between Card attributes when printed
     * Only used in London::View::__
     */
    #define SPACE_BETWEEN_CARD_INFO 15
    
    /**
     * Is used in London::createDeckOfBuildings(),
     * as parameter to the function London::csleep()
     */
    #define TIME_BEFORE_NEXT_RANDOM_NUMBER 4

    /**
     * Amount of gold, a player gets upon taking a loan
     */
    #define LOAN 10

    /**
     * Amount of gold, a player should pay in return for the loan 
     */
    #define LOAN_PAYBACK 15

    /**
     * Initial cost, used when buying a district
     * Modfier cost, used when buying a district
     * Poverty reduction, used when buying a district
     */
    #define DISTRICT_INITIAL_COST 10 
    #define DISTRICT_MODIFIER_COST 5
    #define DISTRICT_POVERTY_REDUCTION 5
    
    enum Land { SMALL };
    enum Color { RED, BROWN, YELLOW, GREEN };
    enum Level { A, B, C };

    /**
     * Exception, thrown if London::draw,
     * is called on an empty std::vector<Card>
     */
    class EmptyDeck: public std::exception 
        {
        virtual const char* what() const throw(); 
        };

    const EmptyDeck emptydeck;

    /**
     * this method is build upon usleep:
     * http://pubs.opengroup.org/onlinepubs/7908799/xsh/unistd.h.html
     *
     * System sleeps for 100000 microseconds * times
     */
    void csleep(int times=9);

    /**
     * Takes a string s,
     * prints characters equal to:
     * s + std::max((space - s.length);0)
     *
     * Ex. space(std::string("two"), 6)
     * Result: "two   "
     */
    void space(std::string s, int space);
    
    /**
     * Draw one card from deck
     * That card is removed from deck,
     * and returned as output.
     *
     * If draw is used on an empty list,
     * the exception: EmptyDeck is thrown
     */
    Card draw(std::vector<Card> &deck) throw();

    /**
     * Create a new Card,
     * where fields are set with help from
     * time.h & rand()
     *
     * http://www.cplusplus.com/reference/cstdlib/rand/
     */
    Card create(int ID);
    
    /**
     * A: Call London::create(ID)
     * B: Call London::csleep(x)
     * 
     * Loop over A & B until number of created cards,
     * equals input::size.
     */
    std::vector<Card> createDeckOfBuildings(int size);

/* CARD */
    class Card
        {
    public:
        Card(int ID) {this->ID = ID;};

        bool operator==(const Card &c)
            {
            return (this->color == c.color && this->level == c.level);
            }
        operator=(const Card &c)
            {
            this->color = c.color;
            this->level = c.level;
            this->ID = c.ID;
            this->initialcost = c.initialcost;
            this->activationcost = c.activationcost;
            this->activationbenefit = c.activationbenefit;
            };

        Color color;
        Level level;

        int initialcost;
        int activationcost;
        int activationbenefit;
        int ID; 
        };

/* PLAYER */
    class Player
        {
    public:
        Player(int pID) {this->pID = pID; poverty = 5; gold = 5;};

        /**
         * Searchs Player.hand[] for a card c,
         * where c.ID == input::ID
         * 
         * If one such exists, remove it from Player.hand[]
         */
        void handRemoveById(int ID);
        
        /**
         * Try to display c1,
         *
         * On success:
         *  - c2 is removed from game
         *  - Player.gold is reduced by c1.initialcost
         *  - Player.poverty is increased by c1.level
         * 
         * @c1 (Card)     # Card is moved from Player.hand[] to Player.display[]
         * @c2 (Card)     # Card is removed from Player.hand[]
         * 
         * @return (bool) # wether c1 was displayed 
         */
        bool buildCard(Card c1, Card c2);

        /**
         * Try to activate all cards i Player.display[]
         * On success
         *  - Player.gold += sum(Player.display[].activationBenefit)
         *  - Player.gold -= sum(Player.display[].activationCost)
         *  - Player.display[].clear()
         *
         * @return (bool) # wether cards was activatede
         */
        bool runCity();

        /**
         * Player.gold += London::LOAN;
         * Player.loans.push_back(Loan(London::LOAN_PAYBACK))
         */
        bool takeLoan();

        /**
         * Interest is calculated & added to Player.gold
         * Formular:
         *  A: Player.gold - London::LOAN*count(Player.loans[])
         *  B: 0.5 + (0.1*const(Player.districts[]) )
         *  C: (A) * ((B) / Player.poverty )
         *  D: std::max((C), 0 )
         *
         * @return (int) # amount which Player.gold got in interest
         */
        int interest();

        /**
         * Try to buy a district
         * Formula:
         *  DISTRICT_INITIAL_COST * pow(DISTRICT_MODIFIER_COST, count(Player.districts[])+1)
         * 
         * If bought:
         *  - A district is added to Player.districts[]
         *  - Player.poverty = std::max(Player.poverty-5, 0)
         *
         * @return (bool) # wether a district was bought or not
         */
        bool buyDistrict();

        /**
         * Contains all none displayed cards drawn by this player
         */ 
        std::vector<Card> hand;
        
        /**
         * Contains all displayed cards, which has yet to by activated
         */ 
        std::vector<Card> display;
 
        std::vector<Loan> loans;
        std::vector<Land> districts;
        
        /* Used by view functions
         * to tell cost & benefit of last runCity() */
        int lastSumCost;
        int lastSumBenefit;

        int poverty;
        int gold;
        int pID;
        };
    
/* LOAN */
    class Loan
        {       
    public:
        Loan() {};
        
        int getAmount() { return LOAN_PAYBACK; };
        };
    
    };    

/**
 * VIEW - LONDON
 *
 * London::View is a bunch of functions,
 * used to visualize model such as London::Player, London::Card etc.
 *
 * Some of the functions will allso be able to wait of user input.
 * Those functions is ending on Loop
 * fx buildLoop
 */
namespace London
    { 
    namespace View
        {
        struct ViewPoint { bool (*f)(Logger&, London::Player&); };        
        static std::map<char, ViewPoint> viewpoints;
        static std::map<char, ViewPoint>::iterator viewpointsIt;

/* PLAYER */
        bool deck(Logger &log, const std::vector<Card> &deck, 
            const std::string name);

        bool player(Logger &log, Player &p);

/* GAME */
        bool options(Logger &log, Player &p);

        bool takeLoan(Logger &log, Player &p);

        bool build(Logger &log, Player &p, Card &c1, Card &c2);

        bool buildLoop(Logger &log, Player &p);

        bool turnLoop(Logger &log, Player &p, std::vector<Card> &deck);

        bool runCity(Logger &log, Player &p);
        
        bool district(Logger &log, Player &p);

        bool gameEnd(Logger &log);
        };
    };
#endif