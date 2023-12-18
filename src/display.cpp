#include "display.hpp"

/*----------------------------------------------------------------------------------------------------*/

#define gameName "2D_Race"

/*----------------------------------------------------------------------------------------------------*/

int play(Game game, bool aiPlayer)
{
    int statut;

    /* #region Window initialization */
    sf::RenderWindow window(sf::VideoMode(800, 600), gameName, sf::Style::Titlebar | sf::Style::Close);
    window.setVerticalSyncEnabled(true);
    // window.setKeyRepeatEnabled(false);
    /* #endregion */


    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {   
                switch (event.type)
                {
                    // window closed
                    case sf::Event::Closed:
                        window.close();
                        break;

                    case sf::Event::LostFocus:
                        game.pause();
                        break;

                    case sf::Event::GainedFocus:
                        game.resume();
                        break;

                    case sf::Event::KeyPressed:
                        switch (event.key.code)
                        {
                            case sf::Keyboard::Escape:
                                statut = -1;
                                std::cout << "manual interruption of game\n";
                                goto Quit;
                            
                            case sf::Keyboard::Q:
                                game.movesLeft(1);
                                break;
                            
                            case sf::Keyboard::D:
                                game.movesRight(1);
                                break;
                            
                            case sf::Keyboard::Left:
                                game.movesLeft(2);
                                break;
                            
                            case sf::Keyboard::Right:
                                game.movesRight(2);
                                break;
                            
                            default:
                                break;
                        }
                        break;
                    
                    case sf::Event::KeyReleased:
                        switch (event.key.code)
                        {
                            case sf::Keyboard::Q:
                                game.stops(1);
                                break;
                            
                            case sf::Keyboard::D:
                                game.stops(1);
                                break;
                            
                            case sf::Keyboard::Left:
                                game.stops(2);
                                break;
                            
                            case sf::Keyboard::Right:
                                game.stops(2);
                                break;
                            
                            default:
                                break;
                        }
                        break;


                    // other types of events not processed
                    default:
                        break;
                }
        }
        window.clear(sf::Color::Black);





        window.display();
    }

Quit:


    return statut;
}