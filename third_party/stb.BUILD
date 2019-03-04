# Description:
#   single-file public domain (or MIT licensed) libraries for C/C++

licenses(["notice"])  # MIT license

exports_files(["LICENSE"])

cc_library(
    name = "stb",
    srcs = [
    ],
    hdrs = [
        "stb.h",  # helper functions for C, mostly redundant in C++; basically author's personal stuff
        "stb_c_lexer.h",  # simplify writing parsers for C-like languages
        "stb_connected_components.h",  # incrementally compute reachability on grids
        "stb_divide.h",  # more useful 32-bit modulus e.g. "euclidean divide"
        "stb_ds.h",  # typesafe dynamic array and hash tables for C, will compile in C++
        "stb_dxt.h",  # Fabian "ryg" Giesen's real-time DXT compressor
        "stb_easy_font.h",  # quick-and-dirty easy-to-deploy bitmap font for printing frame rate, etc
        "stb_herringbone_wang_tile.h",  # herringbone Wang tile map generator
        "stb_image.h",  # image loading/decoding from file/memory: JPG, PNG, TGA, BMP, PSD, GIF, HDR, PIC
        "stb_image_resize.h",  #resize images larger/smaller with good quality
        "stb_image_write.h",  # image writing to disk: PNG, TGA, BMP
        "stb_leakcheck.h",  # quick-and-dirty malloc/free leak-checking
        "stb_perlin.h",  # revised Perlin noise (3D input, 1D output)
        "stb_rect_pack.h",  # simple 2D rectangle packer with decent quality
        "stb_sprintf.h",  # fast sprintf, snprintf for C/C++
        "stb_textedit.h",  # guts of a text editor for games etc implementing them from scratch
        "stb_tilemap_editor.h",  # embeddable tilemap editor
        "stb_truetype.h",  # parse, decode, and rasterize characters from truetype fonts
        "stb_vorbis.c",  # decode ogg vorbis files from file/memory to float/16-bit signed output
        "stb_voxel_render.h",  # Minecraft-esque voxel rendering "engine" with many more features
        "stretchy_buffer.h",  # typesafe dynamic array for C (i.e. approximation to vector<>), doesn't compile as C++
    ],
    copts = [
    ],
    linkopts = [
    ],
    visibility = ["//visibility:public"],
)
