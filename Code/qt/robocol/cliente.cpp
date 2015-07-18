#include "cliente.h"
#include <QDebug>

ssize_t readLine(int fd, char *buffer1, size_t n){
    ssize_t numRead;                    /* # of bytes fetched by last read() */
    size_t totRead;                     /* Total bytes read so far */
    char *buf;
    char ch;

    if (n <= 0 || buffer1 == NULL) {
        errno = EINVAL;
        return -1;
    }

    buf = buffer1;                       /* No pointer arithmetic on "void *" */

    totRead = 0;
    for (;;) {
        numRead = read(fd, &ch, 1);

        if (numRead == -1) {
            if (errno == EINTR)         /* Interrupted --> restart read() */
                continue;
            else
                return -1;              /* Some other error */

        } else if (numRead == 0) {      /* EOF */
            if (totRead == 0)           /* No bytes read; return 0 */
                return 0;
            else                        /* Some bytes read; add '\0' */
                break;

        } else {                        /* 'numRead' must be 1 if we get here */
            if (totRead < n - 1) {      /* Discard > (n - 1) bytes */
                totRead++;
                *buf++ = ch;
            }

            if (ch == '\n')
                break;
        }
    }

    *buf = '\0';
    return totRead;
}

int conectarServidor(char *ip_servidor)
{
    int cfd;

    struct addrinfo hints;
    struct addrinfo *result, *rp;

    /* Call getaddrinfo() to obtain a list of addresses that
        we can try connecting to */

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;
    hints.ai_family = AF_UNSPEC;                /* Allows IPv4 or IPv6 */
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_NUMERICSERV;

    if (getaddrinfo(ip_servidor, PORT_NUM, &hints, &result) != 0)
        qDebug()<<"getaddrinfo";


    /* Walk through returned list until we find an address structure
        that can be used to successfully connect a socket */

    for (rp = result; rp != NULL; rp = rp->ai_next) {
        cfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (cfd == -1)
            continue;                           /* On error, try next address */

        if (connect(cfd, rp->ai_addr, rp->ai_addrlen) != -1)
            break;                              /* Success */

        /* Connect failed: close this socket and try next address */

        close(cfd);
    }

    if (rp == NULL)
    {
        qDebug()<<"Could not connect socket to any address";
        return -1;
    }

    freeaddrinfo(result);
    return cfd;
}

int enviarComando(char* comando,int cfd)
{
    if (write(cfd, comando, strlen(comando)) == -1)
    {
        qDebug()<<"Partial/failed write (reqLenStr)";
        return -1;
    }
    if (write(cfd, "\n", 1) != 1)
    {
        qDebug()<<"Partial/failed write (newline)";
        return -1;
    }
    return 0;
}

int cerrarConexion(int sfd)
{
    //Cerrar archivo
    if(close(sfd) == -1){
        qDebug()<<"close";
        return -1;
    }
    return 0;
}


