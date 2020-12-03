#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>


int main(int argc, char* argv[])
{
    /*
        hints -> Infos recebidas pelo DNS dado pelo usuário
        *res  -> Ponteiro para o primeiro resultado da pesquisa
        *p    -> Ponteiro auxiliar
    */
    struct addrinfo hints, *res, *p;

    int status;                     // Recebe o retorno de getaddrinfo (val. esperado: 0)
    char ipstr[INET6_ADDRSTRLEN];   // Buffer para string do ip

   
    if (argc != 2)  // Se não for passado precisamente 1 endereço, imprime erro
    {
        fprintf(stderr, "please info hostname\n");
        return 1;
    }

    if (strcmp(argv[1], " --help"))
    {
        printf("--help        \t| show this page\n");
        printf("showip adress \t| show ip(v4 and/or v6) of adress\n");
        return 0;
    }

    
    memset(&hints, 0, sizeof(hints));   // Zerando valores de hints
    hints.ai_family = AF_UNSPEC;        // IPV4 ou IPV6
    hints.ai_socktype = SOCK_STREAM;     // Socket tipo TCP

    if ((status = getaddrinfo(argv[1], NULL, &hints, &res)) != 0) // Se não encontrar, imprime erro
    {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        return 2;
    }

    // Imprime cabeçalho da resposta
    printf("%s IP(s) adress:\n\n", argv[1]);
    
    for (p=res; p!=NULL; p=p->ai_next)  // Para cada IP encontrado (IPv4, IPv6)
    {
        void *addr;     // Ponteiro para endereço
        char *ipver;    // String de versão do IP

        // Busca as infos de acordo com o protocolo
        if (p->ai_family == AF_INET) // Se o protocolo for IPV4
        {
            struct sockaddr_in *ipv4 = (struct sockaddr_in *) p->ai_addr;
            addr = &(ipv4->sin_addr);
            ipver = "IPv4";
        }
        else // Senão, o protocolo é IPV6
        {
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
            addr = &(ipv6->sin6_addr);
            ipver = "IPv6";
        }

        // Converte as estruturas em número e imprime
        inet_ntop(p->ai_family, addr, ipstr, sizeof(ipstr));
        printf("\t %s: %s", ipver, ipstr);
    }

    printf("\n");       // Imprime '\n'
    freeaddrinfo(res);  // Libera lista

    return 0;
}