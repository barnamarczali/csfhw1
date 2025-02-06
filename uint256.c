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
  UInt256 result;
  // TODO: implement
  return result;
}

// Return a dynamically-allocated string of hex digits representing the
// given UInt256 value.
char *uint256_format_as_hex( UInt256 val ) {
  char *hex = NULL;
  // TODO: implement
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
  assert( shift < 256 );
  UInt256 result;
  // TODO: implement
  return result;
}
