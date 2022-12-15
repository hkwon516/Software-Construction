#include <stdbool.h>

struct cpuid { unsigned eax, ebx, ecx, edx; };

struct cpuid cpuid (unsigned int leaf, unsigned int subleaf);
void hardware_rand64_fini (void);
_Bool rdrand_supported (void);
void hardware_rand64_init (void);
unsigned long long hardware_rand64 (void);
