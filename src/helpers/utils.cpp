#include <iostream>
#include "../../include/helpers/utils.h"

std::string utils::getMimeType(const std::string &filePath) {
    magic_t magic;
    magic = magic_open(MAGIC_MIME_TYPE | MAGIC_COMPRESS);
    magic_load(magic, "/usr/share/file/magic.mgc");
    std::string mimeType(magic_file(magic, filePath.c_str()));
    magic_close(magic);
    return mimeType;
}