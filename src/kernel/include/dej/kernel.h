#pragma once
// Most stuff that is commonly included
// also some macros and stuff

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <limine.h>
#include <stdatomic.h>

#define likely(x) __builtin_expect(!!(x), true)
#define unlikely(x) __builtin_expect(!!(x), false)
#define always_inline __attribute__((always_inline))

#define KiB(x) (x) * 1024
#define KB(x) (x) * 1000
#define MB(x) KB(x) * 1000
