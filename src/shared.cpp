#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include "shared.hpp"

using std::string;

bool get_socket(string addr, int port, struct commsock *sock) {
	struct addrinfo *addr_res;
	struct addrinfo addr_hints = {
		.ai_flags = 0,
		.ai_family = AF_INET,
		.ai_socktype = SOCK_STREAM,
		.ai_protocol = 0,
		.ai_addrlen = 0,
		.ai_addr = NULL,
		.ai_canonname = NULL,
		.ai_next = NULL
	};

	int err = getaddrinfo(addr.c_str(), NULL, &addr_hints, &addr_res);
	if (err != 0) {
		std::cerr << "Failed to resolve address (error " << err << ")\n";
		return false;
	}

	struct addrinfo *addr_info = addr_res;

	int sock_fd = socket(addr_info->ai_family, addr_info->ai_socktype, 0);
	if (sock_fd == -1 && addr_info->ai_next == NULL) {
		std::cerr << "Failed to create socket.\n";
		freeaddrinfo(addr_res);
		return false;
	} else if (sock_fd == -1 && addr_info->ai_next != NULL) {
		struct addrinfo *addr_curr = addr_info;
		struct addrinfo *addr_next = addr_info->ai_next;
		while (sock_fd == -1 && addr_next != NULL) {
			sock_fd = socket(addr_next->ai_family, addr_next->ai_socktype, addr_next->ai_protocol);
			addr_curr = addr_next;
			addr_next = addr_curr->ai_next;
		}

		if (sock_fd == -1) {
			std::cerr << "Failed to create socket.\n";
			freeaddrinfo(addr_res);
			return false;
		} else {
			addr_info = addr_curr;
		}
	}

	struct sockaddr_in *sock_addr = (struct sockaddr_in *) addr_info->ai_addr;
	sock_addr->sin_port = htons(port);

	sock->sock_fd = sock_fd;
	sock->sock_addr = sock_addr;
	sock->sock_len = addr_info->ai_addrlen;
	freeaddrinfo(addr_res);

	return true;
}

