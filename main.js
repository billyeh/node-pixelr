#!/usr/bin/env node

var jpeg = require('./build/Release/pixelr')
  , png  = require('pngjs').PNG
  , fs   = require('fs');

var pixels = []
  , width
  , height;

function read(filename, type, callback) {
  if (type === 'png') {
    readPng(filename, callback);
  }
  else if (type === 'jpeg') {
    pixels = jpeg.read(filename);
    callback({pixels: pixels.pixels, width: pixels.width, height: pixels.height});
  }
  else {
    console.log('Unidentifiable image format: ' + type);
  }
}

function readPng(filename, cb) {
  fs.createReadStream(filename)
    .pipe(new png({
        filterType: 4
    }))
    .on('parsed', function() {
      width  = this.width;
      height = this.height;
      for (var y = 0; y < this.height; y++) {
        for (var x = 0; x < this.width; x++) {
          var idx = (this.width * y + x) << 2;
          pixels[idx]   = this.data[idx];
          pixels[idx+1] = this.data[idx+1];
          pixels[idx+2] = this.data[idx+2];
          pixels[idx+3] = this.data[idx+3];
        }
      }
      cb({pixels: pixels, width: width, height: height});
    });
}

exports.read = read;
