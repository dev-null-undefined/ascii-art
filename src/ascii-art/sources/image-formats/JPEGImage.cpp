#include <cstddef>
#include <cstdio>
#include <jpeglib.h>
#include <csetjmp>
#include "JPEGImage.h"
#include "../../../container/Matrix.h"
#include "../BufferedFrame.h"

extern JSAMPLE * image_buffer;    /* Points to large array of R,G,B-order data */
extern int image_height;    /* Number of rows in image */
extern int image_width;        /* Number of columns in image */

// TODO: link source of this snippet
/******************** JPEG DECOMPRESSION SAMPLE INTERFACE *******************/

/*
 * ERROR HANDLING:
 *
 * The JPEG library's standard error handler (jerror.c) is divided into
 * several "methods" which you can override individually.  This lets you
 * adjust the behavior without duplicating a lot of code, which you might
 * have to update with each future release.
 *
 * Our example here shows how to override the "error_exit" method so that
 * control is returned to the library's caller when a fatal error occurs,
 * rather than calling exit() as the standard error_exit method does.
 *
 * We use C's setjmp/longjmp facility to return control.  This means that the
 * routine which calls the JPEG library must first execute a setjmp() call to
 * establish the return point.  We want the replacement error_exit to do a
 * longjmp().  But we need to make the setjmp buffer accessible to the
 * error_exit routine.  To do this, we make a private extension of the
 * standard JPEG error handler object.  (If we were using C++, we'd say we
 * were making a subclass of the regular error handler.)
 *
 * Here's the extended error handler struct:
 */



struct error_mgr {
    struct jpeg_error_mgr pub;    /* "public" fields */
};


/*
 * Here's the routine that will replace the standard error_exit method:
 */

void my_error_exit(j_common_ptr cinfo) {
    char error_msg[1000];
    (*cinfo->err->format_message)(cinfo, error_msg);
    throw std::invalid_argument(error_msg);
}


/*
 * Sample routine for JPEG decompression.  We assume that the source file name
 * is passed in.  We want to return 1 on success, 0 on error.
 */


void JPEGImage::Load() {
    /* This struct contains the JPEG decompression parameters and pointers to
     * working space (which is allocated as needed by the JPEG library).
     */
    jpeg_decompress_struct cinfo{};
    /* We use our private extension JPEG error handler.
     * Note that this struct must live as long as the main JPEG parameter
     * struct, to avoid dangling-pointer problems.
     */
    error_mgr jerr{};
    /* More stuff */
    FILE * infile;        /* source file */
    JSAMPARRAY buffer;        /* Output row buffer */
    long row_stride;        /* physical row width in output buffer */

    /* In this example we want to open the input file before doing anything else,
     * so that the setjmp() error recovery below can assume the file is open.
     * VERY IMPORTANT: use "b" option to fopen() if you are on a machine that
     * requires it in order to read binary files.
     */

    if ((infile = fopen(m_filename.c_str(), "rb")) == nullptr) {
        throw std::invalid_argument("Can not open file: " + m_filename);
    }

    /* Step 1: allocate and initialize JPEG decompression object */

    /* We set up the normal JPEG error routines, then override error_exit. */
    cinfo.err = jpeg_std_error(&jerr.pub);
    jerr.pub.error_exit = my_error_exit;
    /* Establish the setjmp return context for my_error_exit to use. */

    try {
        /* Now we can initialize the JPEG decompression object. */
        jpeg_create_decompress(&cinfo);

        /* Step 2: specify data source (eg, a file) */

        jpeg_stdio_src(&cinfo, infile);

        /* Step 3: read file parameters with jpeg_read_header() */

        (void) jpeg_read_header(&cinfo, TRUE);
        /* We can ignore the return value from jpeg_read_header since
         *   (a) suspension is not possible with the stdio data source, and
         *   (b) we passed TRUE to reject a tables-only JPEG file as an error.
         * See libjpeg.doc for more info.
         */

        /* Step 4: set parameters for decompression */

        /* In this example, we don't need to change any of the defaults set by
         * jpeg_read_header(), so we do nothing here.
         */

        /* Step 5: Start decompressor */

        (void) jpeg_start_decompress(&cinfo);
        /* We can ignore the return value since suspension is not possible
         * with the stdio data source.
         */

        /* We may need to do some setup of our own at this point before reading
         * the data.  After jpeg_start_decompress() we have the correct scaled
         * output image dimensions available, as well as the output colormap
         * if we asked for color quantization.
         * In this example, we need to make an output work buffer of the right size.
         */
        /* JSAMPLEs per row in output buffer */
        row_stride = cinfo.output_width * cinfo.output_components;
        auto pixels = Matrix<Color>(cinfo.output_height, cinfo.output_width);
        /* Make a one-row-high sample array that will go away when done with image */
        buffer = (*cinfo.mem->alloc_sarray)
                ((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

        /* Step 6: while (scan lines remain to be read) */
        /*           jpeg_read_scanlines(...); */

        /* Here we use the library's state variable cinfo.output_scanline as the
         * loop counter, so that we don't have to keep track ourselves.
         */
        while (cinfo.output_scanline < cinfo.output_height) {
            /* jpeg_read_scanlines expects an array of pointers to scanlines.
             * Here the array is only one element long, but you could ask for
             * more than one scanline at a time if that's more convenient.
             */
            (void) jpeg_read_scanlines(&cinfo, buffer, 1);
            /* Assume put_scanline_someplace wants a pointer and sample count. */
            long row_index = cinfo.output_scanline - 1;
            for (long column_index = 0; column_index < cinfo.output_width; ++column_index) {
                Color pixel;
                for (int component = 0; component < cinfo.output_components; ++component) {
                    JSAMPLE value = buffer[0][column_index * cinfo.output_components + component];
                    switch (component) {
                        case 0:
                            pixel.setRed(value);
                            break;
                        case 1:
                            pixel.setGreen(value);
                            break;
                        case 2:
                            pixel.setBlue(value);
                            break;
                        case 3:
                            pixel.setAlpha(value);
                            break;
                        default:
                            throw std::runtime_error("");
                    }
                }
                pixels.at(column_index, row_index) = pixel;
            }
        }

        m_frame = std::shared_ptr<Frame>(new BufferedFrame(std::move(pixels)));


        /* Step 7: Finish decompression */

        (void) jpeg_finish_decompress(&cinfo);
        /* We can ignore the return value since suspension is not possible
         * with the stdio data source.
         */

        /* Step 8: Release JPEG decompression object */

        /* This is an important step since it will release a good deal of memory. */
        jpeg_destroy_decompress(&cinfo);

        /* After finish_decompress, we can close the input file.
         * Here we postpone it until after no more JPEG errors are possible,
         * so as to simplify the setjmp error logic above.  (Actually, I don't
         * think that jpeg_destroy can do an error exit, but why assume anything...)
         */
        fclose(infile);

        /* At this point you may want to check to see whether any corrupt-data
         * warnings occurred (test whether jerr.pub.num_warnings is nonzero).
         */
    } catch (std::invalid_argument & e) {
        jpeg_destroy_decompress(&cinfo);
        fclose(infile);
        throw e;
    }
}

JPEGImage::JPEGImage(std::string filename) : m_filename(std::move(filename)) {

}

std::string_view JPEGImage::filename() const {
    return std::string_view(m_filename);
}
