#include "lunarity/include/location.h"

lunarity_byte_location_t lunarity_new_byte_location(size_t line, size_t column,
                                                    size_t offset) {
  lunarity_byte_location_t location;
  location.line = line;
  location.column = column;
  location.offset = offset;
  return location;
}

lunarity_span_t lunarity_new_span(lunarity_byte_location_t start,
                                  lunarity_byte_location_t end) {
  lunarity_span_t span;
  span.start = start;
  span.end = end;
  return span;
}

lunarity_span_t
lunarity_new_single_byte_span(lunarity_byte_location_t byte_location) {
  lunarity_span_t span;
  span.start = byte_location;

  span.end.line = byte_location.line;
  span.end.column = byte_location.column + 1;
  span.end.offset = byte_location.offset + 1;

  return span;
}

lunarity_span_t
lunarity_new_double_byte_span(lunarity_byte_location_t byte_location) {
  lunarity_span_t span;
  span.start = byte_location;

  span.end.line = byte_location.line;
  span.end.column = byte_location.column + 2;
  span.end.offset = byte_location.offset + 2;

  return span;
}

lunarity_span_t
lunarity_new_triple_byte_span(lunarity_byte_location_t byte_location) {
  lunarity_span_t span;
  span.start = byte_location;

  span.end.line = byte_location.line;
  span.end.column = byte_location.column + 3;
  span.end.offset = byte_location.offset + 3;

  return span;
}

size_t lunarity_span_size(lunarity_span_t span) {
  return span.end.offset - span.start.offset;
}
