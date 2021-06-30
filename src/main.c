#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <png.h>

void asciify(char *path, char *chars);
static void _file_error(FILE *file, const char *format, ...);

char *chars = " .,:;-_=+*#%0@";

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "%s", "No input file is provided.\n");
        exit(1);
    }

    for (int i = 1; i < argc; i++) {
        asciify(argv[i], chars);
        if (i < argc - 1) printf("\n\n");
    }

    exit(0);
}

void asciify(char *path, char *chars) {
    FILE *file = fopen(path, "rb");
    if (file == NULL) {
        _file_error(NULL, "Cannot open '%s'.\n", path);
    }

    unsigned char header[8];

    // check PNG signature on the first 8 bytes
    fread(header, 1, 8, file);
    if (png_sig_cmp(header, 0, 8)) {
        _file_error(file, "'%s' must be a valid PNG file.\n", path);
    }

    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png) {
        _file_error(file, "Failed to create read struct for '%s'.\n", path);
    }

    png_infop png_info = png_create_info_struct(png);
    if (!png_info) {
        png_destroy_read_struct(&png, NULL, NULL);
        _file_error(file, "Failed to create info struct for '%s'.\n", path);
    }

    png_init_io(png, file);
    png_set_sig_bytes(png, 8);
    png_read_png(png, png_info, PNG_TRANSFORM_IDENTITY, NULL);

    png_uint_32 width = png_get_image_width(png, png_info);
    png_uint_32 height = png_get_image_height(png, png_info);

    png_bytepp rows = png_get_rows(png, png_info);
    size_t length = strlen(chars);

    printf("%d %d\n", width, height);

    // TODO should have grayscaled it first before doing anything
    for (png_uint_32 i = 0; i < height; i++) {
        png_bytep row = rows[i];

        for (size_t j = 0; j < width * 3; j += 4) {
            // 10/10
            float avg = (row[j] + row[j + 1] + row[j + 2]) / 3;
            size_t index = length - round(avg / 255 * length);

            printf("%c%c%c", chars[index], chars[index], chars[index]);
        }

        printf("\n");
    }

    fclose(file);
    png_destroy_read_struct(&png, &png_info, NULL);
}

static void _file_error(FILE *file, const char *format, ...) {
    if (file != NULL) fclose(file);
    fprintf(stderr, format);

    exit(1);
}
