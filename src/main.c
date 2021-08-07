#include <errno.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <getopt.h>

#include "stb_image.h"
#include "stb_image_resize.h"

static void print_help(const char *file);

const struct option options[] = {
    { "help",     no_argument,       0, 'h' },
    { "chars",    required_argument, 0, 'c' },
    { "scale",    required_argument, 0, 's' },
    { "colorize", no_argument,       0, 'C' },
    { 0, 0, 0, 0 }
};

int main(int argc, char *argv[]) {
    char *chars = " _.,-~:oO0@#";
    float scl = 0;
    bool colorize = false;

    char *bin = argv[0];
    int opt;

    while ((opt = getopt_long_only(argc, argv, "hc:s:C", options, NULL)) != -1) {
        switch (opt) {
            case 'h':
                print_help(bin);
                return 0;
                break;
            case 'c':
                chars = optarg;
                break;
            case 's':
                if ((scl = atof(optarg)) <= 0) {
                    fprintf(stderr, "%s: scale cannot be less than or equal to 0\n", bin);
                    return 1;
                }
                break;
            case 'C':
                colorize = true;
                break;
            default:
                return 1;
        }
    }

    if (optind == argc) {
        fprintf(stderr, "%s: input file is not specified\n", bin);
        return 1;
    }

    // Desired channels: 3 (RGB) if colorize is enabled, 1 (gray) otherwise
    int dc = colorize ? 3 : 1;
    char *file = argv[optind];
    int w, h, c;

    // Load specified file
    unsigned char *img = stbi_load(file, &w, &h, &c, dc);
    if (img == NULL) {
        fprintf(stderr, "%s: failed to load '%s': %s\n", bin, file, strerror(errno));
        return 1;
    }

    // A copy of the initial image that will be modified
    unsigned char *imgs;
    int ws, hs;

    // Scale image by the specified amount or use the initial image size if
    // not specified
    if (scl > 0) {
        ws = w * scl;
        hs = h * scl;

        imgs = malloc(ws * hs * dc);
        stbir_resize_uint8(img, w, h, 0, imgs, ws, hs, 0, dc);
    } else {
        ws = w;
        hs = h;
        imgs = img;
    }

    size_t len = strlen(chars);
    size_t imgs_size = ws * hs * dc;

    // Convert image to the specified characters
    for (int i = 0, j = 0; i < imgs_size; i += dc, j++) {
        if (colorize) {
            float avg = (imgs[i] + imgs[i + 1] + imgs[i + 2]) / 3.0f;
            int n = round(avg / 255.0f * (len - 1));

            printf("\033[38;2;%d;%d;%dm%c", imgs[i], imgs[i + 1], imgs[i + 2], chars[n]);
        } else {
            int n = round((float) imgs[i] / 255.0f * (len - 1));

            printf("%c", chars[n]);
        }

        if (j % ws == ws - 1) printf("\n");
    }

    // Free image memory
    if (imgs != img) stbi_image_free(imgs);
    stbi_image_free(img);
    
    return 0;
}

static void print_help(const char *file) {
    printf(
        "Usage: %s [options ...] file\n"
        "  -h, --help            show this information\n"
        "  -c, --chars <string>  set output characters from dark to light\n"
        "  -s, --scale <float>   set scaling of the image and text output\n"
        "  -C, --colorize        colorize text output, uses true color (24-bit color)\n",
        file
    );
}
