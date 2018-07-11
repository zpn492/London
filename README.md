# London
```c++
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
```

 ```c++
// New game is initiated
London::Game();
```