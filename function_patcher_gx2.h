#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "../utils/function_patcher.h"

extern FunctionHook method_hooks_gx2[];

extern u32 method_hooks_size_gx2;

extern volatile unsigned int method_calls_gx2[];

#ifdef __cplusplus
}
#endif