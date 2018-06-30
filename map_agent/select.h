//
//  select.h
//  raft
//
//  Created by Dave Kennedy on 01/12/2015.
//
//

#ifndef select_h
#define select_h

#include <sys/select.h>
#include <vector>
#include <functional>

#include "socket.h"

namespace network {
    template <ip_version IPv>
    class selector {
    private:
        std::vector<socket<IPv>> _sockets;
        std::function<void (socket<IPv>&)> _readHandler;
        std::function<void (socket<IPv>&)> _writeHandler;
        std::function<void (socket<IPv>&)> _errorHandler;
        fd_set _baseSet;
        int _maxFd;
    private:
        void setSocket (const socket<IPv>& s) {
            FD_SET(s, &_baseSet);
            if (s > _maxFd) {
                _maxFd = s;
            }
        }
    public:
        selector()
        : _maxFd (0)
        {
            FD_ZERO(&_maxFd);
        }
        
        size_t size () {
            return _sockets.size();
        }
        
        void onRead (std::function<bool (socket<IPv>&)> && handler) {
            _readHandler = handler;
        }
        
        void onWrite (std::function<bool (socket<IPv>&)> && handler) {
            _writeHandler = handler;
        }
        
        void onError (std::function<bool (socket<IPv>&)> && handler) {
            _errorHandler = handler;
        }
        
        // Copies socket, leaves original alone
        void add (const socket<IPv>& s) {
            _sockets.push_back (s);
            setSocket(s);
        }
        
        // Moves socket, original should become unset
        void add (socket<IPv>&& s) {
            setSocket(s);
            _sockets.push_back (std::move(s));
        }
        
        void remove (socket<IPv>& s) {
            _sockets.erase (std::remove (_sockets.begin(), _sockets.end(), s), _sockets.end());
            FD_CLR(s, &_baseSet);
        }
        
        void select () {
            fd_set readSet;
            fd_set writeSet;
            fd_set errorSet;
            
            FD_COPY(&_baseSet, &readSet);
            FD_COPY(&_baseSet, &writeSet);
            FD_COPY(&_baseSet, &errorSet);
            
            if (::select (_maxFd+1, &readSet, &writeSet, &errorSet, nullptr) > 0) {
                for (socket<IPv>& s : _sockets) {
                    if (FD_ISSET (s, &readSet) && _readHandler) {
                        _readHandler(s);
                    }
                    if (FD_ISSET (s, &writeSet) && _writeHandler) {
                        _writeHandler(s);
                    }
                    if (FD_ISSET (s, &errorSet) && _errorHandler) {
                        _errorHandler(s);
                    }
                }
            }
        }
    };
}

#endif /* select_h */
