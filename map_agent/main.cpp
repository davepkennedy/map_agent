//
//  main.cpp
//  map_agent
//
//  Created by Dave Kennedy on 29/06/2018.
//  Copyright © 2018 Dave Kennedy. All rights reserved.
//

#include <iostream>
#include "map_client.hpp"

void doMove(network::ip4_socket& socket,
            int north, int east, int south, int west,
            const char* prize)
{
    
}

int main(int argc, const char * argv[]) {
    // insert code here...
    if (argc < 4) {
        std::cout << "Usage: " << argv[0] << " address port team" << std::endl;
        return 1;
    }
    
    map_client client (argv[1], atoi(argv[2]), argv[3]);
    while (!client.done()) {
        client.move();
    }
    
    return 0;
}
