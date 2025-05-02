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
	struct addrinfo *s_addr_res;
	struct addrinfo s_addr_hints = {
		.ai_flags = 0,
		.ai_family = AF_INET,
		.ai_socktype = SOCK_STREAM,
		.ai_protocol = 0,
		.ai_addrlen = 0,
		.ai_addr = NULL,
		.ai_canonname = NULL,
		.ai_next = NULL
	};

	int err = getaddrinfo(addr.c_str(), NULL, &s_addr_hints, &s_addr_res);
	if (err != 0) {
		std::cerr << "Failed to resolve address (error " << err << ")\n";
		return false;
	}

	struct addrinfo *s_addr_info = s_addr_res;

	int s_sock_fd = socket(s_addr_info->ai_family, s_addr_info->ai_socktype, 0);
	if (s_sock_fd == -1 && s_addr_info->ai_next == NULL) {
		std::cerr << "Failed to create socket.\n";
		freeaddrinfo(s_addr_res);
		return false;
	} else if (s_sock_fd == -1 && s_addr_info->ai_next != NULL) {
		struct addrinfo *s_addr_curr = s_addr_info;
		struct addrinfo *s_addr_next = s_addr_info->ai_next;
		while (s_sock_fd == -1 && s_addr_next != NULL) {
			s_sock_fd = socket(s_addr_next->ai_family, s_addr_next->ai_socktype, s_addr_next->ai_protocol);
			s_addr_curr = s_addr_next;
			s_addr_next = s_addr_curr->ai_next;
		}

		if (s_sock_fd == -1) {
			std::cerr << "Failed to create socket.\n";
			freeaddrinfo(s_addr_res);
			return false;
		} else {
			s_addr_info = s_addr_curr;
		}
	}

	struct sockaddr_in *s_sock_info = (struct sockaddr_in *) s_addr_info->ai_addr;
	s_sock_info->sin_port = htons(port);

	sock->sock_fd = s_sock_fd;
	sock->sock_info = s_sock_info;
	sock->addr_len = s_addr_info->ai_addrlen;
	freeaddrinfo(s_addr_res);

	return true;
}

