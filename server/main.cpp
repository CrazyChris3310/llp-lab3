#include <iostream>
#include <netinet/ip.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024*10

struct sockaddr_in local;

int main() {

    int ss = socket(AF_INET, SOCK_STREAM, 0);

	inet_aton("127.0.0.1", &local.sin_addr);
	local.sin_port = htons(1234);
	local.sin_family = AF_INET;

	bind(ss, (struct sockaddr*)&local, sizeof(local));
	listen(ss, 5);

	int cs = accept(ss, NULL, NULL);

	char buf[BUFFER_SIZE];
	while (1) {
		int bytes_read = recv(cs, buf, BUFFER_SIZE, 0);
		buf[bytes_read] = 0;
        printf("%s\n", buf);
	}

    return 0;
}