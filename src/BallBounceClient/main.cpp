#include <iostream>
#include "main.h"
#include "SFML/Main.hpp"
#include "Network/UDPSocket.h"
#include "Game.h"
#include "Network/Server.h"

int main()
{

    UDPSocket clientSocket = UDPSocket("127.0.0.1", 2230);

    clientSocket.Connect();
    clientSocket.OpenSession(2231);
    
    Game ballGame = Game(&clientSocket, 500, 500);
    ballGame.Loop();
    
}
