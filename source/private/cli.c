
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <conio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include "cli.h"


// TOCO1 handle e.g. immediate single space bar.


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
    int stat = 0;
    _buff_index = -1;
    _iftype = IF_STDIO;
    _prompt = "$";

    memset(_cli_buff, 0, CLI_BUFF_LEN);

    // Prompt.
    cli_WriteLine("");
    return stat;
}


//--------------------------------------------------------//
int cli_OpenSocket(const char* ip, int port)
{
    int stat = 0;
    _buff_index = -1;
    _iftype = IF_SOCKET;
    _prompt = "";

    memset(_cli_buff, 0, CLI_BUFF_LEN);

    // Prompt.
    cli_WriteLine("");
    return stat;
}


//--------------------------------------------------------//
int cli_OpenSerial(int port, int baudrate)
{
    int stat = 0;
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
    int stat = 0;

    switch(_iftype)
    {
        case IF_STDIO:
            break;
        case IF_SOCKET:
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
                c = _kbhit() ? (char)_getch() : -1;
                break;
            case IF_SOCKET:
                // telnet - see sock.c
                // while ((c = fgetc(p_CliIn)) != EOF)
                // if (fread(&c, 1, 1, p_CliIn) > 0)
                c = -1;
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
    int stat = 0;

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
    int stat = 0;

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
