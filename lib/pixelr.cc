#include <stdio.h>
#include <jpeglib.h>
#include <stdlib.h>
#include <unistd.h>
#include <cstring>

unsigned int size;
unsigned int *raw_image = NULL;
unsigned int width;
unsigned int height;

int read_jpeg_file(char *filename)
{
  struct jpeg_decompress_struct cinfo;
  struct jpeg_error_mgr jerr;
  JSAMPROW row_pointer[1];
  FILE *infile = fopen(filename, "rb");
  unsigned long location = 0;
  int i = 0;
  if (!infile) {
      printf("Error opening jpeg file %s\n!", filename);
      return -1;
  }
  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_decompress(&cinfo);
  jpeg_stdio_src(&cinfo, infile);
  jpeg_read_header(&cinfo, TRUE);
  jpeg_start_decompress(&cinfo);

  width = cinfo.output_width;
  height = cinfo.output_height;
  size = cinfo.output_width*cinfo.output_height*cinfo.num_components*sizeof(unsigned int);
  raw_image = (unsigned int *)malloc(size);
  row_pointer[0] = (unsigned char *)malloc(cinfo.output_width*cinfo.num_components);
  while (cinfo.output_scanline < cinfo.image_height) {
      jpeg_read_scanlines( &cinfo, row_pointer, 1 );
      for (i=0; i<cinfo.image_width*cinfo.num_components;i++) {
          raw_image[location++] = (unsigned int) row_pointer[0][i];
      }
  }
  jpeg_finish_decompress(&cinfo);
  jpeg_destroy_decompress(&cinfo);
  free(row_pointer[0]);
  fclose(infile);
  return 1;
}

#define BUILDING_NODE_EXTENSION
#include <node.h>

using namespace v8;

Handle<Value> CreateObject(const Arguments& args) {
  HandleScope scope;

  Local<Object> obj = Object::New();
  Local<Array> pic = Array::New(size / sizeof(unsigned int));
  for (unsigned int i = 0; i < size / sizeof(unsigned int); i++) {
    pic->Set(i, Number::New(raw_image[i]));
  }
  obj->Set(String::NewSymbol("pixels"), pic);
  obj->Set(String::NewSymbol("width"), Number::New(width));
  obj->Set(String::NewSymbol("height"), Number::New(height));

  return obj;
}

Handle<Value> Read(const Arguments& args) {
  HandleScope scope;

  if (args.Length() < 1) {
    ThrowException(Exception::TypeError(String::New("Specify an image filename to read")));
    return scope.Close(Undefined());
  }

  String::AsciiValue string(args[0]);
  char *filename = (char *) malloc(string.length() + 1);
  strcpy(filename, *string);
  read_jpeg_file(filename);

  if (!raw_image) {
    ThrowException(Exception::TypeError(String::New("Error reading image file")));
    return scope.Close(Undefined());
  }

  return scope.Close(CreateObject(args));
}

void init(Handle<Object> exports) {
  exports->Set(String::NewSymbol("read"),
      FunctionTemplate::New(Read)->GetFunction());
}

NODE_MODULE(pixelr, init)
