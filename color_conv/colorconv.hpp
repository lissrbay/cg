#pragma once 
#include <iostream>

struct Rgb
{
    int r;
    int g;
    int b;
};
struct Rgbd
{
    double r;
    double g;
    double b;
};
struct Cmyk
{
    double c;
    double m;
    double y;
    double k;
};
struct Hsv
{
    double h;
    double s;
    double v;
};
struct Hsl
{
    double h;
    double s;
    double l;
};
struct Cmy
{
    double c;
    double m;
    double y;
};
struct Xyz
{
    double x;
    double y;
    double z;
};

Rgbd rgbConvertToDouble(Rgb rgb);

int hueCalculation(double delta, double cmax, Rgbd rgb);

Cmyk rgbToCmyk(Rgb rgb);

Rgb cmykToRgb(Cmyk cmyk);

Rgbd conversationForCX(double h, double c, double x);

Rgb hsvToRgb(Hsv hsv);

Hsv rgbToHsv(Rgb rgb);

Hsv cmykToHsv(Cmyk cmyk);

Cmyk hsvToCmyk(Hsv hsv);

Hsl rgbToHsl(Rgb rgb);

Rgb hslToRgb(Hsl hsl);

Hsl cmykToHsl(Cmyk cmyk);

Cmyk hslToCmyk(Hsl hsl);

Hsv hslToHsv(Hsl hsl);

Hsl hsvToHsl(Hsv hsv);

Cmy rgbToCmy(Rgb rgb);

Rgb cmyToRgb(Cmy cmy);

Cmyk cmyToCmyk(Cmy cmy);

Cmy cmykToCmy(Cmyk cmyk);

Hsl cmyToHsl(Cmy cmy);

Cmy HslToCmy(Hsl hsl);

Hsv cmyToHsv(Cmy cmy);

Cmy hsvToCmy(Hsv hsv);

double colorForRgbToXyzConversion(double c);

double colorForXyzToRgbConversion(double c);

Rgb xyzToRgb(Xyz xyz);

Xyz rgbToXyz(Rgb rgb_);

Xyz cmyToXyz(Cmy cmy);

Cmy xyzToCmy(Xyz xyz);

Xyz cmykToXyz(Cmyk cmyk);

Cmyk xyzToCmyk(Xyz xyz);

Xyz hslToXyz(Hsl hsl);

Hsl xyzToHsl(Xyz xyz);

Xyz hsvToXyz(Hsv hsv);

Hsv xyzToHsv(Xyz xyz);

