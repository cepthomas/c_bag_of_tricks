#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <winsock2.h>
#include <Ws2tcpip.h>
#include <conio.h>
#include <errno.h>

#include "status.h"
#include "cli.h"


// TODO1 handle e.g. immediate single space bar.


//---------------- Private ------------------------------//

/// Max raw line.
#define CLI_BUFF_LEN 128

typedef enum { IF_NONE, IF_STDIO, IF_SOCKET, IF_SERIAL } ifType_t;

/// CLI buffer to collect input chars.
static char _cli_buff[MAX_CLI_ARGS * MAX_CLI_ARG_LEN];

/// Buff status. -1 means empty.
static int _buff_index = -1;

/// CLI prompt.
static char* _prompt = "";

/// The flavor.
static ifType_t _iftype = IF_NONE;


//---------------- API Implementation -----------------//


//--------------------------------------------------------//
int cli_OpenStdio(void)
{
    int stat = ENOERR;
    _buff_index = -1;
    _iftype = IF_STDIO;
    _prompt = "$";

    memset(_cli_buff, 0, CLI_BUFF_LEN);

    // Prompt.
    cli_WriteLine("");
    return stat;
}


//--------------------------------------------------------//
int cli_OpenSocket(const char* host, int port) //FUTURE
{
    int stat = ENOERR;
    _buff_index = -1;
    _iftype = IF_SOCKET;
    _prompt = "";

    memset(_cli_buff, 0, CLI_BUFF_LEN);

    // Prompt.
    cli_WriteLine("");

    // // Connect.
    // struct hostent* h = gethostbyname(host);
    // if(!h) { return 1; }
    // char* ip = inet_ntoa(*(struct in_addr*)h->h_addr);
    // _fd = socket(AF_INET, SOCK_STREAM, 0);
    // if (_fd < 0) { return 2; }
    // struct sockaddr_in saddr;
    // memset(&saddr, 0, sizeof(saddr));
    // saddr.sin_family = AF_INET;
    // saddr.sin_port = htons(port);
    // inet_aton(ip, &saddr.sin_addr);
    // stat = connect(fd, (struct sockaddr*)&saddr, sizeof(saddr));
    // if (stat < 0) { return 3; }

    return stat;
}


//--------------------------------------------------------//
int cli_OpenSerial(int port, int baudrate) // FUTURE
{
    int stat = ENOERR;
    _buff_index = -1;
    _iftype = IF_SERIAL;
    _prompt = "$";

    memset(_cli_buff, 0, CLI_BUFF_LEN);

    // Prompt.
    cli_WriteLine("");
    return stat;
}


//--------------------------------------------------------//
int cli_Destroy(void)
{
    int stat = ENOERR;

    switch(_iftype)
    {
        case IF_STDIO:
            // Nothing.
            break;
        case IF_SOCKET:
            // if (_fd > 0)
            // {
            //     close(_fd);
            // }
            break;
        case IF_SERIAL:
            break;
        default:
            break;
    }

    return stat;
}


//--------------------------------------------------------//
const bool cli_ReadLine(cli_args_t* args)
{
    bool line_done = false;
    char* serr = NULL;

    // Process each new char.
    char c = -1;
    bool process_done = false;

    while (!process_done)
    {
        switch(_iftype)
        {
            case IF_STDIO:
                {
                    c = _kbhit() ? (char)_getch() : -1;
                }
                break;
            case IF_SOCKET:
                c = -1;
                // {
                //     int num_read = read(fd, &c, 1);
                //     if (num_read == 1)
                //     {
                //         // A char is read.
                //     }
                //     else if (num_read == -1)
                //     {
                //         if (errno == EINTR) // Interrupted --> restart read
                //         {
                //             c = -1;
                //         }
                //         else // Some other error
                //         {
                //             return -1;              
                //         }
                //     }
                //     else if (num_read == 0) // EOF
                //     {      
                //         return 0;
                //     }
                // }
                break;
            case IF_SERIAL:
                break;
            default:
                break;
        }

        switch(c)
        {
            case -1:
                process_done = true;
                break;

            case '\n':
                // Ignore.
                break;

            case '\r':
                // Echo return.
                cli_WriteLine("");

                // Line process_done.
                line_done = true;

                // Echo prompt.
                //cli_WriteLine("");
                break;

            default:
                // Echo char.
                cli_WriteChar(c);
                
                // Save it.
                _cli_buff[_buff_index++] = c;

                // Check for overrun.
                if (_buff_index >= CLI_BUFF_LEN - 1)
                {
                    // Notify.
                    serr = "Command line too long";
                    line_done = true;
                }
                break;
        }

        if (line_done)
        {
            memset(args, 0, sizeof(cli_args_t));

            if (serr == NULL)
            {
                int argc = 0;

                // Chop up the raw command line into args.
                char* tok = strtok(_cli_buff, " ");
                while(tok != NULL && argc < MAX_CLI_ARGS && serr == NULL)
                {
                    if (strlen(tok) >= MAX_CLI_ARG_LEN)
                    {
                        serr = "Argument too long";
                    }
                    else
                    {
                        strncpy(args->arg_values[argc], tok, MAX_CLI_ARG_LEN-1);
                        argc++;
                        tok = strtok(NULL, " ");
                    }
                }
            }

            if (serr != NULL)
            {
                strncpy(args->arg_values[0], serr, MAX_CLI_ARG_LEN-1);
                args->arg_count = 0;
            }

            // Clear buffer.
            memset(_cli_buff, 0, CLI_BUFF_LEN);
            line_done = false;
            _buff_index = 0;
        }

    }

    return line_done;
}


//--------------------------------------------------------//
int cli_WriteLine(const char* format, ...)
{
    int stat = ENOERR;

    static char buff[CLI_BUFF_LEN];

    va_list args;
    va_start(args, format);
    vsnprintf(buff, CLI_BUFF_LEN-1, format, args);
    va_end(args);

    switch(_iftype)
    {
        case IF_STDIO:
            printf("%s\r\n>", buff);
            break;
        case IF_SOCKET:
            break;
        case IF_SERIAL:
            stat = 1;
            break;
        default:
            stat = 1;
            break;
    }

    return stat;    
}


//--------------------------------------------------------//
int cli_WriteChar(char c)
{
    int stat = ENOERR;

    switch(_iftype)
    {
        case IF_STDIO:
            putchar(c);
            break;
        case IF_SOCKET:
            // fputc();
            break;
        case IF_SERIAL:
            stat = 1;
            break;
        default:
            stat = 1;
            break;
    }
    
    return stat;    
}
