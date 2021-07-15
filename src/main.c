#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <getopt.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize.h"

static void print_help(const char *file);
static void debug(const char *format, ...);

bool verbose = false;
const struct option options[] = {
    { "help",    no_argument,       0,  0  },
    { "verbose", no_argument,       0, 'v' },
    { "chars",   required_argument, 0, 'c' },
    { "scale",   required_argument, 0, 's' },
    { 0, 0, 0, 0 }
};

int main(int argc, char *argv[]) {
    char *chars = " _.,-~:oO0@#";
    float scl = 0;

    char *bin = argv[0];
    int opt;

    while ((opt = getopt_long_only(argc, argv, "vc:s:", options, NULL)) != -1) {
        switch (opt) {
            case 0:
                print_help(bin);
                return 0;
                break;
            case 'v':
                verbose = true;
                break;
            case 'c':
                chars = optarg;
                break;
            case 's':
                if ((scl = atof(optarg)) <= 0) {
                    fprintf(stderr, "%s: scale cannot be or less than zero\n", bin);
                    return 1;
                }
                break;
            default:
                return 1;
        }
    }

    if (optind == argc) {
        fprintf(stderr, "%s: input file is not specified\n", bin);
        return 1;
    }

    char *file = argv[optind];
    int w, h, c;

    debug("- Loading specified file '%s'\n", file);

    unsigned char *img = stbi_load(file, &w, &h, &c, 1);
    if (img == NULL) {
        fprintf(stderr, "%s: failed to load '%s': %s\n", bin, file, strerror(errno));
        return 1;
    }

    debug("- Loaded image '%s'\n  Size: %d x %d, Channels: %d\n", file, w, h, c);

    unsigned char *imgs;
    int ws, hs;

    if (scl > 0) {
        debug("- Scaling image size by %f\n", scl);
        ws = w * scl;
        hs = h * scl;

        imgs = malloc(ws * hs * 2);
        stbir_resize_uint8(img, w, h, 0, imgs, ws, hs, 0, 1);
    } else {
        debug("- Scale is not specified, using default image size\n");
        ws = w;
        hs = h;
        imgs = img;
    }

    debug("- Result image size: %d x %d\n", ws, hs);
    debug("- Converting image to ASCII characters\n");

    size_t len = strlen(chars);

    for (int y = 0; y < hs; y++) {
        for (int x = 0; x < ws; x++) {
            int ind = round((float) imgs[x + ws * y] / 255.0f * len);

            printf("%c", chars[ind]);
        }

        printf("\n");
    }

    debug("- Finished converting image to ASCII characters\n");
    debug("- Freeing image memory\n");

    if (imgs != img) stbi_image_free(imgs);
    stbi_image_free(img);
    
    return 0;
}

static void print_help(const char *file) {
    printf(
        "Usage: %s [options ...] file\n"
        "      --help            show this information\n"
        "  -v, --verbose         be verbose\n"
        "  -c, --chars <string>  set output characters from dark to light\n"
        "  -s, --scale <float>   scales up/down the image and text output\n",
        file
    );
}

static void debug(const char *format, ...) {
    if (verbose) printf(format);
}
