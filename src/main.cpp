#include "core/world.h"

int main() {
    clock_t start = clock();
    World w;
    cout << "Building scene... \n";
    w.setup();
    cout << "Rendering... \nComplete: \n";
    w.render();
    cout << "\nOutputing image... \n";
    w.output("output.png");
    float duration = float(clock() - start) / CLOCKS_PER_SEC;
    cout << "Done. Time elapsed: " << setprecision(4) << duration << " seconds. \n";
    return 0;
}
