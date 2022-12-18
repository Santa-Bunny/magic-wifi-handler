
#include <tins/tins.h>
#include "../magicWiFi.hpp"
#include <cassert>
#include <iostream>
#include <string>
#include <sstream>



using namespace Tins;



#define SNIFFER
#define SENDER


int main(int argc, char* argv[]) {


    if (argc == 1 || argv[1][1] == 'h'){
        std::cout << "correct use: " << argv[0] << " <MAC Address>\n\n";
        std::cout << "write all messages in forms of <priority bit> <message>\n"
            << "For example:\n1 high priority message!\n0 low priority message\n\n";
        std::cout << "every sent message that makes it to it's destination should receive an\n" 
            << "-->Ack message. As well all messages received will be displayed with -->\n";
        return 0;
    }

    PacketSender sender;
    IP pkt = IP("192.168.0.1") / TCP(22) / RawPDU("foo");
    sender.send(pkt);
    std::cin.ignore(5);
    magicwifi::MAC_addr = std::string(argv[1]);
    

    std::stringstream userInputStream;
    std::string userInputString;
    bool priority;
    std::string message = "test";
    
    std::getline(std::cin, userInputString);
    userInputStream << userInputString;
    
    if (userInputString[0] != 'q'){
        userInputStream >> priority;
        std::getline(userInputStream, message);
    }
    else {
        message = 'q';
    }
    magicwifi::SendPacket(1, message);


    while(message[0] != 'q') {
        
        std::getline(std::cin, userInputString);
        userInputStream << userInputString;
    
        if (userInputString[0] != 'q') {
            userInputStream >> priority;
            std::getline(userInputStream, message);
            std::cout << priority << " " << message << std::endl;
            magicwifi::SendPacket(priority, message);
        }
    }
}
// // same as above, just shorter
// EthernetII eth = EthernetII("77:22:33:11:ad:ad", info.hw_addr) / 
//                  IP("0.0.0.0", info.ip_addr) /
//                  TCP(13, 15) /
//                  RawPDU("I'm a payload!");