
#ifndef CLI_H
#define CLI_H

#include <stdbool.h>

//---------------- Public API ----------------------//

/// Convenience container for processed args.
#define MAX_CLI_ARGS 10
#define MAX_CLI_ARG_LEN  32 // incl terminator
typedef struct
{
    // how many arg_values. 0 indicates error string in arg_values[0].
    int arg_count;
    char arg_values[MAX_CLI_ARGS][MAX_CLI_ARG_LEN];
} cli_args_t;


/// Open a cli using stdio.
/// @return status.
int cli_OpenStdio(void);

/// Open a cli using socket.
/// @param host
/// @param port
/// @return status.
int cli_OpenSocket(const char* host, int port);

/// Open a cli using serial port.
/// @param port
/// @param baudrate
/// @return status.
int cli_OpenSerial(int port, int baudrate);

/// Clean up component resources.
/// @return status.
int cli_Destroy(void);

/// Read a line from a cli. This does not block. Buffers chars until EOL.
/// @param args User supplied collection.
/// @return status | ENODATA.
int cli_ReadLine(cli_args_t* args);

/// Write a line to a cli.
/// @param buff Line to send to user.
/// @return status.
int cli_WriteLine(const char* format, ...);

/// Write a car to a cli.
/// @param c Char to send to user.
/// @return status.
int cli_WriteChar(char c);

#endif // CLI_H
