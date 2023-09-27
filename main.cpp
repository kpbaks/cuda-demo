#include <algorithm>
#include <cstdarg>
#include <cstdio>
#include <iostream>
#include <string>
#include <unistd.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "common.hpp"

auto main(int argc, char **argv) -> int {
  if (argc != 2) {
    std::fprintf(stderr, "Usage: %s <file.png>\n", argv[0]);
    return EXIT_FAILURE;
  }

  auto const filename = std::string(argv[1]);

  int width, height, channels;
  auto const data = stbi_load(filename.c_str(), &width, &height, &channels, 0);
  if (data == nullptr) {
    std::fprintf(stderr, "Error: could not load image %s\n", filename.c_str());
    return EXIT_FAILURE;
  }

  auto input_image = Image{
      .width = width, .height = height, .channels = channels, .data = data};

  std::printf("%s\n", input_image.info().c_str());

  auto output_image =
      Image{.width = input_image.width,
            .height = input_image.height,
            .channels = input_image.channels,
            .data = new unsigned char[input_image.width * input_image.height *
                                      input_image.channels]};

  // Image processing code goes here ...
  // Iterate over a 3x3 patch of pixels. Find the average color. and set the 9
  // pixels to that color.

  // Apply the kernel to the image
  // Save the image to a new file
  // Free the image data
  //
  // Iterate over the pixels and find the average color
  for (int i = 0; i < width; ++i) {
    for (int j = 0; j < height; ++j) {
      channel c = input_image(i, j);
      unsigned char significant_channel = std::max(c.r, std::max(c.g, c.b));
      output_image.set(i, j,
                       {.r = significant_channel,
                        .g = significant_channel,
                        .b = significant_channel,
                        .a = c.a});
    }
  }

  stbi_image_free(data);

  auto const out_filename = filename + ".out.png";
  auto const result = stbi_write_png(out_filename.c_str(), output_image.width,
                                     output_image.height, output_image.channels,
                                     output_image.data, output_image.width * 4);

  delete[] output_image.data;

  return EXIT_SUCCESS;
}