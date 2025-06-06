rgbto256
========

Not all terminals support 24-bit true color—others support only 256 colors, so
a conversion must take place. However, when converting from true color to 256,
[most programs perform a simple Euclidean distance calculation][1] in display
RGB space, which is not [perceptually uniform][2] and thus produces slightly
inaccurate results.

rgbto256 accepts true color input and more accurately converts it to 256 colors
using the [CIEDE2000][3] algorithm.

[1]: https://github.com/termstandard/colors/blob/master/README.md#note-about-color-differences
[2]: https://en.wikipedia.org/wiki/Color_difference#Uniform_color_spaces
[3]: https://en.wikipedia.org/wiki/Color_difference#CIEDE2000

Example
-------

```bash
cat misc/example
```

In a terminal that supports true color, this produces the following:

![original image](misc/original.png)

In a terminal that converts to 256 colors (using a simple Euclidean distance
calculation), we get this instead:

![usual 256 color conversion](misc/256-1.png)

However, when piped through `rgbto256`, the colors are closer to the original
image:

```bash
cat misc/example | ./utils/rgbto256
```

![result when run through rgbto256](misc/256-2.png)

Building
--------

Make and GCC must be installed. Then simply run `make`.

Usage
-----

`./utils/rgbto256` reads data from standard input and echoes it to standard
output, except with true color ANSI escape sequences converted to 256 colors.

For example:

```bash
cat misc/example | ./utils/rgbto256
```

License
-------

rgbto256 is licensed under version 3 of the GNU Affero General Public License,
or (at your option) any later version. See [LICENSE](LICENSE).

Contributing
------------

By contributing to rgbto256, you agree that your contribution may be used
according to the terms of rgbto256’s license.
