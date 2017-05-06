//
// Created by sibirsky on 26.04.17.
//

#ifndef TP_SERVER_MIMEUTIL_H
#define TP_SERVER_MIMEUTIL_H

#include <map>

namespace utils {
    class Util {
    private:
        std::map<std::string, std::string> mimeTypes = {{"js", "application/javascript"}, {"html", "text/html"}, {"htm", "text/html"},
                                                        {"css", "text/css"}, {"jpg", "image/jpeg"}, {"jpeg", "image/jpeg"}, {"png", "image/png"},
                                                        {"gif", "image/gif"}, {"swf", "application/x-shockwave-flash"}};
    public:
        std::string getMimeType(std::string &filePath);
        std::string getDateTime();
    };
}


#endif //TP_SERVER_MIMEUTIL_H
