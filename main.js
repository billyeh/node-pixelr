#!/usr/bin/env node

var jpeg = require('./build/Release/pixelr')
  , png  = require('pngjs').PNG
  , fs   = require('fs');

var image
  , width
  , height;

function read(filename, type, cb) {
  image = {'pixels': [], 'width': 0, 'height': 0};
  if (type === 'png') {
    readPng(filename, cb);
  }
  else if (type === 'jpeg') {
    jpeg.read(filename, function(err, image) {
      if (err) cb(err);        
      cb(null, image);
    });    
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
      image['width']  = this.width;
      image['height'] = this.height;
      for (var y = 0; y < this.height; y++) {
        for (var x = 0; x < this.width; x++) {
          var idx = (this.width * y + x) << 2;
          image['pixels'][idx]   = this.data[idx];
          image['pixels'][idx+1] = this.data[idx+1];
          image['pixels'][idx+2] = this.data[idx+2];
          image['pixels'][idx+3] = this.data[idx+3];
        }
      }
      cb(null, image);
    });
}

exports.read = read;