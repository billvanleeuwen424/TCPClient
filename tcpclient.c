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

    int socket_peer;
    if((socket_peer = socket(peer_address->ai_family, peer_address->ai_socktype, peer_address->ai_protocol)) < 0){
        fprintf(stderr, "Socket creation failure: %s\n", strerror(errno));
        return 1;
    }

    







}