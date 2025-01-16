#include <iostream>
#include "main.h"
#include "SFML/Main.hpp"
#include "Network/UDPSocket.h"
#include "Game/Game.h"

int main()
{

    UDPSocket clientSocket = UDPSocket("127.0.0.1", 2230);

    clientSocket.Connect();
    clientSocket.OpenSession(2229);
    
    Game ballGame = Game(&clientSocket, 500, 500, 'b');
    ballGame.Loop();
    
}
