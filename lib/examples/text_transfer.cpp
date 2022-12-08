
#include <tins/tins.h>
#include "../magicWiFi.hpp"
#include <cassert>
#include <iostream>
#include <string>
#include <sstream>
#include <thread>



using namespace Tins;



#define SNIFFER
#define SENDER

std::mutex cout_lock;

void sniffer(){
    SnifferConfiguration::set_immediate_mode(1);
    SnifferConfiguration conf;
    conf.set_promisc_mode(true);
    conf.set_filter("port 13");

    Sniffer sniff("eth0", conf);

    sniff.sniff_loop(magicwifi::priority_sniff(), 0);
}


int main(int argc, char* argv[]) {


    if (argc == 1 || argv[1][1] == 'h'){
        std::cout << "correct use: " << argv[0] << " <MAC Address>\n\n";
        std::cout << "write all messages in forms of <priority bit> <message>\n"
            << "For example:\n1 high priority message!\n0 low priority message\n\n";
        std::cout << "every sent message that makes it to it's destination should receive an\n" 
            << "-->Ack message. As well all messages received will be displayed with -->\n";
        return 0;
    }

    magicwifi::MAC_addr = string(argv[1]);
    

    std::stringstream s;
    bool priority;
    std::string message = "test";
    
    std::getline(std::cin, s);
    
    if (s[0] != 'q'){
        s >> priority;
        std::getline(s, message);
    }
    else {
        message = 'q';
    }
    magicwifi::SendPacket(1, message);
    

#ifdef SNIFFER
    std::thread t1(sniffer(), nullptr);
#endif //SNIFFER

    while(message[0] != 'q') {

        
        
        
        std::getline(std::cin, s);
    
        if (s[0] != 'q'){
            s >> priority;
            std::getline(s, message);
            cout_lock.lock();
            std::cout << priority << " " << message << std::endl;
            cout_lock.unlock();
            magicwifi::SendPacket(priority, message);
        }
    }
}
// // same as above, just shorter
// EthernetII eth = EthernetII("77:22:33:11:ad:ad", info.hw_addr) / 
//                  IP("0.0.0.0", info.ip_addr) /
//                  TCP(13, 15) /
//                  RawPDU("I'm a payload!");