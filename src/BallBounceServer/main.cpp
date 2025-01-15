#include "main.h"

#include <iostream>

#include "Network/Server.h"

int main()
{

    Server server = Server("127.0.0.1", 2231);

    server.Connect();
    server.OpenSession(2230);

    while(true)
    {
        int x;
        int y;
        memcpy(&x, server.GetSocketBuffer(), 4);
        memcpy(&y, server.GetSocketBuffer()+4, 4);

        std::cout << x << " ::: " << y << std::endl;
    }
    
}