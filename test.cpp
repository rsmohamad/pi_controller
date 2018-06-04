#include "nanocoater.h"

int main() {
    gpioInitialise();
    NanoCoater nano;
    nano.setDefaultState();

    while (1) {
        //sleep(1);
    }

    return 0;
}
