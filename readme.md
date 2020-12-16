[![Build Status](https://ci.simulton.com/buildStatus/icon?job=CppProperties)](https://ci.simulton.com/job/CppProperties/)

# Introduction
This is a C++17 library providing a property system to client classes.

# Features
The library is built with the following aspects in mind:
- Modern C++
- Easy to use
- Providing "raw access" to the properties just as if they were regular class members.
- Easy integration of optional (de)serialization.
- Easy registration of custom property types.
- Observer interface for property change notifications.

# Notes
A couple of things to be aware of when using this library:
- Requires a C++17 capable compiler
- Properties are stored on the heap
- The memory layout of `struct { MAKE_PROPERTY(a, int) };` is not the same as `struct { int a; };`
- Property change notification observer callbacks are invoked by which ever thread modified the property value.

# License
This library is MIT licenses.

It uses [tinyxml2](https://github.com/leethomason/tinyxml2) for serialization. The tinyxml2 library itself is zlib licensed.

# Support types
Any type can be registered as a property type using the `REGISTER_PROPERTY` macro.
For convenience, a set of built-in types are already registered:
- `bool`
- `int`
- `float`
- `double`
- `std::basic_string<T>` (eg. `std::string`, `std::wstring`, ...)
- `std::filesystem::path`

If the cmake option `ENABLE_QT` is set to `ON`, the following types are also built-in:
- `QString`

# Examples
Start by reading the `Usage` section below. More examples can be found in the [examples](examples) directory.

# Usage
Basic usage only requires inheriting from `cppproperties::properties` and adding properties using `MAKE_PROPERTY()`:
```cpp
struct shape :
    tct::cppproperties::properties
{
    MAKE_PROPERTY(x, float);
    MAKE_PROPERTY(y, float);
};
```
The defined properties may now be used as if they were just class members of type `float`:
```cpp
int main(void)
{
    shape s;
    s.x = 24.48f;
    s.y = -13.29f;
  
    // Print them 
    std::cout << "s.x = " << s.x << std::endl;
    std::cout << "s.y = " << s.x << std::endl;
}
```

## Custom types
Custom types may be used after registering them with `REGISTER_PROPERTY()`:
```cpp
/**
 * Define a custom type 'color'.
 */
struct color
{
    std::string name;
    uint8_t r, g, b;

    [[nodiscard]] std::string to_string() const
    {
        // ...
        return { };
    }

    void from_string(const std::string& str)
    { 
        // ...
    }
};

/**
 * Register the property
 */
REGISTER_PROPERTY(
    color,
    [this](){ return data.to_string(); },
    [this](const std::string& str){ this->data.from_string(str); }
)

/**
 * Client class using properties.
 */
struct shape :
    tct::cppproperties::properties
{
    MAKE_PROPERTY(x, float);
    MAKE_PROPERTY(y, float);
    MAKE_PROPERTY(stroke_color color);
    MAKE_PROPERTY(fill_color color);
};
```

## Notifications
Properties allow registering observers to notify them upon changes of the property value.
```cpp
struct shape :
    tct::cppproperties::properties
{
    MAKE_PROPERTY(x, float);
    MAKE_PROPERTY(y, float);

    shape()
    {
        x.register_observer([](){ std::cout << "x property changed!\n"; });
        y.register_observer([](){ std::cout << "y property changed!\n"; });
    }

};

int main()
{
    shape s;
    s.x = 42;   // Prints "x property changed!";
    s.y = 73;   // Prints "y property changed!";

    return 0;
}
```

## Serialization
The library comes with built-in support for (de)serialization. Classes can be easily (de)serialization to/from XML:
```cpp
struct shape :
    tct::cppproperties::properties
{
    MAKE_PROPERTY(x, float);
    MAKE_PROPERTY(y, float);
}

int main(void)
{
    // Create a shape
    shape s;
    s.x = 13;
    s.y = 37;
 
    // Serialize to std::string using XML format   
    const std::string& xml_string =  s.to_xml();
    std::cout << xml_string << std::endl;
    
    // Serialize to XML file
    s.to_xml_file("/path/to/shape.xml");

    // Deserialize from std::string
    shape s2;
    s2.from_xml(xml_string);

    // Deserialize from XML file
    shape s3;
    s3.from_xml_file("/path/to/shape.xml");

    return 0;
}
```

# Testing
This library provides a [doctest](https://github.com/onqtam/doctest) based test suite under `/test`. The corresponding cmake target is `tests`.
