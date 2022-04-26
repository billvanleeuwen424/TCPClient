#include "tcpclient.h"

int main(int argc, char *argv[]) {

    //take ip and port from cmd line arg
    if(argc < 3){
        fprintf(stderr, "usage: tcpclient [hostname] [port number]\n");
        return 1;
    }
    
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));   //zero out hints
    hints.ai_socktype = SOCK_STREAM;


    struct addrinfo *peer_address;

    //get address info, store in peer_address
    if(getaddrinfo(argv[1],argv[2], &hints, &peer_address)){
        fprintf(stderr, "get address info failed: %s\n", strerror(errno));
        return 1;
    }


    //print address info to console
    char addressBuff[25], serviceBuff[25];
    getnameinfo(peer_address->ai_addr, peer_address->ai_addrlen, addressBuff, sizeof(addressBuff), serviceBuff, sizeof(serviceBuff), NI_NUMERICHOST);
    printf("Connection successful. Address is: %s %s\n", addressBuff, serviceBuff);


    //create socket and error check
    int socket_peer;
    if((socket_peer = socket(peer_address->ai_family, peer_address->ai_socktype, peer_address->ai_protocol)) < 0){
        fprintf(stderr, "Socket creation failure: %s\n", strerror(errno));
        return 1;
    }

    //connect to server and error check
    if(connect(socket_peer, peer_address->ai_addr, peer_address->ai_addrlen)){
        fprintf(stderr, "Connection failure: %s\n", strerror(errno));
        return 1;
    }
    //free unneeded address mem
    freeaddrinfo(peer_address);

    printf("Connected Successfully.\n");
    printf("Enter text into terminal to send data.\n");


    while (1)
    {
        //create set for file descriptors. 
        fd_set reads;
        FD_ZERO(&reads);

        //add socket, stdin to set
        FD_SET(socket_peer, &reads);
        FD_SET(0, &reads);

        //100ms
        struct timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = 100000;

        //wait for readable data from fdset
        if(select(socket_peer+1, &reads, 0,0,&timeout) < 0){
            fprintf(stderr, "Select() failure: %s\n", strerror(errno));
            return 1;
        }

        //if the server has sent us anything, read it. else break.
        if(FD_ISSET(socket_peer, &reads)){
            char read[4096];
            int bytes_rec = recv(socket_peer, read, sizeof(read), 0);
            if(bytes_rec < 1){
                printf("Connection closed.\n");
                break;
            }
            printf("Recieved %d bytes from server.\n%.*s", bytes_rec, bytes_rec, read);
        }

        //if anything has been written to the terminal, read and send it. else break.
        if(FD_ISSET(0, &reads)){
            char read[4096];
            if(!fgets(read,sizeof(read), stdin)){
                break;
            }
            printf("Sending.\n");
            int bytes_sent = send(socket_peer, read, strlen(read), 0);
            printf("Sent %d bytes.\n", bytes_sent);
        }
    }

    printf("Closing socket.\n");
    close(socket_peer);
    
    return 0;
}