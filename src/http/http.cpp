#include <cstdlib>
#include <cstring>
#include <sstream>

#include "../../include/http/http.h"

std::map<std::string, std::string> Http::mimeTypes = {{"js", "application/javascript"},
                                                      {"html", "text/html"}, {"htm", "text/html"},
                                                      {"css", "text/css"}, {"jpg", "image/jpeg"},
                                                      {"jpeg", "image/jpeg"}, {"png", "image/png"},
                                                      {"gif", "image/gif"},
                                                      {"swf", "application/x-shockwave-flash"},
                                                      {"txt", "text/plain"}};

void Http::parseRequest(const std::string &request, std::string &parsedMethod, std::string &parsedPath) {
    unsigned int i = 0;
    while ((request[i] != ' ') && (i < request.length()-1) && (request[i] != '\r') && (request[i] != '\n')) {
        parsedMethod += request[i];
        i++;
    }
    if ((request[i] == '\0')) {
        return;
    }

    i++;
    int copyStart = i;
    char hex[3];
    int code;
    char currentChar;
    //TODO: improve
    while ((request[i] != ' ') && (i < request.length()-1) && (request[i] != '\r') &&
            (request[i] != '\n') && (request[i] != '?')) {
        currentChar = request[i];
        if (currentChar == '%') {
            hex[0] = request[i+1];
            hex[1] = request[i+2];
            hex[3] = '\0';
            sscanf(hex, "%X", &code);
            parsedPath += (char)code;
            i += 2;
        } else if (currentChar == '+') {
            parsedPath += ' ';
        } else {
            parsedPath += currentChar;
        }
        i++;
    }
    //int copyEnd = i;
    //parsedPath = request.substr(copyStart, copyEnd - copyStart);
}

std::string Http::makeResponseHead(const std::string &status, const std::string &date,
                                   const std::string &contentType, ssize_t contentLength,
                             const std::string &connection) {
    return (status + "Date: " + date + "\r\nServer: " + SERVER_NAME + "\r\nContent-Type: " + contentType + "\r\nContent-Length: " +
            std::to_string(contentLength) + "\r\nConnection: " + connection + "\r\n\r\n");
}

std::string Http::makeResponse(const std::string &status, const std::string &date,
                               const std::string &contentType, ssize_t contentLength,
                          const std::string &connection, const std::string &body) {
    return (status + "Date: " + date + "\r\nServer: " + SERVER_NAME + "\r\nContent-Type: " + contentType + "\r\nContent-Length: " +
            std::to_string(contentLength) + "\r\nConnection: " + connection + "\r\n\r\n" + body);
}

std::string Http::getMimeType(std::string &filePath) {
    std::string result;
    unsigned long pos = filePath.find_last_of('.', filePath.length());
    if (pos != std::string::npos) {
        std::string extension = filePath.substr(pos+1, filePath.length()-pos);
        result = mimeTypes[extension];
        if (result.length() == 0) {
            result = "application/octet-stream";
        }
    }
    return result;
}

std::string Http::getDateTime() {
    time_t currentTime = time(NULL);
    struct tm t = *localtime(&currentTime);
    char* buff = (char*)malloc(sizeof(char)*32);
    std::string wday;
    switch (t.tm_wday) {
        case 1:
            wday = "Mon";
            break;
        case 2:
            wday = "Tue";
            break;;
        case 3:
            wday = "Wed";
            break;
        case 4:
            wday = "Thu";
            break;
        case 5:
            wday = "Fri";
            break;
        case 6:
            wday = "Sat";
            break;
        case 7:
            wday = "Sun";
            break;
    }
    std::string month;
    switch (t.tm_mon) {
        case 0:
            month = "Jan";
            break;
        case 1:
            month = "Feb";
            break;
        case 2:
            month = "Mar";
            break;
        case 3:
            month = "Apr";
            break;
        case 4:
            month = "May";
            break;
        case 5:
            return "Jun";
        case 6:
            return "Jul";
        case 7:
            return "Aug";
        case 8:
            return "Sep";
        case 9:
            return "Oct";
        case 10:
            return "Nov";
        case 11:
            return "Dec";
    }
    sprintf(buff, "%s, %d %s %d %d:%d:%d %s", wday.c_str(), t.tm_mday, month.c_str(), t.tm_year+1990, t.tm_hour, t.tm_min,
            t.tm_sec, t.tm_zone);
    std::string result = buff;
    delete buff;
    return result;
}

std::string Http::urlDecode(std::string &encodedUrl) {
    std::ostringstream resultStream;
    for (int i = 0; i < encodedUrl.length(); i++) {
        if (encodedUrl[i] == '%') {
            std::string hexChar(encodedUrl.substr(i+1, 2));

            unsigned short chr = 0;
            std::istringstream decodeStream(hexChar);
            decodeStream >> std::hex >> chr;
            resultStream << static_cast<unsigned char>(chr);

            i += 2;
        } else {
            resultStream << encodedUrl[i];
        }
    }
    return resultStream.str();
}