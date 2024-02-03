
#ifndef CLI_H
#define CLI_H

#include <stdbool.h>

//---------------- Public API ----------------------//

//---------------- Types ---------------------------//

/// Flavor of connection.
typedef enum { IF_NONE, IF_STDIO, IF_SOCKET, IF_SERIAL } iftype_t;

/// Convenience container for processed args.
#define MAX_CLI_ARGS    10
#define MAX_CLI_ARG_LEN 32 // incl terminator
typedef struct
{
    /// How many arg_values. 0 indicates error string in arg_values[0].
    int arg_count;
    /// The actual values as strings.
    char arg_values[MAX_CLI_ARGS][MAX_CLI_ARG_LEN];
} cli_args_t;

/// Cli command descriptor.
typedef struct cli_command_desc
{
    /// If you like to type.
    const char* long_name;
    /// If you don't.
    const char* short_name;
    /// Optional single char for immediate execution (no return). Can be prefixed with ^(ctrl) or ~(alt).
    const char* immediate_key;
    /// Free text for description.
    const char* info;
    /// Free text for description.
    const char* args;
} cli_command_desc_t;

// Cli command handler.
typedef int (* cli_command_handler_t)(const cli_command_desc_t* pcmd, cli_args_t* args);

/// Bind a cli command to its handler.
typedef struct cli_command
{
    const cli_command_handler_t handler;
    const cli_command_desc_t desc;
} cli_command_t;


//---------------- Functions -----------------------//

/// Open a cli using stdio.
/// @return status.
int cli_Open(iftype_t iftype, cli_command_t _commands[]);

/// Clean up component resources.
/// @return status.
int cli_Destroy(void);

/// Cycle the periodic cli processing.
/// @return status.
int cli_Pump(void);

// /// Read a line from a cli. This does not block. Buffers chars until EOL.
// /// @param args User supplied collection.
// /// @return status | ENODATA.
// int cli_ReadLine(cli_args_t* args);

/// Write a line to a cli.
/// @param buff Line to send to user.
/// @return status.
int cli_WriteLine(const char* format, ...);

/// Write a car to a cli.
/// @param c Char to send to user.
/// @return status.
int cli_WriteChar(char c);

#endif // CLI_H
