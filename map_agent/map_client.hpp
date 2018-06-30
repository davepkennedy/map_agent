//
//  map_client.hpp
//  map_agent
//
//  Created by Dave Kennedy on 29/06/2018.
//  Copyright © 2018 Dave Kennedy. All rights reserved.
//

#ifndef map_client_hpp
#define map_client_hpp

#include <string>
#include "socket.h"
#include "select.h"
#include "direction.hpp"

class map_client {
private:
    network::ip4_socket _socket;
    network::selector<network::ip4> _selector;
    std::string _host;
    std::string _team;
    int _port;
    bool _done;
    const direction* _direction;
protected:
    void send(const char* msg);
    void handleData(const char* data);
public:
    map_client(const std::string& host, int port, const std::string& team);
    inline bool done() const {return _done;}
    void move();
    
    void moveNorth();
    void moveEast();
    void moveSouth();
    void moveWest();
};

#endif /* map_client_hpp */
