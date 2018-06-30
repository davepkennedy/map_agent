  //
//  map_client.cpp
//  map_agent
//
//  Created by Dave Kennedy on 29/06/2018.
//  Copyright © 2018 Dave Kennedy. All rights reserved.
//

#include "map_client.hpp"
#include <iostream>

#define ASK_LANGUAGE "Language/team name?"

map_client::map_client(const std::string& host, int port, const std::string& team)
: _host(host)
, _port(port)
, _team(team)
, _done(false)
, _direction(direction::east)
{
    _socket.connect(host, _port);
    _selector.add(_socket);
    _selector.onRead([this](network::ip4_socket& socket){
        char buffer[1024];
        size_t read = socket.receive(buffer, sizeof(buffer));
        buffer[read] = '\0';
        this->handleData(buffer);
        return true;
    });
}

void map_client::send(const char* msg) {
    _socket.send(msg, strlen(msg));
    _socket.send("\n", strlen("\n"));
}

void map_client::move()
{
    _selector.select();
}

void map_client::handleData(const char* data)
{
    int n, e, s, w;
    char p;
    if (strncmp(ASK_LANGUAGE, data, strlen(ASK_LANGUAGE)) == 0) {
        send(_team.c_str());
    } else if (sscanf(data, "N%d E%d S%d W%d P%c", &n, &e, &s, &w, &p)) {
        if (p == 'X') {
            _done = true;
        } else {
            _direction->handleMovement(this, n,e,s,w);
        }
    }
}

void map_client::moveNorth()
{
    send("N");
    _direction = direction::north;
}

void map_client::moveEast()
{
    send("E");
    _direction = direction::east;
}

void map_client::moveSouth()
{
    send("S");
    _direction = direction::south;
}

void map_client::moveWest()
{
    send("W");
    _direction = direction::west;
}
