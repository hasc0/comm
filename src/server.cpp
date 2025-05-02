#include <iostream>
#include <string>
#include <vector>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include "shared.hpp"

using std::string;
using std::vector;

class comm_server {
private:
	const string addr;
	const int port;

	struct commsock *sock;

	size_t max_clients;
	vector<int> clients;

public:
	comm_server(string addr, int port) : addr(addr), port(port), max_clients(128) {}

	bool init_server() {
		std::cout << "Starting comm server..." << std::endl;

		struct commsock *sock = (struct commsock *) calloc(1, sizeof(struct commsock));
		if (!get_socket(this->addr, this->port, sock)) {
			return false;
		}

		if (bind(sock->sock_fd, (sockaddr *) sock->sock_info, sock->addr_len) == -1) {
			std::cerr << "Failed to bind socket to address.\n";
			return false;
		}

		this->sock = sock;

		return true;
	}

	void start_server() {
		if (listen(this->sock->sock_fd, 128) == -1) {
			std::cerr << "Failed to listen for connections.\n";
			exit(EXIT_FAILURE);
		}

		std::cout << "Server is listening at " << this->addr << ":" << this->port << std::endl;

		while (true) {
			if (clients.size() < max_clients) {
				int client_fd = accept(this->sock->sock_fd, (struct sockaddr *) this->sock->sock_info, &this->sock->addr_len);
				if (client_fd == -1) {
					std::cout << "Failed to establish connection with client." << std::endl;
				} else {
					clients.push_back(client_fd);
					std::cout << "Connection established with client." << std::endl;
				}
			}
		}
	}
};

int main() {
	string addr;
	std::cout << "Please enter an IPv4 address: ";
	std::getline(std::cin, addr);

	string port;
	std::cout << "Please enter a port: ";
	std::getline(std::cin, port);

	comm_server c_server = comm_server(addr, stoi(port));
	if (!c_server.init_server()) {
		exit(EXIT_FAILURE);
	}

	c_server.start_server();

	return EXIT_SUCCESS;
}

