About
========
Simple pixel reader, supporting JPEG and PNG formats.

Example
==========
This is how I ended up using `pixelr` in my project to create video chatting in the terminal (https://github.com/billyeh/termchat). I  read pixels from an image, compress them, and match them to colors to form a string I could output to the terminal.

```
var pixelr = require('pixelr')
  , colors = require('./node_modules/blessed/lib/colors');
// Colors.js contains a function to match RGB colors to terminal characters.
// It's from the awesome library Blessed: https://github.com/chjj/blessed

/** pixelr.read(filename, format, callback)
 * Format can be either 'jpeg' or 'png'.
 * This function creates a one-dimensional array of values.
 * PNGs have 4 values per pixel: R, G, B, Alpha
 * JPEGs have 3 values per pixel: R, G, B
 * The callback takes an object with properties 'pixels' (an array), 'width', and 'height'.
 */
pixelr.read("image.jpeg", "jpeg", asciizeImage);

function asciizeImage(image) {
  return asciize(image, 400, 400);
}

function asciize(image, asciiWidth, asciiHeight) {
  var countedColors = countColors(image, asciiWidth, asciiHeight)
    , ascii = ''
    , ansiColorCode
    , previousColor = -2;

  for (var i = 0; i < countedColors.length; i++) {
    ansiColorCode = sortColors(countedColors[i])[0].color;
    if (ansiColorCode !== previousColor) {
      ascii += '\033[38;5;' + ansiColorCode + 'm';
      previousColor = ansiColorCode;
    }
    ascii += '#';
  }

  return ascii + '\033[0m';
}

function sortColors(countedColors) {
  var sortedColors = []
    , color;

  for (color in countedColors) {
    sortedColors.push({'color': color, 'count': countedColors[color]});
  }
  return sortedColors.sort(s);

  function s(a, b) {
    if (a.count < b.count) {
      return 1;
    } else if (a.count > b.count) {
      return -1;
    }
    return 0;
  }
}

function countColors(image, asciiWidth, asciiHeight) {
  var colorCount = []
    , blockWidth  = image.width / asciiWidth
    , blockHeight = image.height / asciiHeight
    , index = 0
    , pixelIndex
    , pixelColor;

  for (var i = 0; i < image.pixels.length; i += 3) {
    pixelIndex = i / 3;
    index = Math.floor(pixelIndex / blockWidth) % asciiWidth + 
            Math.floor(pixelIndex / image.width / blockHeight) * asciiWidth;
    if (!colorCount[index]) {
      colorCount[index] = {};
    }
    pixelColor = colors.match(image.pixels[i], image.pixels[i + 1], image.pixels[i + 2]);
    if (!(pixelColor in colorCount[index])) {
      colorCount[index][pixelColor] = 0;
    }
    // Assign less weight to grayscale values for a more interesting picture
    if (pixelColor >= 232 || pixelColor === 59) {
      colorCount[index][pixelColor] += .5;
    } else {
      colorCount[index][pixelColor]++;
    }
  }

  return colorCount;
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
