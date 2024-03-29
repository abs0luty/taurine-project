#ifndef _LUNARITY_LOCATION_H_
#define _LUNARITY_LOCATION_H_

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  size_t line, column, offset;
} lunarity_byte_location_t;

/**
 * @param   line the line number
 * @param   column the column number
 * @param   offset the byte offset
 * @returns A new location with the given line, column, and offset.
 * @version 0.1.0
 */
lunarity_byte_location_t lunarity_new_byte_location(size_t line, size_t column,
                                                    size_t offset);

typedef struct {
  lunarity_byte_location_t start, end;
} lunarity_span_t;

/**
 * @param   start the location of the first byte in the span
 * @param   end the location of the last byte in the span
 * @returns A new span with the given start and end locations.
 * @version 0.1.0
 */
lunarity_span_t lunarity_new_span(lunarity_byte_location_t start,
                                  lunarity_byte_location_t end);

/**
 * @brief   Create a new span containing a single byte
 * @param   byte_location the location of a single byte
 * @returns A new span containing the single byte
 * @version 0.1.0
 */
lunarity_span_t
lunarity_new_single_byte_span(lunarity_byte_location_t byte_location);

/**
 * @brief   Create a new span containing two bytes
 * @param   byte_location the location of two bytes
 * @returns A new span containing two bytes
 * @version 0.1.0
 */
lunarity_span_t
lunarity_new_double_byte_span(lunarity_byte_location_t byte_location);

/**
 * @brief   Create a new span containing three bytes
 * @param   byte_location the location of three bytes
 * @returns A new span containing three bytes
 * @version 0.1.0
 */
lunarity_span_t
lunarity_new_triple_byte_span(lunarity_byte_location_t byte_location);

/**
 * @param   span the span
 * @returns The size of the span in bytes.
 * @version 0.1.0
 */
size_t lunarity_span_size(lunarity_span_t span);

#ifdef __cplusplus
}
#endif

#endif /* _LUNARITY_LOCATION_H_ */
