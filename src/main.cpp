#include "main.hpp"

/*----------------------------------------------------------------------------------------------------*/
#define RED "\033[1;31m"
#define RESET "\033[0m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define BLUE "\033[1;34m"
#define PURPLE "\033[1;35m"
#define CYAN "\033[1;36m"
/*----------------------------------------------------------------------------------------------------*/

int main(int argc, char** argv){
    int status = 1;
    int nb_players = 0;
    uint8_t difficulty = 0;
    if (argc == 1)
    {
        std::cout << "error: please give the number a players to main\n";
        std::cout << "-> " YELLOW "main 1 difficulty" RESET " to play solo\n";
        std::cout << "-> " YELLOW "main 2 difficulty" RESET " to play duo\n";
        status = -1;
    }
    else if (argc >= 2)
    {
        if (std::string(argv[1]) == "1")
        {
            nb_players = 1;
            std::cout << PURPLE "Playing solo " RESET "\n";
        }
        else if (std::string(argv[1]) == "2")
        {
            nb_players = 2;
            std::cout << PURPLE "Playing duo " RESET "\n";
        }
        else
        {
            std::cout << "error: please give a valid number a players to main\n";
            std::cout << "-> " YELLOW "main 1 args" RESET " to play solo\n";
            std::cout << "-> " YELLOW "main 2 args" RESET " to play duo\n";
            status = -1;
        }
        if (argc >= 3)
        {
            if (std::string(argv[2]) == "1")
            {
                difficulty = 1;
            }
            else if (std::string(argv[2]) == "2")
            {
                difficulty = 2;
            }
            else if (std::string(argv[2]) == "3")
            {
                difficulty = 3;
            }
            else
            {
                std::cout << "error: please give a valid difficulty to main\n";
                std::cout << "-> " YELLOW "main nb_players 1" RESET " to play solo in easy mode\n";
                std::cout << "-> " YELLOW "main nb_players 2" RESET " to play solo in average mode\n";
                std::cout << "-> " YELLOW "main nb_players 3" RESET " to play solo in hard mode\n";
                status = -1;
            }
        }
        else
        {
            difficulty = 1;
        }
    }

    if (difficulty == 1)
    {
        std::cout << PURPLE "Playing in easy mode " RESET "\n";
    }
    else if (difficulty == 2)
    {
        std::cout << PURPLE "Playing in average mode " RESET "\n";
    }
    else if (difficulty == 3)
    {
        std::cout << PURPLE "Playing in hard mode " RESET "\n";
    }

    while (status == 1)
    {
        status = play(nb_players == 1, difficulty);
    }
    
    return status;
}
