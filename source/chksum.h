#ifndef CHKSUM_H
#define CHKSUM_H

#include <stdint.h>
#include <stdbool.h>

//---------------- Public API ----------------------//

/// Generate a crc32.
/// @param crc  starting value.
/// @param buf  calc values.
/// @param len  size of buf.
/// @param If the calced crc ends up being 0, change this to 1.
///   This adds extremely small additional error but makes it handy in this application.
uint32_t chksum_crc32(uint32_t crc, const uint8_t* buf, size_t len, bool adjust_zero);

#endif // CHKSUM_H