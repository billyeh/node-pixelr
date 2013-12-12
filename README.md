About
========
Simple pixel reader, supporting JPEG and PNG formats.

Example
==========
```
var pixelr = require('pixelr');

/** pixelr.read(filename, format, callback)
 * Format can be either 'jpeg' or 'png'.
 * This function creates a one-dimensional array of values.
 * PNGs have 4 values per pixel: R, G, B, Alpha
 * JPEGs have 3 values per pixel: R, G, B.
 * The callback takes an object with properties 'pixels' (an array), 'width', and 'height'.
 */
pixelr.read("image.jpeg", "jpeg", doSomething);

doSomething(data) {
  console.log(data.pixels);
  console.log(data.width);
  console.log(data.height);
}
```

Documentation
===============
JPEG parsing uses `libjpeg`, and PNG parsing uses [`pngjs`](https://npmjs.org/package/pngjs).

The read() function assumes that colors fit in 8-bit integers and have 4 channels for PNGs, and 3 for JPEGs.

Installation
===============
```
$ npm install pixelr
```

License
=========

(The MIT License)

Copyright (c) 2013 Bill Yeh

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
