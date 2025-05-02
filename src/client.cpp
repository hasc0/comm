#include <iostream>
#include <string>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include "shared.hpp"

using std::string;

class comm_client {
private:
	const string username;

	string addr;
	int port;

	struct commsock *sock;

public:
	comm_client(string username) : username(username) {}

	bool connect_server() {
		string addr;
		std::cout << "Please enter an IPv4 server address: ";
		std::getline(std::cin, addr);

		string port;
		std::cout << "Please enter the server port: ";
		std::getline(std::cin, port);

		struct commsock *sock = (struct commsock *) calloc(1, sizeof(struct commsock));
		if (!get_socket(addr, stoi(port), sock)) {
			return false;
		}

		std::cout << "Attempting to connect to server..." << std::endl;

		if (connect(sock->sock_fd, (struct sockaddr *) sock->sock_info, sock->addr_len) == -1) {
			std::cerr << "Failed to connect to server.\n";
			return false;
		}

		this->addr = addr;
		this->port = stoi(port);
		this->sock = sock;

		std::cout << "Connection successful, connected to: " << this->addr << ":" << this->port << std::endl;

		return true;
	}
};

int main() {
	string username;
	bool len_flag = false;
	while (!len_flag) {
		std::cout << "Please enter a username (16 characters max): ";
		std::getline(std::cin, username);

		if (username.length() > 16) {
			std::cerr << "Username can be at most 16 characters long.\n";
		} else {
			len_flag = true;
		}
	}

	comm_client c_client = comm_client(username);

	bool conn_flag = false;
	while (!conn_flag) {
		conn_flag = c_client.connect_server();
	}

	return EXIT_SUCCESS;
}

