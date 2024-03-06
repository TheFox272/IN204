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
    Game game(&window);
    /* #endregion */

    while (window.isOpen())
    {
        if (!game.getPaused())
        {
            game.update();
            mainView.move(0, -game.getSpeed());
            window.setView(mainView);
        }
        else
        {
            game.updatePause();
        }

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

                case sf::Keyboard::P:
                    game.switchPause();
                    break;

                case sf::Keyboard::Enter:
                    statut = 1;
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

        window.draw(game.tile1);
        window.draw(game.tile2);
        window.draw(game.tile3);
        window.draw(game.p1);
        window.draw(game.p2);
        window.draw(game.score);
        if (game.explosion.is_displayed)
        {
            window.draw(game.explosion);
        }

        window.display();
    }

Quit:
    return statut;
}
