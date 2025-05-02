#ifndef SHARED_HPP
#define SHARED_HPP

#include <string>

#include <netdb.h>

using std::string;

struct commsock {
	int sock_fd;
	struct sockaddr_in *sock_addr;
	socklen_t sock_len;
};

bool get_socket(string addr, int port, struct commsock *sock);

#endif

