# London : 

## What is London?
London is a single player game, written with inspiration from the actual boardgame [London](https://en.wikipedia.org/wiki/London_(board_game)).

## Executeable
London.exe takes one argument (int), which will decide the deck size. fx:

```
London.exe 30
```

## New game instance
```c++
 #include "london/london.hpp"
/**
 * 2018-07-11
 *
 * Setup:
 *  Create a deck of cards, equal to decksize
 *  Player initiate:
 *  1) Draw 6 cards from deck
 *  2) Gain 5 gold
 *  3) Gain 5 poverty
 * Gameplay:
 *  Take turn:
 *  1) Draw a card
 *  2) Choose an action:
 *      - (Build) Put one card at display, discard one of equal color & level
 *      - (Run city) All cards at display benefit to players gold deposit
 *      - (Take loan) Get 10 gold (You'll have to pay 15 back)
 *      - (Buy district) price 10 + pow(5, owned_district-1) gold
 *      - (End turn) you'll get an extra card
 *  3) Interest (you get interest from your gold deposit)
 */
 London::Game();
```

## Dependencies
> src\london\london.cpp

> src\london\london.hpp

> lib\filehandler\filehandler.cpp

> lib\filehandler\filehandler.h

> lib\logger\logger.cpp

> lib\logger\logger.h

> Simple\SimpleContaier.h

> Simple\SimpleMath.h

> Simple\SimpleSystem.h

> Simple\SimpleWindows.h

> SimpleJust.h