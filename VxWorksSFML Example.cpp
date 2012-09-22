#include "../SFML/Network/IpAddress.hpp"
#include "../SFML/Network/Packet.hpp"
#include "../SFML/Network/UdpSocket.hpp"

int main() {
	sf::UdpSocket serverSocket;
	unsigned short robotPort = 5614;
	serverSocket.bind( robotPort ); // must bind before sending or receiving
	sf::IpAddress robotIP( "10.35.12.2" );

	sf::Packet sendPacket;
	float rotate = 0.f;
	float distance = 0.f;

	while ( 1 ) { // while program is running
		/* get data to send */

		sendPacket.clear(); // clear packet before adding more data
		sendPacket << rotate << distance;
		serverSocket.send( sendPacket , robotIP , robotPort ); // it doesn't matter if the socket send fails so we won't check for error conditions like sf::Socket::Fail, sf::Socket::Disconnect, or sf::Socket::NotReady
	}

	return 0;
}
