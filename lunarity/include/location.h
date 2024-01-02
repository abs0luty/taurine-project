#include <cstddef>
#ifndef _LUNARITY_LOCATION_H_
#define _LUNARITY_LOCATION_H_

typedef struct
{
  unsigned int line;
  unsigned int column;
  unsigned int offset;
} lunarity_byte_location_t;

/**
 * @param   line the line number
 * @param   column the column number
 * @param   offset the byte offset
 * @return  A new location with the given line, column, and offset.
 * @version 0.1.0
*/
lunarity_byte_location_t lunarity_new_byte_location(
    unsigned int line,
    unsigned int column,
    unsigned int offset);

typedef struct {
  lunarity_byte_location_t start, end;
} lunarity_span_t;

/**
 * @param   start the location of the first byte in the span
 * @param   end the location of the last byte in the span
 * @return  A new span with the given start and end locations.
 * @version 0.1.0
 */
lunarity_span_t lunarity_new_span(
    lunarity_byte_location_t start,
    lunarity_byte_location_t end);

/**
 * @param   span the span
 * @return  The size of the span in bytes.
 * @version 0.1.0
 */
unsigned int lunarity_span_size(lunarity_span_t span);

#endif /* _LUNARITY_LOCATION_H_ */
