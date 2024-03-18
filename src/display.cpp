#include "display.hpp"

/*----------------------------------------------------------------------------------------------------*/

// The name the window will have
#define gameName "2D_Race"

/*----------------------------------------------------------------------------------------------------*/

int play(bool singlePlayer, uint8_t difficulty)
{
    // the statut that will be returned. 0 corresponds to a normal exit
    int statut = 0;

    /* #region Window initialization */
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), gameName, sf::Style::Fullscreen);
    window.setVerticalSyncEnabled(true);
    sf::View mainView(window.getDefaultView());
    window.setView(mainView);
    /* #endregion */

    /* #region  Game initialization*/
    Game game(&window, singlePlayer, difficulty);
    /* #endregion */

    while (window.isOpen())
    {
        if (!game.getPaused())
        {
            game.update();
            if (game.updateTile())
            {
                game.tileChange();
            }
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

            // on another window
            case sf::Event::LostFocus:
                game.pause();
                break;

            // back on the game's window
            case sf::Event::GainedFocus:
                game.resume();
                break;

            case sf::Event::KeyPressed:
                switch (event.key.code)
                {
                // press escape to force exit the game
                case sf::Keyboard::Escape:
                    statut = -1;
                    std::cout << "manual interruption of game\n";
                    goto Quit;

                // press P to Pause
                case sf::Keyboard::P:
                    game.switchPause();
                    break;

                // press Enter to restart
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
        
        // drawing everything
        window.clear(sf::Color::White);
        window.draw(game.tile1);
        window.draw(game.tile2);
        window.draw(game.tile3);
        window.draw(game.p1);
        window.draw(game.p2);
        window.draw(game.hpBar1);
        window.draw(game.hpBar2);
        window.draw(game.score);
        if (game.explosion.is_displayed)
        {
            window.draw(game.explosion);
        }
        for (const auto& entity : game.entities)
        {
            window.draw(*entity);
        }

        window.display();
    }

Quit:
    return statut;
}
