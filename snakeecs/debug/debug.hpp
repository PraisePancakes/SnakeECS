#define _SNEK_PEDANTIC_ false
#define _SNEK_DEBUG_ false
#define _SNEK_BENCHMARK_ false
#define _SNEK_CONFIG true
#define _SNEK_STATIC_TEST_ true
#define _SNEK_WORLD_TEST_ true
#define SNEK_ASSERT(value, msg) \
    static_assert(value, msg)

#ifdef _SNEK_DEBUG_
#define LOG(X) std::cout << X << std::endl;
#define LOG_DESCRIPTION(WHICH, WHERE, WHAT) std::cout << "[MODULE]\n\t" << WHICH << "\n[WHERE]\n\t" << WHERE << "\n[WHAT]\n\t" << WHAT << std::endl;

#endif
