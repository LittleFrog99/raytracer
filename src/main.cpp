#include "core/world.h"
#include "photon/photonmap.h"

int main() {
    void test();
    test();
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

void test() {
    auto photons = new PhotonMap();
    for (int i = 0; i < 10000; i++)
        photons->addPhoton(dvec3(rnd.randomDouble(), rnd.randomDouble(), rnd.randomDouble()), 
        dvec3(1), vec3(1.0));
    photons->build();
    photons->output();
    delete photons;
    exit(0);
}
