#include "core/world.h"



int main() {
    clock_t start = clock();
    World w;
    cout << "Building scenes... \n";
    w.build();
    cout << "Rendering... \nComplete: \n";
    w.renderScene();
    cout << "\nOutputing scenes... \n";
    w.output("output.png");
    float duration = float(clock() - start) / CLOCKS_PER_SEC;
    cout << "Done. Time elapsed: " << setprecision(4) << duration << " seconds. \n";
    return 0;
}
