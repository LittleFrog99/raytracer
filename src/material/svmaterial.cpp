#include "svmaterial.h"

SVMatte::SVMatte(Texture *tex, float amb_int, float diff_int) {
    ambBRDF = new SVLambertian(amb_int, tex);
    diffBRDF = new SVLambertian(diff_int, tex);
}

SVPhong::SVPhong(Texture *tex, float amb_int, float diff_int, float spec_int) {
    ambBRDF = new SVLambertian(amb_int, tex);
    diffBRDF = new SVLambertian(diff_int, tex);
    specBRDF = new SVSpecular(spec_int, tex);
}
