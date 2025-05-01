#include <iostream>
#include <cstdlib>
#include <cstring>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

class comm_server {
private:
	char *addr;
	int port;
	int s_socket_fd;

public:
	comm_server(char *addr, int port) : addr(addr), port(port) {}

	bool init_server() {
		std::cout << "Starting comm server..." << std::endl;

		struct addrinfo *s_addr_res;
		struct addrinfo s_addr_hints = {
			.ai_flags = 0,
			.ai_family = AF_INET,
			.ai_socktype = SOCK_STREAM,
			.ai_protocol = IPPROTO_TCP,
			.ai_addrlen = 0,
			.ai_addr = NULL,
			.ai_canonname = NULL,
			.ai_next = NULL
		};

		if (int err = getaddrinfo(this->addr, NULL, &s_addr_hints, &s_addr_res) != 0) {
			std::cerr << "Failed to resolve address (error " << err << ")\n";
			return false;
		}

		struct addrinfo *s_addr_info = s_addr_res;

		int s_socket_fd = socket(s_addr_info->ai_family, s_addr_info->ai_socktype, s_addr_info->ai_protocol);
		if (s_socket_fd == -1 && s_addr_info->ai_next == NULL) {
			std::cerr << "Failed to create socket.\n";
			freeaddrinfo(s_addr_res);
			return false;
		} else if (s_socket_fd == -1 && s_addr_info->ai_next != NULL) {
			struct addrinfo *s_addr_curr = s_addr_info;
			struct addrinfo *s_addr_next = s_addr_info->ai_next;
			while (s_socket_fd == -1 && s_addr_next != NULL) {
				s_socket_fd = socket(s_addr_next->ai_family, s_addr_next->ai_socktype, s_addr_next->ai_protocol);
				s_addr_curr = s_addr_next;
				s_addr_next = s_addr_curr->ai_next;
			}

			if (s_socket_fd == -1) {
				std::cerr << "Failed to create socket.\n";
				freeaddrinfo(s_addr_res);
				return false;
			} else {
				s_addr_info = s_addr_curr;
			}
		}

		struct sockaddr_in *s_sock_info = (struct sockaddr_in *) s_addr_info->ai_addr;
		s_sock_info->sin_port = htons(port);

		if (bind(s_socket_fd, (sockaddr *) s_sock_info, s_addr_info->ai_addrlen) == -1) {
			std::cerr << "Failed to bind socket to address.\n";
			freeaddrinfo(s_addr_res);
			return false;
		}

		std::cout << "Server is running at " << this->addr << ":" << this->port << std::endl;

		freeaddrinfo(s_addr_res);
		this->s_socket_fd = s_socket_fd;

		return true;
	}

	void start_server() {
		while (true) {
			listen(this->s_socket_fd, 128);
		}
	}
};

int main(int argc, char *argv[]) {
	if (argc != 3) {
		std::cerr << "Please supply a valid address and port.\nUsage: " << argv[0] << " [address] [port]\n";
		exit(EXIT_FAILURE);
	}

	comm_server c_server = comm_server(argv[1], atoi(argv[2]));
	if (!c_server.init_server()) {
		exit(EXIT_FAILURE);
	}

	c_server.start_server();

	return EXIT_SUCCESS;
}

