#include "image.hpp"

#include <cstring>

#include "bee/file_writer.hpp"

namespace pixel {

Image::Image(int height, int width, uint8_t fill)
    : _height(height), _width(width)
{
  _pixels = (Color*)calloc(height * width, sizeof(Color));
  if (fill != 0) { std::memset(_pixels, fill, height * width * sizeof(Color)); }
}

Image::~Image() { free(_pixels); }

void Image::save_pnm(const bee::FilePath& filepath) const
{
  must(f, bee::FileWriter::create(filepath));
  must_unit(f->write(F("P6\n$ $\n255\n", _width, _height)));
  must_unit(f->write((const char*)data(), data_size()));
}

Image Image::upscale(int factor) const
{
  Image scaled_img(height() * factor, width() * factor);
  for (int y = 0; y < height(); y++) {
    for (int y0 = 0; y0 < factor; y0++) {
      for (int x = 0; x < width(); x++) {
        auto p = get_pixel(y, x);
        for (int x0 = 0; x0 < factor; x0++) {
          scaled_img.set_pixel(y * factor + y0, x * factor + x0, p);
        }
      }
    }
  }
  return scaled_img;
}

Image Image::downscale(int factor) const
{
  int factor2 = factor * factor;
  Image out(height() / factor, width() / factor);
  for (int y = 0; y < out.height(); y++) {
    int y0 = y * factor;
    for (int x = 0; x < out.width(); x++) {
      int x0 = x * factor;
      uint32_t red = 0;
      uint32_t green = 0;
      uint32_t blue = 0;
      for (int y1 = 0; y1 < factor; y1++) {
        for (int x1 = 0; x1 < factor; x1++) {
          auto p = get_pixel(y0 + y1, x0 + x1);
          red += p.r;
          green += p.g;
          blue += p.b;
        }
      }
      Color c;
      c.r = red / factor2;
      c.g = green / factor2;
      c.b = blue / factor2;
      out.set_pixel(y, x, c);
    }
  }
  return out;
}

} // namespace pixel
