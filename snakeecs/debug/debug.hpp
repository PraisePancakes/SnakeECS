#define _SNEK_DEBUG_ true
#define _SNEK_BENCHMARK_ true
#define _SNEK_CONFIG true
#define _SNEK_STATIC_TEST_ true
#ifdef _SNEK_DEBUG_
#define LOG(X) std::cout << X << std::endl;
#endif
