#include "../include/location.h"

lunarity_byte_location_t lunarity_new_byte_location(
  unsigned int line,
  unsigned int column,
  unsigned int offset)
{
  lunarity_byte_location_t location;
  location.line = line;
  location.column = column;
  location.offset = offset;
  return location;
}

lunarity_span_t lunarity_new_span(
  lunarity_byte_location_t start,
  lunarity_byte_location_t end)
{
  lunarity_span_t span;
  span.start = start;
  span.end = end;
  return span;
}

unsigned int lunarity_span_size(lunarity_span_t span)
{
  return span.end.offset - span.start.offset;
}
