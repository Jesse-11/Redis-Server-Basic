#include <stdint.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>



//Display error message and exit syscall
static void die(const char *msg) {
    int err = errno;
    fprintf(stderr, "[%d] %s\n", err, msg);
    abort();
}




int main () {

    int fd = socket(AF_INET, SOCK_STREAM, 0); // AF_INET = IPv4, SOCK_STREAM = TCP
    if (fd < 0) {
        die("socket()");
    }



    struct sockaddr_in addr = {}; // holds IPv4 adress and port
    addr.sin_family = AF_INET; // IPv4
    addr.sin_port = ntohs(1234); // port 1234
    addr.sin_addr.s_addr = ntohl(INADDR_LOOPBACK); // 127.0.0.1
    int rv = connect(fd, (struct sockaddr*)&addr, sizeof(addr)); // connect to server
    if (rv) {
        die("connect()");
    }


    // write message to server
    char msg[] = "hello";
    write(fd, msg, sizeof(msg));


    // read message from server
    char rbuf[64] = {}; // read buffer
    ssize_t n = read(fd, rbuf, sizeof(rbuf) -1); // read message from server
    if (n < 0) {
        die("read()");
    }
    printf("server says: %s\n", rbuf); // print message from server
    close(fd); // close connection
    return 0;


}

