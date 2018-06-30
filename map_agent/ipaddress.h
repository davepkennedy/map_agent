//
//  ipaddress.hpp
//  raft
//
//  Created by Dave Kennedy on 30/11/2015.
//
//

#ifndef ipaddress_h
#define ipaddress_h

#include <string>
#include <exception>
#include <sstream>

#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

namespace network {
    
    class basic_address {
    protected:
        std::string portString (short s) const {
            std::ostringstream stream;
            stream << s;
            return stream.str();
        }
        
    public:
        virtual int family() const = 0;
        virtual socklen_t addrLength() const = 0;
        virtual std::string hostname (const sockaddr* t) const = 0;
        virtual short hostport (const sockaddr* t) const = 0;
        
        virtual void hostaddr (const std::string& name, short port, sockaddr* addr, int sockType = SOCK_STREAM) const {
            int status;
            struct addrinfo hints;
            struct addrinfo *servinfo;  // will point to the results
            
            memset(&hints, 0, sizeof hints); // make sure the struct is empty
            hints.ai_family = family();     // don't care IPv4 or IPv6
            hints.ai_socktype = sockType; // TCP stream sockets
            
            if ((status = getaddrinfo(name.c_str(), portString(port).c_str(), &hints, &servinfo)) != 0) {
                throw std::runtime_error(gai_strerror(status));
            }
            
            memcpy(addr, servinfo->ai_addr, servinfo->ai_addrlen);
            
            freeaddrinfo(servinfo);
            
        }
        /*
        static std::string hostname () {
            char buffer [256];
            ::gethostname(buffer, sizeof(buffer));
            return buffer;
        }
        */
    };
    
    template <typename T>
    class address: public basic_address {
    public:
        int family() const {return AF_UNSPEC;}
        socklen_t addrLength() const {return 0;}
        
        std::string hostname (const sockaddr* t) const {
            throw std::runtime_error ("hostname is undefine for this address type");
        }
        
        short hostport (const sockaddr* addr) const {
            throw std::runtime_error ("hostport is undefine for this address type");
        }
    };
    
    template <>
    class address<sockaddr_in>: public basic_address {
    public:
        int family() const {return AF_INET;}
        socklen_t addrLength() const {
            return sizeof(sockaddr_in);
        }
        
        std::string hostname (const sockaddr* addr) const {
            char buffer[INET_ADDRSTRLEN];
            inet_ntop(family(), &((sockaddr_in*)addr)->sin_addr, buffer, sizeof(buffer));
            return buffer;
        }
        
        short hostport (const sockaddr* addr) const {
            return htons(((sockaddr_in*)addr)->sin_port);
        }
        
    };
    
    template <>
    class address<sockaddr_in6>: public basic_address {
    public:
        int family() const {return AF_INET6;}
        socklen_t addrLength() const {
            return sizeof(sockaddr_in6);
        }
        
        std::string hostname (const sockaddr* addr) const {
            char buffer[INET6_ADDRSTRLEN];
            inet_ntop(family(), &((sockaddr_in6*)addr)->sin6_addr, buffer, sizeof(buffer));
            return buffer;
        }
        
        short hostport (const sockaddr* addr) const {
            return htons(((sockaddr_in6*)addr)->sin6_port);
        }
    };
    
    enum ip_version {
        ip4 = 4,
        ip6 = 6
    };
    
    template <ip_version IPv>
    struct address_version {
        using address_type = sockaddr_storage;
    };
    
    template <>
    struct address_version<ip4> {
        using address_type = sockaddr_in;
    };
    
    template <>
    struct address_version<ip6> {
        using address_type = sockaddr_in6;
    };
}

#endif /* ipaddress_h */
