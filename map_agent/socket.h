//
//  socket.hpp
//  raft
//
//  Created by Dave Kennedy on 30/11/2015.
//
//

#ifndef socket_h
#define socket_h

#include <utility>
#include <exception>
#include <unistd.h>

#include "ipaddress.h"

namespace network {
    
    template <ip_version IPv>
    class socket {
    private:
        using address_type = typename address_version<IPv>::address_type;
        address<address_type> address_trait;
    private:
        int _socketType;
        int _socket;
        inline void throwIfUnset() const {
            if (_socket == 0) {
                throw std::runtime_error("socket is unset");
            }
        }
    protected:
        socket (int handle, int socketType)
        : _socket (handle)
        , _socketType(socketType) {}
    public:
        socket (int socketType = SOCK_STREAM)
        : _socketType (socketType)
        , _socket (::socket(address_trait.family(), socketType, 0)){
        }
        
        socket (const socket& other)
        : _socket (other._socket)
        , _socketType(other._socketType)
        {}
        
        socket (socket&& other)
        : _socket (0)
        , _socketType (AF_UNSPEC)
        {
            std::swap(_socket, other._socket);
            std::swap(_socketType, other._socketType);
        }
        
        virtual ~socket() {
            if (_socket) {
                shutdown();
                close();
            }
        }
        
        operator int() const {return _socket;}
        const socket<IPv>& operator = (const socket<IPv>& other) {
            _socket = other._socket;
            _socketType = other._socketType;
            return *this;
        }
        
        socket<IPv>& operator = (socket<IPv>&& other) {
            std::swap(_socket, other._socket);
            std::swap(_socketType, other._socketType);
            return *this;
        }
        
        bool operator == (const socket& other) {
            return (_socket == other._socket);
        }
        
        
        void shutdown (int how = SHUT_RDWR) const {
            if (_socket) {
                ::shutdown(_socket, how);
            }
        }
        
        void close () {
            if (_socket) {
                ::close (_socket);
                _socket = 0;
            }
        }
        
        /*
        void bind (short port) const {
            this->bind (basic_address::hostname(), port);
        }
        */
        
        void bind (const std::string& hostname, short port) const {
            throwIfUnset();
            sockaddr_storage addr;
            address_trait.hostaddr (hostname, port, (sockaddr*)&addr, _socketType);
            std::string name = address_trait.hostname((sockaddr*)&addr);
            int success = ::bind(_socket, (sockaddr*)&addr, address_trait.addrLength());
        }
        
        void connect (const std::string& hostname, short port) const {
            throwIfUnset();
            sockaddr_storage addr;
            address_trait.hostaddr (hostname, port, (sockaddr*)&addr, _socketType);
            ::connect(_socket, (sockaddr*)&addr, address_trait.addrLength());
        }
        
        void listen (int backlog = 5) {
            throwIfUnset();
            ::listen(_socket, backlog);
        }
        
        std::string sockname () const {
            throwIfUnset();
            sockaddr_storage addr;
            socklen_t addrLen;
            memset (&addr, 0, sizeof(addr));
            getsockname(_socket, (sockaddr*)&addr, &addrLen);
            return address_trait.hostname ((sockaddr*)&addr);
        }
        
        std::string peername () const {
            throwIfUnset();
            sockaddr_storage addr;
            socklen_t addrLen;
            memset (&addr, 0, sizeof(addr));
            getpeername(_socket, (sockaddr*)&addr, &addrLen);
            return address_trait.hostname ((sockaddr*)&addr);
        }
        
        socket<IPv> accept () const {
            throwIfUnset();
            sockaddr_storage addr;
            socklen_t length;
            int client = ::accept(_socket, (sockaddr*)&addr, &length);
            
            return socket<IPv> (client, _socketType);
        }
        
        size_t send (const void* data, size_t length) const {
            throwIfUnset();
            return ::send(_socket, data, length, 0);
        }
        
        size_t receive (void* data, size_t length) const {
            throwIfUnset();
            return ::recv(_socket, data, length, 0);
        }
        
    };
    
    using ip4_socket = socket<ip4>;
    using ip6_socket = socket<ip6>;
    
}

#endif /* socket_h */
