#ifndef _BUILDIN_H
#define _BUILDIN_H

#include "vm_data.h"

#define BUILDIN_FUNC_LIST(T) \
T(print)\
T(println)\
T(scani)\
T(scanf)\
T(scans)\
T(exit)\
T(len)\
T(push)\
T(clear)\
T(resize)\
T(get_keys)\

#define GET_BUILDIN_FUNC_NAME(NAME) build_in##NAME
#define BUILD_FUNC_SIGN(NAME) NATIVE_FUNCTION_SIGN(GET_BUILDIN_FUNC_NAME(NAME))
#define DECL_BUILDIN_FUNC_SIGN(NAME) BUILD_FUNC_SIGN(NAME);

BUILDIN_FUNC_LIST(DECL_BUILDIN_FUNC_SIGN)
#endif
