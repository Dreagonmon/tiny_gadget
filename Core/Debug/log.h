#include <stdint.h>

#define log(t) _log((const char *)(t))

void _log(const char *text);
void log_ln();
void log_unum(uint64_t num);
void log_num(int64_t num);
