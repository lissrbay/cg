#include <iostream>
#include "colorconv.hpp"
using namespace std;
int main()
{
    Rgb rgb = {255, 13, 56};
    Cmyk cmyk = {0, 0.84, 0.52, 0.09};
    Hsv hsv  = {213, 0.43, 0.07};
    Hsl t0 = rgbToHsl(rgb);
    Rgb t = hsvToRgb(hsv);
    cout << t.r << " " << t.g << " " << t.b << endl;
    /*Rgb p = xyzToRgb(t);

    cout << p.r << " " << p.g << " " << p.b << endl;*/
    return 0;
}