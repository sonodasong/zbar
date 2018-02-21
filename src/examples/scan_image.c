#include <stdio.h>
#include <stdlib.h>
//#include <png.h>
#include <zbar.h>

zbar_image_scanner_t *scanner = NULL;
char barcode_type[100];
char barcode_val[100];
#if 0
/* to complete a runnable example, this abbreviated implementation of
 * get_data() will use libpng to read an image file. refer to libpng
 * documentation for details
 */
static void get_data (const char *name,
                      int *width, int *height,
                      void **raw)
{
    FILE *file = fopen(name, "rb");
    if(!file) exit(2);
    png_structp png =
        png_create_read_struct(PNG_LIBPNG_VER_STRING,
                               NULL, NULL, NULL);
    if(!png) exit(3);
    if(setjmp(png_jmpbuf(png))) exit(4);
    png_infop info = png_create_info_struct(png);
    if(!info) exit(5);
    png_init_io(png, file);
    png_read_info(png, info);
    /* configure for 8bpp grayscale input */
    int color = png_get_color_type(png, info);
    int bits = png_get_bit_depth(png, info);
    if(color & PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(png);
    if(color == PNG_COLOR_TYPE_GRAY && bits < 8)
        png_set_gray_1_2_4_to_8(png);
    if(bits == 16)
        png_set_strip_16(png);
    if(color & PNG_COLOR_MASK_ALPHA)
        png_set_strip_alpha(png);
    if(color & PNG_COLOR_MASK_COLOR)
        png_set_rgb_to_gray_fixed(png, 1, -1, -1);
    /* allocate image */
    *width = png_get_image_width(png, info);
    *height = png_get_image_height(png, info);
    *raw = malloc(*width * *height);
    png_bytep rows[*height];
    int i;
    for(i = 0; i < *height; i++)
        rows[i] = *raw + (*width * i);
    png_read_image(png, rows);
}
#endif

int zbar_main (int width, int height, const uint8_t *buffer)
{
    /* obtain image data */
    //int width = 640, height = 480;
    void *raw = NULL;
    int n;
    zbar_image_t *image;
    zbar_symbol_t *symbol;
    zbar_symbol_type_t typ;
    char *data;
    int bar_num = 0;
    //if(argc < 2) return(1);

    /* create a reader */
    //printf("\r\n BR create!");
    scanner = zbar_image_scanner_create();

    /* configure the reader */
    //printf("\r\n BR configure!");
    zbar_image_scanner_set_config(scanner, 0, ZBAR_CFG_ENABLE, 1);

    //get_data(argv[1], &width, &height, &raw);

    /* wrap image data */
    //printf("\r\n BR image create!");
    image = (zbar_image_t *)zbar_image_create();
    zbar_image_set_format(image, *(int*)"Y800");
    zbar_image_set_size(image, width, height);
    zbar_image_set_data(image, buffer, width * height, zbar_image_free_data);

    /* scan the image for barcodes */
    //printf("\r\n BR scan!");
    n = zbar_scan_image(scanner, image);

    /* extract results */
    //printf("\r\n BR result!");
    symbol = (zbar_symbol_t *)zbar_image_first_symbol(image);
    for(; symbol; symbol = (zbar_symbol_t *)zbar_symbol_next(symbol)) {
        /* do something useful with results */
        typ = zbar_symbol_get_type(symbol);
        data = (char *)zbar_symbol_get_data(symbol);
//        printf("decoded %s symbol \"%s\"\n",
//               zbar_get_symbol_name(typ), data);
        strcpy(barcode_type, zbar_get_symbol_name(typ));
        strcpy(barcode_val, data);
    }

    /* clean up */
    zbar_image_destroy(image);
    zbar_image_scanner_destroy(scanner);

    return(n);
}
