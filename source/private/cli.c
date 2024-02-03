#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <winsock2.h>
#include <Ws2tcpip.h>
#include <conio.h>

#include "cbot.h"
#include "cli.h"


// TODO1 [history](C:\Dev\AL\harvester\xib-firmware\vendor\appliedlogix\cli-library\cli\cli_interpreter.c)

// FUTURE support nx, maybe ncurses.


//---------------- Private ------------------------------//

/// Max raw line.
#define CLI_BUFF_LEN 128

/// CLI buffer to collect input chars.
static char _cli_buff[MAX_CLI_ARGS * MAX_CLI_ARG_LEN];

/// Buff status. -1 means empty.
static int _buff_index = -1;

/// CLI prompt.
static char* _prompt = "";

/// The flavor.
static iftype_t _iftype = IF_NONE;

/// The command list.
static cli_command_t* _commands;

/// Local function.
static int _ReadLine(cli_args_t* args);

/// Local function.
static int _Usage(void);


//---------------- API Implementation -----------------//


//--------------------------------------------------------//
int cli_Open(iftype_t iftype, cli_command_t commands[])
{
    int stat = CBOT_ERR_NO_ERR;

    _iftype = iftype;
    _commands = commands;
    _buff_index = -1;
    memset(_cli_buff, 0, CLI_BUFF_LEN);

    switch(_iftype)
    {
        case IF_STDIO:
            _prompt = "$";
            break;
        case IF_SOCKET:
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
            break;
        case IF_SERIAL:
            _prompt = "$";
            break;
        default:
            break;
    }

    // Prompt.
    cli_WriteLine("");
    return stat;
}


//--------------------------------------------------------//
int cli_Destroy(void)
{
    int stat = CBOT_ERR_NO_ERR;

    switch(_iftype)
    {
        case IF_STDIO:
            // Nothing.
            break;
        case IF_SOCKET:
            // if (_fd > 0) { close(_fd); }
            break;
        case IF_SERIAL:
            break;
        default:
            break;
    }

    return stat;
}


//--------------------------------------------------------//
int cli_Pump(void)
{
    int stat = CBOT_ERR_NO_ERR;
    int cbot_stat = CBOT_ERR_NO_ERR;
    cli_args_t cli_args;

    cbot_stat = _ReadLine(&cli_args);
    // _EvalStatus(cbot_stat, "Failed to _ReadLine");

    switch (cbot_stat)
    {
    case CBOT_ERR_NO_ERR: // something to do
        bool valid = false;

        // Process the command and its options.
        if (cli_args.arg_count > 0)
        {
            // Find and execute the command.
            const cli_command_t* pcmd = _commands;
            while (pcmd->handler != NULL)
            {
                if (strcmp(pcmd->desc.short_name, cli_args.arg_values[0]) || strcmp(pcmd->desc.long_name, cli_args.arg_values[0]))
                {
                    valid = true;
                    // Lock access to lua context.
                    //ENTER_CRITICAL_SECTION;
                    // Execute the command. They handle any errors internally.
                    stat = (*pcmd->handler)(&(pcmd->desc), &cli_args);
                    // _EvalStatus(stat, "handler failed: %s", pcmd->desc.long_name);
                    //EXIT_CRITICAL_SECTION;
                    break;
                }
            }

            if (!valid)
            {
                cli_WriteLine("invalid command");
            }
        }
        break;

    case CBOT_ERR_NO_DATA: // nothing to do
        break;

    default: // error
        cbot_stat = cli_WriteLine(cli_args.arg_values[0]);
        // _EvalStatus(cbot_stat, "cli_WriteLine() error");
        break;
    }

    return stat;
}


//--------------------------------------------------------//
int cli_WriteLine(const char* format, ...)
{
    int stat = CBOT_ERR_NO_ERR;

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
    int stat = CBOT_ERR_NO_ERR;

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


//--------------------------------------------------------//
int _ReadLine(cli_args_t* args)
{
    // TODO1 handle immediate key e.g. space bar to start/stop, maybe ctrl-space? 
    // TODO1 { _Usage, { "help",       "?",   "",   "explain it all", "" } },

    bool line_done = false;
    char* serr = NULL;

    // Process each new char.
    char c = -1;
    bool ctrl = false;
    bool alt = false;
    bool chars_done = false;

    while (!chars_done)
    {
        switch(_iftype)
        {
            case IF_STDIO:
                {
                    if (_kbhit())
                    {
                        c = (char)_getch();
                        //TODO1 ctrl/alt? GetAsyncKeyState()
                    }
                    else
                    {
                        c = -1;
                    }
                }
                break;
            case IF_SOCKET:
                c = -1;
                // int num_read = read(fd, &c, 1);
                // if (num_read == 1) { } // valid
                // else if (num_read == -1)
                // {
                //     if (errno == EINTR) { c = -1; } // Interrupted --> restart read
                //     else { return -1; } // Some other error
                // }
                // else if (num_read == 0) { return 0; } // EOF
                break;
            case IF_SERIAL:
                break;
            default:
                break;
        }

        // What happened?
        switch(c)
        {
            case -1:
                chars_done = true;
                break;
            case '\n':
                // Ignore.
                break;
            case '\r':
                // Echo return.
                cli_WriteLine("");
                // Line chars_done.
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
            _buff_index = 0;
        }
    }

    return line_done ? CBOT_ERR_NO_ERR : CBOT_ERR_NO_DATA;
}


//--------------------------------------------------------//
int _Usage(void)
{
    int stat = CBOT_ERR_NO_ERR;

    const cli_command_t* cmditer = _commands;
    while (_commands->handler != NULL_PTR)
    {
        const cli_command_desc_t* pdesc = &(cmditer->desc);
        cli_WriteLine("%s|%s: %s", pdesc->long_name, pdesc->short_name, pdesc->info);
        if (strlen(pdesc->args) > 0)
        {
            // Maybe multiline args. Make writable copy and tokenize it.
            char cp[128];
            strncpy(cp, pdesc->args, sizeof(cp));
            char* tok = strtok(cp, "$");
            while (tok != NULL)
            {
                cli_WriteLine("    %s", tok);
                tok = strtok(NULL, "$");
            }
        }
        cmditer++;
    }

    return stat;
}
