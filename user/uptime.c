#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[]) {
    uint ticks = uptime();
    printf(1, "System uptime: %d ticks\n", ticks);
    exit();
}
