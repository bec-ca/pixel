#pragma once

#include <array>
#include <vector>

#include "bee/file_path.hpp"

namespace pixel {

struct Color {
 public:
  uint8_t r, g, b;

  bool operator==(const Color& other) const = default;
};

struct Image {
 public:
  Image(int height, int width, uint8_t fill = 0);

  Image(const Image& other);
  Image(Image&& other);

  Image& operator=(const Image& other);
  Image& operator=(Image&& other);

  ~Image();

  int height() const { return _height; }
  int width() const { return _width; }

  void set_pixel(int y, int x, uint8_t r, uint8_t g, uint8_t b)
  {
    set_pixel(y, x, {r, g, b});
  }

  void set_pixel(int y, int x, Color p) { _pixels[x + y * _width] = p; }

  Color get_pixel(int y, int x) const { return _pixels[x + y * _width]; }

  void save_pnm(const bee::FilePath& filepath) const;

  const void* data() const { return _pixels; }

  size_t data_size() const { return _height * _width * sizeof(Color); }

  Image upscale(int factor) const;
  Image downscale(int factor) const;

 private:
  int _height;
  int _width;

  Color* _pixels;
};

} // namespace pixel
