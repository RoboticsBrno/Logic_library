#if defined(LOGIC_VERSION_1_1)
#include "platform/1_1.hpp"
#elif defined(LOGIC_VERSION_1_2)
#include "platform/1_2.hpp"
#elif defined(LOGIC_VERSION_2_0)
#include "platform/2_0.hpp"
#else
#pragma error "Unknown Logic version"
#endif