//
//  direction.cpp
//  map_agent
//
//  Created by Dave Kennedy on 29/06/2018.
//  Copyright © 2018 Dave Kennedy. All rights reserved.
//

#include "direction.hpp"
#include "map_client.hpp"

const direction* direction::north = new North;
const direction* direction::east = new East;
const direction* direction::south = new South;
const direction* direction::west = new West;

void North::handleMovement (map_client* client, int north, int east, int south, int west) const
{
    if (west) {
        client->moveWest();
    } else if (north) {
        client->moveNorth();
    } else if (east) {
        client->moveEast();
    } else if (south) {
        client->moveSouth();
    }
}

void East::handleMovement (map_client* client, int north, int east, int south, int west) const
{
    if (north) {
        client->moveNorth();
    } else if (east) {
        client->moveEast();
    } else if (south) {
        client->moveSouth();
    } else if (west) {
        client->moveWest();
    }
}

void South::handleMovement (map_client* client, int north, int east, int south, int west) const
{
    if (east) {
        client->moveEast();
    } else if (south) {
        client->moveSouth();
    } else if (west) {
        client->moveWest();
    } else if (north) {
        client->moveNorth();
    }
}

void West::handleMovement (map_client* client, int north, int east, int south, int west) const
{
    if (south) {
        client->moveSouth();
    } else if (west) {
        client->moveWest();
    } else if (north) {
        client->moveNorth();
    } else if (east) {
        client->moveEast();
    }
}
