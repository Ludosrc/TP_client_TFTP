#include <string.h>
#include <netdb.h>
#include <unistd.h>
#include "header.h"


int main(int argc, char ** argv) {
    printf("Hello, World!\n");

    // On a un client et un serveur
    // On fait un sendto entre (WRQ) du port X au 1069 du serveur
    // Puis le serveur renvoie l'acquitemment depuis le port Y (pas le 1069)
    // On utilise wireshark pour trouver verifier que l'on reçoit bien une requete d'aquitement
    // verif de l'erreur sur le nombre d'arguments
    // c'est en deux temps : demande d'écriture/acquittement et si oui envoie de data/acquittement

    if(argc!=4){ //on a  4 arguments send localhost 1069 et \file
        printf("Mauvaise utilisation, \nPLease use %s hostname port file", argv[0]);
        exit(EXIT_FAILURE);
    }
    printf("Bienvenue sur l'implementation d'un tftp client de Loudz et Moun\n");

        FILE *inputFile;
        inputFile = fopen("/home/ludovic/Documents/serveur/ones256", "r");
        int caractereActuel = 0;



        if (inputFile != NULL) {
            // Boucle de lecture des caractères un à un
            do {
                caractereActuel = fgetc(inputFile); // On lit le caractère
                printf("%c", caractereActuel); // On l'affiche
            } while (caractereActuel != EOF); // On continue tant que fgetc n'a pas retourné EOF (fin de fichier)

            fclose(inputFile);
        }

        if (inputFile == NULL) {
            printf("Cannot open file %s\n", argv[0]);
            exit(EXIT_FAILURE);
        }

        if (argc != 4) {
            printf("Wrong usage .\nPlease use %s hostname port file", argv[0]);
            exit(EXIT_FAILURE);

        }


        struct addrinfo *result;
        struct addrinfo hints; // type de structure utile pour envoyer un fichier
        memset(&hints, 0, sizeof(struct addrinfo));// type de structure utile pour envoyer un fichier
        hints.ai_family = AF_INET;// Choix D'IP -> IPV4
        hints.ai_protocol = IPPROTO_UDP;// Protocole udp

        int status = getaddrinfo(argv[1], argv[2], &hints, &result);

        if (status != 0) {
            fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
            exit(EXIT_FAILURE);
        }
        if (status == -1) {
            printf("Cannot find host %s", argv[1]);
        }

        printf("sending %s to the host :%s@%s\n", argv[3], argv[1], argv[2]);


        //Création de la trame
        char buffer[MAXSIZE] = {0};
        buffer[1] = 2; //2 pour le mode send
        sprintf(buffer + 2, "%s", argv[3]); //nom du fichier
        sprintf(buffer + 3 + strlen(argv[3]), "NETASCII"); //type d'écriture

        //création du socket
        int sock = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
// rappel socket = flux de données permettant la com entre des machines via des protocoles (udp ou tcp)

        //Test création de la socket
        if (sock == -1) {
            perror("socket error");
            exit(EXIT_FAILURE);
        }

        //envoie de la trame
        sendto(sock, buffer, 12 + strlen(argv[3]), 0, result->ai_addr, result->ai_addrlen);

        //Receive du prof
        int numberReadBytes;
        char receiveBuffer[MAXSIZE] = {0};
        struct sockaddr serv_addr;
        int serv_addr_len;


        numberReadBytes = recvfrom(sock, receiveBuffer, MAXSIZE, 0, &serv_addr, &serv_addr_len);

        if (receiveBuffer[1] == 5) {
            printf("Error code received from the server : \n%s\n", receiveBuffer + 4);
            exit(EXIT_SUCCESS);
        }


        if (receiveBuffer[1] == 3) {
            printf("Error code received from the server : \n%s\n", receiveBuffer + 4);
            exit(EXIT_SUCCESS);
        }

        /* NOTRE RECEIVE : Mystérieusement ne marchait plus après la pause entraînant un craquage complet du demi-binôme
         *
             char buffer2[MAX_SIZE] = {0}; //2 emplacement mémoire
            struct sockaddr exp_addr;// struct pour info de l'expéditeur(serveur Y)
            socklen_t exp_addr_len;
            ssize_t nread; //taille du message
            char host[NI_MAXHOST], service[NI_MAXHOST]; //pour avoir les noms de l'epéditeur et du receveur
            //le & permet d'avoir l'adresse mémoire

            if( result != NULL){
                recvfrom(sock, buffer2, 12+strlen(argv[3]), 0, (struct sockaddr *) &exp_addr, &exp_addr_len);
                printf("On a reçu %zd octets de %s:%s\n", nread, host, service);
            }

    // Construction de recvfrom : flux de donnée, pointeur de tampon mémoire, taille de la mémoire, adresse de l'expéditeur(qui change tout le temps), taille de cette adresse
         */

        char buffersend[MAXSIZE] = {0};
        buffersend[1] = 3;
        sprintf(buffersend + 2, "%d", 1); //nom du fichier
        sprintf(buffersend + 3, "11111111111111111111111111111"); //type d'écriture
        sendto(sock, buffersend, 4 + strlen("11111111111111111111111111"), 0, &serv_addr, serv_addr_len);


    //si pas d'acquittement, il continue à envoyer
    // première étape de demandewrite ok


    // Deuxième étape le chargement de donné (après acceptation et création d'un emplacement)


    //ouverture du fichier pour pouvoir l'envoyer et lecture


        char buffer3[MAXSIZE];
        FILE * ones256;
        int n;
        ones256= fopen(argv[3],'r' );

        if (ones256 == NULL){
            printf("erreur d'ouverture\n");
            return 1;
        }

        n = fread(buffer3, sizeof(char), MAXSIZE, ones256);

        if (n != MAXSIZE) {
           printf("Error reading file!\n");
           return 1;
        }

        printf("%s", buffer3);
        return EXIT_SUCCESS;



}