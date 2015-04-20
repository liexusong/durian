#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int create_listen_socket(char *host, short port)
{
	int sock;
	struct in_addr addr;
	struct sockaddr_in sin;
	int type = AF_INET;

	if (!inet_aton(host, &addr)) {
		struct hostent *hent;
		char buf[32];

		hent = gethostbyname(host);
		if (!hent) {
			return -1;
		}

		type = hent->h_addrtype;

		host = inet_ntop(type,
		    hent->h_addr, buf, sizeof(buf));
		if (!inet_aton(host, &addr)) {
			return -1;
		}
	}

	sock = socket(type, SOCK_STREAM, 0);
	if (sock == -1) {
		return -1;
	}

	bzero(&sin, sizeof(sin));

    sin.sin_family = type;
	sin.sin_addr.s_addr = addr;
	sin.sin_port = htons(port);

	if (bind(sock, (struct sockaddr *)(&sin),
	    sizeof(sin)) == -1)
	{
		close(sock);
		return -1;
	}

	if (listen(sock, 100) == -1) {
		close(sock);
		return -1;
	}

	return sock;
}

