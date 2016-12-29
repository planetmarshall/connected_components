# connected_components
Implements a connected components algorithm based on contour tracing

## Requirements

connected_components is a header-only library, and requires only a modern C++ compiler. 

## Tests

To build the unit tests, a [CMake] script is provided. In addition, the unit tests have the following requirements.

 * [Google Test]
 * [ImageMagick]

## Platform Notes

### Windows

To run the tests, a [binary installation](http://www.imagemagick.org/script/binary-releases.php) of ImageMagick will be sufficient. CMake should find the library dependencies automatically if you have used the ImageMagick installer, otherwise use the CMake GUI and set the ImageMagick path variables appropriately.

ImageMagick binary distributions only provide release mode libraries. If you wish to debug the test project in Visual Studio you will need to build debug versions of both Google Test and [ImageMagick], which is beyond the scope of this documentation. See the documentation for those projects for full details.

[ImageMagick]: https://github.com/ImageMagick/ImageMagick
[Google Test]: https://github.com/google/googletest
[CMake]: https://cmake.org/

