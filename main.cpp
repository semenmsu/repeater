#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> // for sleep()
#include <fstream>
#include <sstream>
#include <set>
#include <ctime>
#include <vector>

#define BUFSIZE 65000
#define PORT 15001

void error(char *msg) {
    perror(msg);
    exit(0);
}

class Client {
    public:

    Client(int port) {
        port_ = port;
        slen_ = sizeof(sockaddr_);
        memset((char *) &sockaddr_, 0, sizeof(sockaddr_));
        sockaddr_.sin_family = AF_INET;
        sockaddr_.sin_port = htons(port);
        char * host = (char *)"127.0.0.1";
        if(inet_aton(host, &sockaddr_.sin_addr) == 0){
            fprintf(stderr,"socket error");
            exit(1);
        }

        if((socket_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
            fprintf(stderr,"socket error");
            exit(1);
        }
    }

    void Send(char *buffer, int len) {
        if(sendto(socket_, (char*)buffer, len, 0, (struct sockaddr *)&sockaddr_, slen_) == -1){
            fprintf(stderr,"socket error");
            exit(1);
        }
    }

    ~Client() {
        close(socket_);
    }

    private:
    
    int port_;
    int socket_;
    struct sockaddr_in sockaddr_;
    int slen_;
};

std::vector<int> GetPorts(std::string &ports_list) {
    std::vector<int> ports;
    std::istringstream iss(ports_list);
    std::string value;
    while(std::getline(iss, value, ',')) {
        ports.push_back(std::atoi(value.c_str()));
    }
    return ports;

}


// how to run: bin/main -l 15001 -p 15002,15003
int main(int argc, char** argv) {

    int opt;
    int listen_port;
    std::string ports_list;
    while ((opt = getopt(argc, argv, "l:p:")) != -1) {
        switch (opt) {
        case 'l':
            listen_port = std::atoi(optarg);
            break;
        case 'p':
            ports_list = optarg;
            break;
        default: /* '?' */
            fprintf(stderr, "Usage: %s [-l listen_port] [-p] port_where_to_repeat\n",
                    argv[0]);
            exit(EXIT_FAILURE);
        }
    }
    auto ports = GetPorts(ports_list);

    int sockfd; 
    char buffer[BUFSIZE]; 
    struct sockaddr_in     servaddr; 
    struct sockaddr_in sendto_address;

    std::vector<Client*> clients;

    for (int port: ports) {
        auto client = new Client(port);
        clients.push_back(client);
    }

    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
    
    memset(&servaddr, 0, sizeof(servaddr)); 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_port = htons(PORT); 
    //servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sockfd, (struct sockaddr*) &servaddr, sizeof(servaddr)) < 0) {
		perror("bind");
		return 1;
	}

    while (true) {
        int len;
        int numbytes = recvfrom(sockfd, (char *)buffer, BUFSIZE,  
                    0, (struct sockaddr *) &servaddr, 
                    (socklen_t*)&len); 

        if (numbytes < 0) {
            perror("receive");
            exit(-1);
        }
        buffer[numbytes] = '\0'; 
        printf("Server : %s\n", buffer); 

        for (auto & client : clients) {
            client->Send(buffer, numbytes);
        }
    }
    
    close(sockfd); 
    return 0; 
}