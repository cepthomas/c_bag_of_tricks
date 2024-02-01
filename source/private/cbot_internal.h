#ifndef CBOT_INTERNAL_H
#define CBOT_INTERNAL_H

#include <string.h>
#include "logger.h"


/// Validate pointer. If fails, returns early with (typed) err. Typically used for function args.
#define VAL_PTR(ptr, err) { if(ptr == NULL) { LOG_ERROR("VAL_PTR: %s", __FILE__); return err; } }


#endif // CBOT_INTERNAL_H
