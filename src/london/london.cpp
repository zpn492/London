#include "london.hpp"

/* ********************************* */
/* LONDON */
/* ********************************* */
namespace London
    {
inline const char* EmptyDeck::what() const throw()
        {
        std::string s = "blocks.cpp, draw(std::vector<deck> &deck), ";
        s += "is not allowed on empty decks";
        return  s.c_str();
        };

/* ********************************* */
/* NAMESPACE FUNCTIONS */
/* ********************************* */
    void csleep(int times)
        {
        for(int i = 0; i < times; i++)
            usleep(100000);
        };

    void space(std::string s, int space)
        {
        std::cout << filehandler::space(s,space).c_str();
        };
    
    Card draw(std::vector<Card> &deck) throw()
        {
        try { if(deck.empty()) throw emptydeck; } 
            catch(std::exception& e) {std::cout << e.what() << std::endl; }

        int r;
        srand(time(NULL));
        r = rand() % deck.size();

        Card c = deck[r];
        std::vector<Card> cc;
        for(int i = 0; i < deck.size(); i++)
            if(deck[i].ID != c.ID)
                cc.push_back(deck[i]);
        deck = cc;
        return c;
        };

    Card create(int ID)
        {
        int r;
        Card c(ID);

        srand(time(NULL));
        r = rand() % 4;
        c.color = (Color) r;
        c.initialcost = rand() % 10;
        c.activationcost = rand() % 5;
        c.activationbenefit = rand() % 10 + 
            c.initialcost + c.activationcost;

        if(c.activationbenefit < 5)
            c.level = Level::A;
        else if(c.activationbenefit > 4 && c.activationbenefit < 10)
            c.level = Level::B;
        else
            c.level = Level::C;

        return c;
        };

    std::vector<Card> createDeckOfBuildings(int size)
        {
        std::vector<Card> cards;
        Logger log;
        std::string s;
        int c = 0;
        for(int i = 0; i < size; i++)
            {
            s = "Creating deck: " + filehandler::int_to_string(i+1) + 
                " out of " + filehandler::int_to_string(size);
            log.write_inline(s);
            cards.push_back(create(i+1));
            csleep(TIME_BEFORE_NEXT_RANDOM_NUMBER);
            }
        std::cout << "" << std::endl;
        return cards;
        };
    
/* ********************************* */
/* PLAYER */
/* ********************************* */
    void Player::handRemoveById(int ID)
        {
        std::vector<Card> c;
        for(int i = 0; i < hand.size(); i++)
            if(hand[i].ID != ID)
                c.push_back(hand[i]);
        this->hand = c;
        };

    bool Player::buildCard(Card c1, Card c2)
        {
        if(c1 == c2 && gold >= c1.initialcost)
            {
            gold -= c1.initialcost;
            display.push_back(c1);
            handRemoveById(c1.ID);
            handRemoveById(c2.ID);
            return TRUE;
            }
        return FALSE;
        };

    bool Player::runCity()
        {
        int tpoverty = 0; lastSumCost = 0; lastSumBenefit = 0;
        for(int i = 0; i < display.size(); i++)
            {
            lastSumCost += display[i].activationcost;
            lastSumBenefit += display[i].activationbenefit;
            tpoverty += display[i].level;
            }
        if(gold >= lastSumCost)
            {
            gold += lastSumBenefit - lastSumCost;
            poverty += tpoverty;
            display.clear();
            return TRUE;
            }
        return FALSE;
        };
    
    bool Player::takeLoan()
        {
        loans.push_back(Loan());
        gold += LOAN;
        return FALSE;
        };

    int Player::interest()
        {
        int step1 = gold - 10*(int)loans.size();
        double step2 = 0.5 + (0.1*(double)districts.size());
        double step3 = step2 / (double)poverty;
        double step4 = (double)step1 * step3;
        int ints = std::max((int)floor(step4), 0);
        
        gold += ints;
        return ints;
        };

    bool Player::buyDistrict()
        {
        int cost = DISTRICT_INITIAL_COST + pow(DISTRICT_MODIFIER_COST, districts.size()+1);
        if(gold >= cost)
            {
            gold -= cost;
            poverty = std::max(0, poverty - DISTRICT_POVERTY_REDUCTION);
            districts.push_back(Land(SMALL));
            return TRUE;
            }
        return FALSE;
        }
    
/* ********************************* */
/* GAME */
/* ********************************* */
    Game::Game(int enemies, int decksize)  
        {
        // Set ViewPoints
        View::ViewPoint vp1; vp1.f = &View::buildLoop;
        View::ViewPoint vp2; vp2.f = &View::runCity;
        View::ViewPoint vp3; vp3.f = &View::takeLoan;
        View::ViewPoint vp4; vp4.f = &View::player;
        View::ViewPoint vp5; vp5.f = &View::district;
        
        View::viewpoints.insert ( std::pair<char,View::ViewPoint>('b',vp1) );
        View::viewpoints.insert ( std::pair<char,View::ViewPoint>('r',vp2) );
        View::viewpoints.insert ( std::pair<char,View::ViewPoint>('t',vp3) );
        View::viewpoints.insert ( std::pair<char,View::ViewPoint>('v',vp4) );
        View::viewpoints.insert ( std::pair<char,View::ViewPoint>('d',vp5) );

        // Initiate game
        Logger log;
        deck = createDeckOfBuildings(decksize);
        for(int i = 0; i < enemies + 1; i++)
            {
            Player p(i);
            for(int j = 0; j < 6; j++)
                {
                p.hand.push_back(draw(deck));
                }
            players.push_back(p);
            }
        while(true)
            {
            if(deck.empty())
                break;
            turn(log, players[0]);
            }
        system("CLS");
        View::gameEnd(log);
        };
    
    void Game::turn(Logger &log, Player &p)
        {       
        p.hand.push_back(draw(deck));

        View::turnLoop(log, p, deck);
        };
    };

/* ********************************* */
/* VIEW - LONDON */
/* ********************************* */
namespace London
    {
    namespace View
        {
/* ********************************* */
/* VIEW - PLAYER */
/* ********************************* */
        bool deck(Logger &log, const std::vector<Card> &deck, 
            const std::string name)
            {
            int sp = SPACE_BETWEEN_CARD_INFO;

            if(!deck.empty())
                {
                space(std::string("Nr."), sp);
                space(std::string("ID"), sp);
                space(std::string("Where"),sp);
                space(std::string("Color"), sp);
                space(std::string("Level"), sp);
                space(std::string("Initialcost"), sp);
                space(std::string("Activationcost"), sp);
                space(std::string("Benefit"), sp);

                std::cout << "" << std::endl;
                }

            for(int i = 0; i < deck.size(); i++)
                {
                if(i % 2 == 0)
                    log.setColor(BGREY);
                space(filehandler::int_to_string(i), sp);
                space(filehandler::int_to_string(deck[i].ID), sp);
                space(name, sp);
                space(std::string(
                    filehandler::int_to_string(deck[i].color)), sp);
                space(std::string(
                    filehandler::int_to_string(deck[i].level)), sp);
                space(std::string(
                    filehandler::int_to_string(deck[i].initialcost)), sp);
                space(std::string(
                    filehandler::int_to_string(deck[i].activationcost)), sp);
                space(std::string(
                    filehandler::int_to_string(deck[i].activationbenefit)),sp);

                if(i % 2 == 0)
                    log.resetColor();

                std::cout << "" << std::endl;
                }
            return TRUE;
            };

        bool player(Logger &log, Player &p)
            {
            int sp = SPACE_BETWEEN_CARD_INFO;

            std::cout << "# ################################### #" << std::endl;
            std::cout << "# Player " << p.pID << " #" << std::endl;
            std::cout << "# ################################### #" << std::endl;
            
            int c = 0;
            for(int i = 0; i < p.loans.size(); i++) 
                c += p.loans[i].getAmount();

            space(std::string("# Gold: "), 37-filehandler::int_to_string(p.gold).length()); space(filehandler::int_to_string(p.gold) + std::string(" #\n"), 0);
            space(std::string("# Gold in none paid loans: "), 
                37-filehandler::int_to_string(c).length());
            
            space(filehandler::int_to_string(c) + std::string(" #\n"), 0);
            space(std::string("# Poverty: "), 
                37-filehandler::int_to_string(p.poverty).length()); 
            space(filehandler::int_to_string(p.poverty) + std::string(" #\n"), 0);

            space(std::string("# Districts: "), 
                37-filehandler::int_to_string(p.districts.size()).length()); 
            space(filehandler::int_to_string(p.districts.size()) + std::string(" #\n"), 0);
            
            space(std::string("# "), 37); 
            space(std::string(" #\n"), 0);

            deck(log, p.hand, std::string("Hand"));
            deck(log, p.display, std::string("Display"));
            return FALSE;
            };

/* ********************************* */
/* VIEW - GAME */
/* ********************************* */
        bool options(Logger &log, Player &p)
            {
            std::cout << "# ################################### #" << std::endl;
            std::cout << "# Options #" << std::endl;
            std::cout << "# ################################### #" << std::endl;
            std::cout << "# b) Build #" << std::endl;
            std::cout << "# r) Run city #" << std::endl;
            std::cout << "# t) Take loan (10 gold) #" << std::endl;
            std::cout << "# v) View cards #" << std::endl;
            std::cout << "# d) Buy district (15 gold) #" << std::endl;
            std::cout << "# e) End turn #" << std::endl;
            std::cout << "# q) Quit game #" << std::endl;
            std::cout << "# ################################### #" << std::endl;
            return TRUE;
            };
    
        bool turnLoop(Logger &log, Player &p, std::vector<Card> &deck)
            {
            std::string s = "";
            std::cout << "Card was drawn, remaing in deck: " << 
                filehandler::int_to_string(deck.size()) << std::endl;
            
            while(TRUE)
                {
                viewpointsIt = viewpoints.find(s.c_str()[0]);

                if(s.c_str()[0] == 'e')
                    break;
                if(s.c_str()[0] == 'q')
                    exit(0);
                else if(viewpointsIt != viewpoints.end())
                    {
                    system("CLS");
                    bool b = FALSE;
                    b = viewpointsIt->second.f(log, p);
                    if(b) break;
                    }
                else
                    options(log, p);
                
                s == "";
                getline(std::cin, s);                
                }

            log.setColor(FYELLOW);
            std::cout << "End of turn, press any key" << std::endl;
            log.resetColor();

            getline(std::cin, s);

            system("CLS");

            log.setColor(FGREEN);
            std::cout << "You earned: " << p.interest();
            std::cout << " due to interest of your gold deposit" << std::endl;
            log.resetColor();

            return TRUE;
            };
    
        bool buildLoop(Logger &log, Player &p)
            {
            std::vector<std::string> tokens;
            bool success = FALSE;
            std::string s;
            
            while(true)
                {
                std::cout << "# ################################### #" << std::endl;
                std::cout << "# Chose two cards, fx 2;12 for card number 2 and 12 #";
                std::cout << "" << std::endl;
                std::cout << "# First card is played, second is discarded #" << std::endl;
                std::cout << "# Type q to return to options #" << std::endl;
                std::cout << "# ################################### #" << std::endl;
                player(log, p);
                
                getline(std::cin, s);

                if(s == "q")
                    return success;

                tokens = filehandler::split_string(s, ';');

                if(tokens.size() < 2)
                    {
                    std::cout << "Couldn't understand input" << std::endl;
                    }
                else
                    {                
                    system("CLS");
                    Card c1 = p.hand[atoi(tokens[0].c_str())];
                    Card c2 = p.hand[atoi(tokens[1].c_str())];
                    if(build(log, p, c1, c2))
                        success = TRUE;
                    }
                }
            };

        bool build(Logger &log, Player &p, Card &c1, Card &c2)
            {
            if(c1 == c2)
                {
                if(p.buildCard(c1, c2))
                    {
                    log.setColor(FGREEN);
                    std::cout << "Build complete, you payed: " << c1.initialcost;
                    std::cout << " gold" << std::endl;
                    log.resetColor();
                    return TRUE;
                    }
                else
                    {
                    log.setColor(FRED);
                    std::cout << "You don't have enough money to play that card" << std::endl;
                    log.resetColor();
                    }
                }
            else
                {
                log.setColor(FRED);
                std::cout << "Cards must be of same color and level" << std::endl;
                log.resetColor();
                }
            return FALSE;
            };

        bool takeLoan(Logger &log, Player &p)
            {
            p.takeLoan();
            log.setColor(FGREEN);
            std::cout << "You took a loan on 10 gold, in return you'll have to return 15 gold"  << std::endl;
            log.resetColor();
            return TRUE;
            };

        bool runCity(Logger &log, Player &p)
            {
            if(p.runCity())
                {
                log.setColor(FGREEN);
                std::cout << "City run complete, you payed: " << p.lastSumCost;
                std::cout << ", and earned: " << p.lastSumBenefit << std::endl;
                std::cout << "Your city now have " << p.poverty << " poverty" << std::endl;
                log.resetColor();
                return TRUE;
                }
            else
                {
                log.setColor(FRED);
                std::cout << "You have " << p.gold << ", but it cost " << p.lastSumCost;
                std::cout << " to run the city" << std::endl;
                log.resetColor();
                return FALSE;
                }
            };
    
        bool district(Logger &log, Player &p)
            {
            if(p.buyDistrict())
                {
                log.setColor(FGREEN);
                std::cout << "You bought a new district for: ";
                std::cout << (10 + pow(5, p.districts.size())) << " gold" << std::endl;
                std::cout << "Poverty was reduced with 5" << std::endl;
                log.resetColor();
                return TRUE;
                }
            else
                {
                log.setColor(FRED);
                std::cout << "You can't afford a new district, it will cost you: ";
                std::cout << (10 + pow(5, p.districts.size())) << " gold" << std::endl;
                log.resetColor();
                return FALSE;
                }
            };

        bool gameEnd(Logger &log)
            {
            log.setColor(BGREEN);
            std::cout << "Congratulation, the game has ended";
            log.resetColor();
            std::cout << "" << std::endl;
            return TRUE;
            };
        };
    };