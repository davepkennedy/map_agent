//
//  direction.hpp
//  map_agent
//
//  Created by Dave Kennedy on 29/06/2018.
//  Copyright © 2018 Dave Kennedy. All rights reserved.
//

#ifndef direction_hpp
#define direction_hpp

class map_client;

class direction {
public:
    virtual void handleMovement (map_client* client, int north, int east, int south, int west) const = 0;
public:
    static const direction* north;
    static const direction* east;
    static const direction* south;
    static const direction* west;
};

class North : public direction {
public:
    void handleMovement (map_client* client, int north, int east, int south, int west) const;
};

class East : public direction {
public:
    void handleMovement (map_client* client, int north, int east, int south, int west) const;
};

class South : public direction {
public:
    void handleMovement (map_client* client, int north, int east, int south, int west) const;
};

class West : public direction {
public:
    void handleMovement (map_client* client, int north, int east, int south, int west) const;
};

#endif /* direction_hpp */
