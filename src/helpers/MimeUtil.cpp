#include "../../include/helpers/MimeUtil.h"

std::string utils::MimeUtil::getMimeType(std::string &filePath) {
    std::string result;
    unsigned long pos = filePath.find_last_of('.', 0);
    if (pos != std::string::npos) {
        std::string extention = filePath.substr(pos, filePath.length()-pos);
        result = mimeTypes[extention.c_str()];
    }
    return result;
}