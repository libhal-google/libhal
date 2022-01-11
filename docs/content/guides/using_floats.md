---
title: "Using Floats"
weight: 60
---

## Runtime Cost

- https://blog.segger.com/wp-content/uploads/2019/11/Bench_Comparison.pdf
- https://blog.segger.com/floating-point-face-off-part-2-comparing-performance/

## Space Cost

Each was tested with the following operators (+, -, *, /, <, >, ==, <=, >=):

### 32-bit integer conversion only:
- Floats: `1392` bytes
- Doubles: `1968` bytes

### 64-bit integer conversion only:
- Floats: `3088` bytes
- Doubles: `2128` bytes

### Both 32-bit & 64-bit conversion:
- Doubles + Floats: `3072` bytes
- Doubles + Floats: `3592` bytes

## Problems with Floats

- Infinties & NaN
- Lose of precision
- Conversion to integer types
- Floating-point environment
  - https://en.cppreference.com/w/cpp/numeric/fenv
  - Requires TLS which is disabled for stock arm-none-eabi-gcc
