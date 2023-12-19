#include "display.hpp"

/*----------------------------------------------------------------------------------------------------*/

#define gameName "2D_Race"

/*----------------------------------------------------------------------------------------------------*/

int play(bool singlePlayer)
{
    int statut = 0;

    /* #region Window initialization */
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), gameName, sf::Style::Fullscreen);
    window.setVerticalSyncEnabled(true);
    sf::View mainView(window.getDefaultView());
    window.setView(mainView);
    /* #endregion */

    /* #region  Game initialization*/
    Game game(window.getSize());
    /* #endregion */

    while (window.isOpen())
    {
        game.update();
        mainView.move(0, -game.getSpeed());
        window.setView(mainView);

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

                        default:
                            break;
                    }
                    break;

                // other types of events not processed
                default:
                    break;
            }
        }
        window.clear(sf::Color::White);

        window.draw(game.p1);
        window.draw(game.p2);

        
        window.display();
    }

Quit:

    return statut;
}