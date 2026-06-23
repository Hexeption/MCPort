//
// Created by Keir Davis on 23/06/2026.
//

#include "PerlinNoiseFilter.h"

PerlinNoiseFilter::PerlinNoiseFilter(int_t levels) {
    this->seed = this->random.nextInt();
    this->levels = 0;
    this->fuzz = 16;
    this->levels = levels;
}

std::vector<int_t> PerlinNoiseFilter::read(int_t width, int_t height) {
    Random random = Random();
    std::vector<int_t> tmp(width * height);
    int_t level = this->levels;
    int_t step = width >> level;

    for (int_t y = 0; y < height; y += step) {
        for (int_t x = 0; x < width; x += step) {
            tmp[x + y * width] = (random.nextInt(256) - 128) * this->fuzz;
        }
    }

    for (step = width >> level; step > 1; step /= 2) {
        int_t val = 256 * (step << level);
        int_t ss = step / 2;

        for (int_t y = 0; y < height; y += step) {
            for (int_t x = 0; x < width; x += step) {
                int_t ul = tmp[(x + 0) % width + (y + 0) % height * width];
                int_t ur = tmp[(x + step) % width + (y + 0) % height * width];
                int_t dl = tmp[(x + 0) % width + (y + step) % height * width];
                int_t dr = tmp[(x + step) % width + (y + step) % height * width];
                int_t m = (ul + dl + ur + dr) / 4 + random.nextInt(val * 2) - val;
                tmp[x + ss + (y + ss) * width] = m;
            }
        }

        for (int_t y = 0; y < height; y += step) {
            for (int_t x = 0; x < width; x += step) {
                int_t c = tmp[x + y * width];
                int_t r = tmp[(x + step) % width + y * width];
                int_t d = tmp[x + (y + step) % width * width];
                int_t mu = tmp[((x + ss) & (width - 1)) + ((y + ss - step) & (height - 1)) * width];
                int_t ml = tmp[((x + ss - step) & (width - 1)) + ((y + ss) & (height - 1)) * width];
                int_t m = tmp[(x + ss) % width + (y + ss) % height * width];
                int_t u = (c + r + m + mu) / 4 + random.nextInt(val * 2) - val;
                int_t l = (c + d + m + ml) / 4 + random.nextInt(val * 2) - val;
                tmp[x + ss + y * width] = u;
                tmp[x + (y + ss) * width] = l;
            }
        }
    }

    std::vector<int_t> result(width * height);

    for (int_t y = 0; y < height; ++y) {
        for (int_t x = 0; x < width; ++x) {
            result[x + y * width] = tmp[x % width + y % height * width] / 512 + 128;
        }
    }

    return result;
}
