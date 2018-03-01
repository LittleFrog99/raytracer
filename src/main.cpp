#include <iostream>
#include "core/world.h"

int main() {
    World w;
    w.build();
    w.renderScene();
    w.output("output.png");
    return 0;
}
