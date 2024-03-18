#pragma once

#include <iostream>
#include <cstdlib>

#include <SFML/Graphics.hpp>

#include "game.hpp"
#include "effects.hpp"

/*----------------------------------------------------------------------------------------------------*/
/**
 * @brief This function is the main gaming loop.
 * 
 * This function is called from the main. 
 * If the returned value is not-equal to 1, it is used to quit the main gaming loop.
 * 
 * @param singlePlayer Has to be False. This indicates 2 players mode.
 * For more fun, the game is designed to be played by two people on the same keyboard as cars can interact.
 * This parameter is let for developpers who may create another solo versions of the game.
 * @return The status code indicating the result of the game. 0 corresponds to a normal exit. 1 corresponds to an asked restart.
 * 
 */
int play(bool, uint8_t);
