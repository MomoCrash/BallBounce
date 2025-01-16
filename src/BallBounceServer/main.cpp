#include "main.h"

#include <iostream>

#include "Network/Server.h"

int main()
{

    Server server = Server("127.0.0.1", 2229);

    server.Connect();
    server.OpenSession(2230);

    while(true)
    {
        Sleep(500);
        int recievedData = 0;
        while (server.GetDatas().size() != 0)
        {
            char* data = server.GetDatas().front();

            for (int i = 0; i < sizeof(data); i++)
                std::cout << data[i] << std::endl;

            char uuid = *(data+8);
            int x;
            int y;
            
            memcpy(&x, data, 4);
            memcpy(&y, data+4, 4);

            std::cout << uuid << std::endl;
            std::cout << server.GetServerData().ballsPosition.size() << std::endl;

            if (!server.GetServerData().clientSessions.contains(uuid))
            {
                std::cout << "New client \n";
                int index = server.GetServerData().ballsPosition.size();
                server.GetServerData().clientSessions[uuid] = index;
                server.GetServerData().ballsPosition.push_back(Vector2(x, y));
            } else
            {
                std::cout << "Updated client : " << server.GetServerData().clientSessions[uuid] << "\n";
                server.GetServerData().ballsPosition[server.GetServerData().clientSessions[uuid]].x = x;
                server.GetServerData().ballsPosition[server.GetServerData().clientSessions[uuid]].y = y;
            }

            server.GetDatas().pop();
            recievedData++;
        }

        std::cout << "Packet received : " << recievedData << "  \nUnique data set : \n";
        for (int i = 0; i < server.GetServerData().ballsPosition.size(); i++)
        {
            auto element = server.GetServerData().ballsPosition[i];
            std::cout << i << " : " << element.x << "x," << element.y << "y\n";
        }
    }
    
}