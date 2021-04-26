// This file is special in that it doesn't use the pre compiled headers.
// We use it to implement the stb_image external library.

/// #TODO: Does this actually work or do we need to remove stb_image from the pre compiled headers?

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tinyobj_loader.h"
