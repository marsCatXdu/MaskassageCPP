/**
 * 服务端，在自己 ip 的指定端口上监听
 * 客户端发来报文暴露位置，返回信息
 * 
 * This program should be running on traverse server
 * and forward nat-traverse info(i.e. two endpoint)
 * automaticly
*/

#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>

using namespace std;
using namespace boost::asio;

// @brief Storage incomming endpoint info
class targetEndpoint {
public:
    targetEndpoint(string i, string p):tgtIp(i), tgtProt(p){}
    string tgtIp;
    string tgtProt;
};

// @brief Forward one endpoint information to the other
class endpointForwarder {

};

// @brief traverse aux server implementation
class traverseServer {
public:
    traverseServer() {}
};

int main() {
    io_service io;
    ip::udp::socket sock(io, ip::udp::endpoint(ip::udp::v4(), 6699));

    char buf[2];
    char buf2[2];

    ip::udp::endpoint ep;       // first incomming client
    ip::udp::endpoint ep2;      // second incomming client
/**
 * @brief: process incomming requests.(say hello to each other)
 * @TODO: Make it like a handshake
*/
    sock.receive_from(buffer(buf) ,ep, 0);      // ep1 info
    targetEndpoint tea(ep.address().to_string(), boost::lexical_cast<string>(ep.port()));

    sock.receive_from(buffer(buf2) ,ep2, 0);    // ep2 info
    targetEndpoint teb(ep2.address().to_string(), boost::lexical_cast<string>(ep2.port()));

    sock.send_to(buffer(tea.tgtIp+":"+tea.tgtProt), ep2);
    sock.send_to(buffer(teb.tgtIp+":"+teb.tgtProt), ep);
// hello end.

/**
 * @brief: forwarding working loop
 * @TODO: make it useable
*/
    for(; ; ) {
        char m_buf[1024]={0}, m_buf2[1024]={0};
        if(sock.receive_from(buffer(m_buf) ,ep)) {
            std::cout<<"Hear from ep, Sending to ep2\n";
            sock.send_to(buffer(m_buf), ep2);
            m_buf[1024]={0};
            m_buf2[1024]={0};
        }
        if(sock.receive_from(buffer(m_buf2) ,ep2)) {
            std::cout<<"Hear from ep2, Sending to ep\n";
            sock.send_to(buffer(m_buf2), ep);
            m_buf[1024]={0};
            m_buf2[1024]={0};
        }
    }

    return 0;
}