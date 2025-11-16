#ifndef BASE_H
#define BASE_H

#include <cstdint>
#include <string>
#include <iostream>

#define KB(x)   ((unsigned long long)(x) * 1024ULL)
#define MB(x)   (KB(x) * 1024ULL)
#define GB(x)   (MB(x) * 1024ULL)

#define ARRAY_COUNT(x) (sizeof(x) / sizeof(*(x)))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define MAX(x, y) (((x) > (y)) ? (x) : (y))

using  i8 = int8_t;
using  u8 = uint8_t;
using i16 = int16_t;
using u16 = uint16_t;
using i32 = int32_t;
using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;
using f32 = float;
using f64 = double;

namespace Base {

    struct RGBA {
        u8 r, g, b, a;
    };

    using Color = u32;

} // namespace Base

#define global   static 
#define persist  static 
#define internal static 

#endif // BASE_H