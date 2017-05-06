#include "../../include/helpers/Util.h"

std::string utils::Util::getMimeType(std::string &filePath) {
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

std::string utils::Util::getDateTime() {
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