#include <iostream>

#include "cppproperties/properties.hpp"
#include "cppproperties/archiver_xml.hpp"

struct color :
    tct::properties::properties
{
    MAKE_PROPERTY(red, int);
    MAKE_PROPERTY(green, int);
    MAKE_PROPERTY(blue, int);

    void set(const std::string& rgb_string)
    {
        if (rgb_string.size() != 6)
            throw std::runtime_error(R"(RGB string is malformed. Needs to be of format "rrggbb" in hex format.)");

        int hex = std::stoi(rgb_string, nullptr, 16);
        this->red   = (hex / 0x10000);
        this->green = (hex / 0x100) % 0x100;
        this->blue  = (hex % 0x100);
    }
};

struct shape :
    tct::properties::properties
{
    MAKE_PROPERTY(x, int);
    MAKE_PROPERTY(y, int);
    MAKE_NESTED_PROPERTY(fg_color, color);
    MAKE_NESTED_PROPERTY(bg_color, color);
};

int main()
{
    tct::properties::archiver_xml ar;

    shape s1;
    s1.x = 13;
    s1.y = 37;
    s1.fg_color.set("a1b2c3");
    s1.fg_color.set("00a4f3");

    // To XML
    const std::string xml_str = s1.save(ar);

    // From XML
    shape s2;
    s2.load(ar, xml_str);

    // Print
    if (xml_str != s2.save(ar)) {
        std::cout << "Error: XML strings do not match.\n";
    } else
        std::cout << s2.save(ar) << std::endl;

    return 0;
}
