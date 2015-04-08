
#include "lodepng/lodepng.h"
#include <assert.h>

struct uvec2
{
  unsigned int x;
  unsigned int y;
  uvec2()
  {
    x = 0;
    y = 0;
  }
};

/// Повернуть изображение на 90 градусов.
/// image Изображение.
/// size Размер изображения.
/// bytes Сколько байт занимает один пиксель.
/// direction Направление. true - по часовой. false - против часовой.
std::vector<unsigned char> Rotate(const std::vector<unsigned char> &image, const uvec2 &size, unsigned int bytes = 4, bool direction = true)
{
  assert(bytes > 0 && bytes <= 4);
  assert(image.size() == size.x * size.y * bytes);
  std::vector<unsigned char> outImage(image.size(), 0);

  for (unsigned int y = 0; y < size.y; ++y)
  {
    unsigned int ny = direction ? (size.y - 1 - y) : y;
    for (unsigned int x = 0; x < size.x; ++x)
    {
      unsigned int cx = direction ? x : (size.x - 1 - x);
      for (unsigned int k = 0; k < bytes; ++k)
      {
        outImage[cx * size.y * bytes + ny * bytes + k] = image[y * size.x * bytes + x * bytes + k];
      }
    }
  }

  return outImage;
}

/// Зеркалирование.
/// image Изображение.
/// size Размер изображения.
/// bytes Сколько байт занимает один пиксель.
/// direction Направление. true - по оси X. false - по оси Y.
void Mirror(std::vector<unsigned char> &image, const uvec2 &size, unsigned int bytes = 4, bool direction = true)
{
  assert(bytes > 0 && bytes <= 4);
  assert(image.size() == size.x * size.y * bytes);
  if(direction)
  {
    for (unsigned int y = 0; y < size.y / 2; ++y)
    {
      // Берем первую строку и меняем с последней.
      for (unsigned int x = 0; x < size.x * bytes; ++x)
      {
        std::swap(image[y * size.x * bytes + x], image[(size.y - 1 - y) * size.x * bytes + x]);
      }
    }
  }
  else
  {
    for (unsigned int x = 0; x < size.x / 2; ++x)
    {
      for (unsigned int y = 0; y < size.y; ++y)
      {
        for (unsigned int k = 0; k < bytes; ++k)
        {
          std::swap(image[y * size.x * bytes + x * bytes + k], image[y * size.x * bytes + (size.x - 1 - x) * bytes + k]);
        }
      }
    }
  }
}

/// Поворот на 180 градусов.
/// image Изображение.
/// size Размер изображения.
/// bytes Сколько байт занимает один пиксель.
void Rotate180(std::vector<unsigned char> &image, const uvec2 &size, unsigned int bytes = 4)
{
  assert(bytes > 0 && bytes <= 4);
  assert(image.size() == size.x * size.y * bytes);

  for (unsigned int y = 0; y < size.y / 2; ++y)
  {
    // Берем первую строку и меняем с последней.
    for (unsigned int x = 0; x < size.x; ++x)
    {
      for (unsigned int k = 0; k < bytes; ++k)
      {
        std::swap(image[y * size.x * bytes + x * bytes + k], image[(size.y - 1 - y) * size.x * bytes + (size.x - 1 - x) * bytes + k]);
      }
    }
  }
}

int main()
{
  std::vector<unsigned char> in;
  std::vector<unsigned char> out;
  uvec2 size;

  lodepng::decode(in, size.x, size.y, "in.png");

  out = Rotate(in, size);
  std::swap(size.x, size.y); // После поворота размеры изменились.

  lodepng::encode("out.png", out, size.x, size.y);
  Rotate180(out, size);

  lodepng::encode("outMirror.png", out, size.x, size.y);

  return 0;
}