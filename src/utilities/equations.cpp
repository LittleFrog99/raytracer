#include "utilities.h"

#define EQN_EPS 1e-90

inline bool isZero(double num) {
    return num < EQN_EPS && num > -EQN_EPS;
}

int Math::solveQuadric(double *c, double *s) {
    double p = c[ 1 ] / (2 * c[ 2 ]);
    double q = c[ 0 ] / c[ 2 ];
    double D = p * p - q;

    if (isZero(D)) {
	    s[ 0 ] = - p;
	    return 1;
    }
    else if (D > 0) {
        double sqrt_D = sqrt(D);
        s[ 0 ] =   sqrt_D - p;
        s[ 1 ] = - sqrt_D - p;
        return 2;
    }
    else return 0;
}

int Math::solveCubic(double *c, double *s) {
    int num;

    /* normal form: x^3 + Ax^2 + Bx + C = 0 */
    double A = c[2] / c[3], B = c[1] / c[3], C = c[0] / c[3];

    /*  substitute x = y - A/3 to eliminate quadric term:
	x^3 +px + q = 0 */
    double sq_A = A * A;
    double p = 1.0 / 3 * (-1.0 / 3 * sq_A + B);
    double q = 1.0 / 2 * (2.0 / 27 * A * sq_A - 1.0 / 3 * A * B + C);

    /* use Cardano's formula */
    double cb_p = p * p * p;
    double D = q * q + cb_p;

    if (isZero(D)) {
        if (isZero(q)) { /* one triple solution */
            s[0] = 0;
            num = 1;
        }
        else { /* one single and one double solution */
            double u = cbrt(-q);
            s[0] = 2 * u;
            s[1] = -u;
            num = 2;
        }
    }
    else if (D < 0) { /* Casus irreducibilis: three real solutions */
        double phi = 1.0 / 3 * acos(-q / sqrt(-cb_p));
        double t = 2 * sqrt(-p);

        s[0] = t * cos(phi);
        s[1] = -t * cos(phi + M_PI / 3);
        s[2] = -t * cos(phi - M_PI / 3);
        num = 3;
    }
    else { /* one real solution */
        double sqrt_D = sqrt(D);
        double u = cbrt(sqrt_D - q);
        double v = -cbrt(sqrt_D + q);

        s[0] = u + v;
        num = 1;
    }

    /* resubstitute */
    double sub = 1.0 / 3 * A;
    for (int i = 0; i < num; ++i)
        s[i] -= sub;

    return num;
}

int Math::solveQuartic(double *c, double *s) {
    double coeffs[4];
    int num;

    /* normal form: x^4 + Ax^3 + Bx^2 + Cx + D = 0 */
    double A = c[3] / c[4];
    double B = c[2] / c[4];
    double C = c[1] / c[4];
    double D = c[0] / c[4];

    /*  substitute x = y - A/4 to eliminate cubic term: x^4 + px^2 + qx + r = 0 */
    double sq_A = A * A;
    double p = -3.0 / 8 * sq_A + B;
    double q = 1.0 / 8 * sq_A * A - 1.0 / 2 * A * B + C;
    double r = -3.0 / 256 * sq_A * sq_A + 1.0 / 16 * sq_A * B - 1.0 / 4 * A * C + D;

    if (isZero(r)) {
        /* no absolute term: y(y^3 + py + q) = 0 */
        coeffs[0] = q;
        coeffs[1] = p;
        coeffs[2] = 0;
        coeffs[3] = 1;

        num = solveCubic(coeffs, s);
        s[num++] = 0;

    } else {
        /* solve the resolvent cubic ... */
        coeffs[0] = 1.0 / 2 * r * p - 1.0 / 8 * q * q;
        coeffs[1] = -r;
        coeffs[2] = -1.0 / 2 * p;
        coeffs[3] = 1;
        
        solveCubic(coeffs, s);
        /* ... and take the one real solution ... */
        double z = s[0];
        /* ... to build two quadric equations */
        double u = z * z - r;
        double v = 2 * z - p;

        if (isZero(u)) 
            u = 0;
        else if (u > 0) 
            u = sqrt(u);
        else return 0;

        if (isZero(v))
            v = 0;
        else if (v > 0)
            v = sqrt(v);
        else return 0;

        coeffs[0] = z - u;
        coeffs[1] = q < 0 ? -v : v;
        coeffs[2] = 1;
        num = solveQuadric(coeffs, s);

        coeffs[0] = z + u;
        coeffs[1] = q < 0 ? v : -v;
        coeffs[2] = 1;
        num += solveQuadric(coeffs, s + num);
    }

    /* resubstitute */
    double sub = 1.0 / 4 * A;
    for (int i = 0; i < num; ++i)
        s[i] -= sub;

    return num;
}