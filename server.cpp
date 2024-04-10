#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/ip.h>




//Display error message
static void msg(const char *msg) {
   fprintf(stderr, "%s\n", msg);
}


// Display error message and exit syscall
static void die(const char *msg) {
    int err = errno;
    fprintf(stderr, "[%d] %s\n", err, msg);
    abort();
}


//Read and Write
static void do_something(int connfd) {
    char rbuf[64] = {};
    ssize_t n = read(connfd, rbuf, sizeof(rbuf) -1);
    if (n < 0) {
        msg("red() error");
        return;
    }
    printf("client says: %s\n", rbuf);

    char wbuf[] = "world";
    write(connfd, wbuf, strlen(wbuf));
}








int main() {

    int fd = socket(AF_INET, SOCK_STREAM, 0); // AF_INET = IPv4, SOCK_STREAM = TCP
    if (fd < 0) {
        die("socket()");
    }

    // This is needed for most server applications.
    int val = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val)); // 2ND & 3RD chooses options to set, 4TH is agrument, option vlaie is ytes so lnegth is specified

    //bind
    struct sockaddr_in addr = {}; // holds IPv4 adress and port
    addr.sin_family = AF_INET;
    addr.sin_port = ntohs(1234); // port 1234, noths converts to big endian
    addr.sin_addr.s_addr = ntohl(0); // wildcard adress 0.0.0.0, ntohl converts to big endian
    int rv = bind(fd, (struct sockaddr*)&addr, sizeof(addr));
    if (rv) {
        die("bind()");
    }



    //listen
    rv = listen(fd, SOMAXCONN); // SOMAXCONN is a constant that represents the maximum number of connections that can be queued
    if (rv) {
        die("listen()");
    }




    //Accept Connections
    while (true) {


        struct sockaddr_in client_addr = {};
        socklen_t addrlen = sizeof(client_addr);
        int connfd = accept(fd, (struct sockaddr*)&client_addr, &addrlen);
        if (connfd < 0) {
            continue; // error
        }
        
        do_something(connfd);
        close(connfd);
    }


    return 0;
}