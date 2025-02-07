#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "uint256.h"

// Create a UInt256 value from a single uint32_t value.
// Only the least-significant 32 bits are initialized directly,
// all other bits are set to 0.
UInt256 uint256_create_from_u32( uint32_t val ) {
  UInt256 result = {0};  
  result.data[0] = val;
  return result;
}

// Create a UInt256 value from an array of NWORDS uint32_t values.
// The element at index 0 is the least significant, and the element
// at index 7 is the most significant.
UInt256 uint256_create( const uint32_t data[8] ) {
  UInt256 result = {0};
  for (int i = 0; i < 8; i++) {
    result.data[i] = data[i];
  }
  return result;
}

// Create a UInt256 value from a string of hexadecimal digits.
UInt256 uint256_create_from_hex( const char *hex ) {
  UInt256 result = {0};  
  size_t len = strlen(hex);
  if (len == 1 && hex[0] == '0') {
    return result;
  }
  size_t hex_start = (len > 64) ? len - 64 : 0;
  size_t hex_len = len - hex_start;
  for (size_t i = 0; i < hex_len; i++) {
    char c = hex[hex_start + i];
    uint32_t val = 0;
    if (c >= '0' && c <= '9') {
      val = c - '0';
    } else if (c >= 'a' && c <= 'f') {
      val = c - 'a' + 10;
    } else if (c >= 'A' && c <= 'F') {
      val = c - 'A' + 10;
    } else {
      continue;
    }
    size_t byte_index = (hex_len - 1 - i) / 8;
    size_t bit_shift = ((hex_len - 1 - i) % 8) * 4;
    result.data[byte_index] |= (val << bit_shift);
  }
  return result;
}

// Return a dynamically-allocated string of hex digits representing the
// given UInt256 value.
char *uint256_format_as_hex( UInt256 val ) {
  char *hex = malloc(65);
  if (!hex) return NULL;
  int started = 0;
  int index = 0;
  for (int i = 7; i >= 0; i--) {
    for (int j = 7; j >= 0; j--) {
      uint8_t nibble = (val.data[i] >> (j * 4)) & 0xF;
      if (nibble != 0 || started) {
        hex[index++] = "0123456789abcdef"[nibble];
        started = 1;
      }
    }
  }
  if (index == 0) {
    hex[index++] = '0';
  }
  hex[index] = '\0';
  return hex;
}

// Get 32 bits of data from a UInt256 value.
// Index 0 is the least significant 32 bits, index 7 is the most
// significant 32 bits.
uint32_t uint256_get_bits( UInt256 val, unsigned index ) {
  uint32_t bits = val.data[index];
  return bits;
}

// Return 1 if bit at given index is set, 0 otherwise.
int uint256_is_bit_set( UInt256 val, unsigned index ) {
  if (index >= 256) {
    return 0;
  }
  uint32_t segment = index / 32;
  uint32_t bit_pos = index % 32;
  return (val.data[segment] & (1U << bit_pos)) ? 1 : 0;
}

// Compute the sum of two UInt256 values.
UInt256 uint256_add( UInt256 left, UInt256 right ) {
  UInt256 sum = {0};
  uint32_t carry = 0;
  for (int i = 0; i < 8; i++) {
    uint64_t temp = (uint64_t)left.data[i] + right.data[i] + carry;
    sum.data[i] = (uint32_t)temp;
    carry = (temp >> 32) & 1;
  }
  return sum;
}

// Compute the difference of two UInt256 values.
UInt256 uint256_sub( UInt256 left, UInt256 right ) {
  return uint256_add(left, uint256_negate(right));
}

// Return the two's-complement negation of the given UInt256 value.
UInt256 uint256_negate( UInt256 val ) {
  UInt256 result = {0};
  uint32_t carry = 1;
  for (int i = 0; i < 8; i++) {
    uint64_t temp = (uint64_t)(~val.data[i]) + carry;
    result.data[i] = (uint32_t)temp;
    carry = (temp >> 32) & 1;
  }
  return result;
}

// Compute the product of two UInt256 values.
UInt256 uint256_mul( UInt256 left, UInt256 right ) {
  UInt256 product = {0};
  for (int i = 0; i < 256; i++) {
    if (uint256_is_bit_set(right, i)) {
      UInt256 temp = uint256_lshift(left, i);
      product = uint256_add(product, temp);
    }
  }
  return product;
}

UInt256 uint256_lshift( UInt256 val, unsigned shift ) {
  assert(shift < 256);
  UInt256 result = {0};
  unsigned word_shift = shift / 32;
  unsigned bit_shift = shift % 32;
  for (int i = 7; i >= (int)word_shift; i--) {
    result.data[i] = val.data[i - word_shift] << bit_shift;
    if (i - word_shift > 0 && bit_shift > 0) {
      result.data[i] |= val.data[i - word_shift - 1] >> (32 - bit_shift);
    }
  }
  return result;
}
