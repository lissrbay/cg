#include <cmath>
#include "colorconv.hpp"

Rgbd rgbConvertToDouble(Rgb rgb)
{
    double r_ = rgb.r / 255.0;
    double g_ = rgb.g / 255.0;
    double b_ = rgb.b / 255.0;

    return {r_, g_, b_};
}
int hueCalculation(double delta, double cmax, Rgbd rgb)
{
    int hue = 0;
    if (delta != 0)
    {
        if (cmax - rgb.r < 1e-4)
            hue = 60 * ((rgb.g - rgb.b) / cmax + (rgb.g < rgb.b ? 6 : 0));
        else if (cmax - rgb.g < 1e-4)
            hue = 60 * ((rgb.b - rgb.r) / delta + 2);
        else if (cmax - rgb.b < 1e-4)
            hue = 60 * ((rgb.r - rgb.g) / delta + 4);
    }
    return hue > 0 ? hue : hue + 360;
}
Cmyk rgbToCmyk(Rgb rgb)
{
    Rgbd rgb_ = rgbConvertToDouble(rgb);
    double key = 1 - std::max(rgb_.r, std::max(rgb_.g, rgb_.b));
    double c = (1 - rgb_.r - key) / (1 - key);
    double m = (1 - rgb_.g - key) / (1 - key);
    double y = (1 - rgb_.b - key) / (1 - key);
    return {c, m, y, key};
}
Rgb cmykToRgb(Cmyk cmyk)
{
    int r = 255 * (1 - cmyk.c) * (1 - cmyk.k);
    int g = 255 * (1 - cmyk.m) * (1 - cmyk.k);
    int b = 255 * (1 - cmyk.y) * (1 - cmyk.k);
    return {r, g, b};
}
Rgbd conversationForCX(double h, double c, double x)
{
    Rgbd rgb_;
    if (h >= 0 && h < 60)
        rgb_ = {c, x, 0};
    if (h >= 60 && h < 120)
        rgb_ = {x, c, 0};
    if (h >= 120 && h < 180)
        rgb_ = {0, c, x};
    if (h >= 180 && h < 240)
        rgb_ = {0, x, c};
    if (h >= 240 && h < 300)
        rgb_ = {x, 0, c};
    if (h >= 300 && h < 360)
        rgb_ = {c, 0, x};
    return rgb_;
}
Rgb hsvToRgb(Hsv hsv)
{
    double c = hsv.s * hsv.v;
    double h = hsv.h;
    double x = c * (1.0 - fabs(fmod(hsv.h / 60.0, 2) - 1.0));
    double m = hsv.v - c;
    Rgbd rgb_ = conversationForCX(h, c, x);

    return {(int)round((rgb_.r + m) * 255), (int)round((rgb_.g + m) * 255), (int)round((rgb_.b + m) * 255)};
}
Hsv rgbToHsv(Rgb rgb)
{
    Rgbd rgb_ = rgbConvertToDouble(rgb);
    double cmax = std::max(rgb_.r, std::max(rgb_.g, rgb_.b));
    double cmin = std::min(rgb_.r, std::min(rgb_.g, rgb_.b));
    double delta = cmax - cmin;
    double h = hueCalculation(delta, cmax, rgb_), s = 0, v = cmax;

    if (cmax != 0)
        s = delta / cmax;
    return {h, s, v};
}
Hsv cmykToHsv(Cmyk cmyk)
{
    return rgbToHsv(cmykToRgb(cmyk));
}
Cmyk hsvToCmyk(Hsv hsv)
{
    return rgbToCmyk(hsvToRgb(hsv));
}
Hsl rgbToHsl(Rgb rgb)
{
    Rgbd rgb_ = rgbConvertToDouble(rgb);
    double cmax = std::max(rgb_.r, std::max(rgb_.g, rgb_.b));
    double cmin = std::min(rgb_.r, std::min(rgb_.g, rgb_.b));
    double delta = cmax - cmin;
    double h = hueCalculation(delta, cmax, rgb_), s = 0, l = (cmax + cmin) / 2;

    if (delta != 0)
        s = delta / (1 - abs(2 * l - 1));
    return {h, s, l};
}
Rgb hslToRgb(Hsl hsl)
{
    double c = (1.0 - fabs(2 * hsl.l - 1.0)) * hsl.s;
    double m = 1.0 * (hsl.l - 0.5 * c);
    double x = c * (1.0 - fabs(fmod(hsl.h / 60.0, 2) - 1.0));
    Rgbd rgb_ = conversationForCX(hsl.h, c, x);

    return {(int)round((rgb_.r + m) * 255), (int)round((rgb_.g + m) * 255), (int)round((rgb_.b + m) * 255)};
}
Hsl cmykToHsl(Cmyk cmyk)
{
    return rgbToHsl(cmykToRgb(cmyk));
}
Cmyk hslToCmyk(Hsl hsl)
{
    return rgbToCmyk(hslToRgb(hsl));
}
Hsv hslToHsv(Hsl hsl)
{
    return rgbToHsv(hslToRgb(hsl));
}
Hsl hsvToHsl(Hsv hsv)
{
    return rgbToHsl(hsvToRgb(hsv));
}
Cmy rgbToCmy(Rgb rgb)
{
    Rgbd rgb_ = rgbConvertToDouble(rgb);
    return {1 - rgb_.r, 1 - rgb_.g, 1 - rgb_.b};
}
Rgb cmyToRgb(Cmy cmy)
{
    Rgb rgb = {(int)round((1 - cmy.c) * 255), (int)round((1 - cmy.m) * 255), (int)round((1 - cmy.y) * 255)};
    return rgb;
}
Cmyk cmyToCmyk(Cmy cmy)
{
    return rgbToCmyk(cmyToRgb(cmy));
}
Cmy cmykToCmy(Cmyk cmyk)
{
    return rgbToCmy(cmykToRgb(cmyk));
}
Hsl cmyToHsl(Cmy cmy)
{
    return rgbToHsl(cmyToRgb(cmy));
}
Cmy HslToCmy(Hsl hsl)
{
    return rgbToCmy(hslToRgb(hsl));
}
Hsv cmyToHsv(Cmy cmy)
{
    return rgbToHsv(cmyToRgb(cmy));
}
Cmy hsvToCmy(Hsv hsv)
{
    return rgbToCmy(hsvToRgb(hsv));
}

double colorForRgbToXyzConversion(double c)
{
    if (c > 0.04045)
        c = pow(((c + 0.055) / 1.055), 2.4);
    else
        c = c / 12.92;
    return c;
}

double colorForXyzToRgbConversion(double c)
{
    if (c > 0.0031308)
        c = 1.055 * pow(c, (1.0 / 2.4)) - 0.055;
    else
        c = 12.92 * c;
    return c;
}

Rgb xyzToRgb(Xyz xyz)
{
    double r, g, b;
   
    r = 3.2406 * xyz.x - 1.5372 * xyz.y - 0.4986 * xyz.z;
    g = -0.9689 * xyz.x + 1.8758 * xyz.y + 0.0415 * xyz.z;
    b = 0.0557 * xyz.x - 0.2040 * xyz.y + 1.057 * xyz.z;
    r = colorForXyzToRgbConversion(r);
    g = colorForXyzToRgbConversion(g);
    b = colorForXyzToRgbConversion(b);
    return {(int)round(r * 255), (int)round(g * 255), (int)round(b * 255)};
}
Xyz rgbToXyz(Rgb rgb_)
{
    double x, y, z;
    Rgbd rgb = rgbConvertToDouble(rgb_);
    rgb.r = colorForRgbToXyzConversion(rgb.r);
    rgb.g = colorForRgbToXyzConversion(rgb.g);
    rgb.b = colorForRgbToXyzConversion(rgb.b);
    x = 0.4124 * rgb.r + 0.3576 * rgb.g + 0.1805 * rgb.b;
    y = 0.2126 * rgb.r + 0.7152 * rgb.g + 0.0722 * rgb.b;
    z = 0.0193 * rgb.r + 0.1192 * rgb.g + 0.9505 * rgb.b;
    return {x, y, z};
}

Xyz cmyToXyz(Cmy cmy)
{
    return rgbToXyz(cmyToRgb(cmy));
}

Cmy xyzToCmy(Xyz xyz)
{
    return rgbToCmy(xyzToRgb(xyz));
}

Xyz cmykToXyz(Cmyk cmyk)
{
    return rgbToXyz(cmykToRgb(cmyk));
}

Cmyk xyzToCmyk(Xyz xyz)
{
    return rgbToCmyk(xyzToRgb(xyz));
}

Xyz hslToXyz(Hsl hsl)
{
    return rgbToXyz(hslToRgb(hsl));
}

Hsl xyzToHsl(Xyz xyz)
{
    return rgbToHsl(xyzToRgb(xyz));
}

Xyz hsvToXyz(Hsv hsv)
{
    return rgbToXyz(hsvToRgb(hsv));
}

Hsv xyzToHsv(Xyz xyz)
{
    return rgbToHsv(xyzToRgb(xyz));
}

