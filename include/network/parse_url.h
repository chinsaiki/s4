#pragma once

#include <string>
#include <stdint.h>
#include <cstring>
#include <sstream>

#include <iostream>


#include <algorithm>


namespace S4 {

#ifdef _MSC_VER
#pragma warning(push)
# pragma warning(disable: 4244) 
#endif

class HTTPURL
{
private:
    std::string _protocol; // http vs https
    std::string _domain;   // mail.google.com
    uint16_t _port;   // 80,443
    std::string _path;     // /mail/
    std::string _query;    // [after ?] a=b&c=b

public:
    const std::string &protocol;
    const std::string &domain;
    const uint16_t &port;
    const std::string &path;
    const std::string &query;

    HTTPURL(const std::string &url) : protocol(_protocol), domain(_domain), port(_port), path(_path), query(_query)
    {
        std::string u = _trim(url);
        size_t offset = 0, slash_pos, hash_pos, colon_pos, qmark_pos;
        std::string urlpath, urldomain, urlport;
        uint16_t default_port;

        static const char *allowed[] = {"https://", "http://", "ftp://", NULL};
        for (int i = 0; allowed[i] != NULL && this->_protocol.length() == 0; i++)
        {
            const char *c = allowed[i];
            if (u.compare(0, strlen(c), c) == 0)
            {
                offset = strlen(c);
                this->_protocol = std::string(c, 0, offset - 3);
            }
        }
        default_port = this->_protocol == "https" ? 443 : 80;
        slash_pos = u.find_first_of('/', offset + 1);
        urlpath = slash_pos == std::string::npos ? "/" : u.substr(slash_pos);
        urldomain = std::string(u.begin() + offset, slash_pos != std::string::npos ? u.begin() + slash_pos : u.end());
        urlpath = (hash_pos = urlpath.find("#")) != std::string::npos ? urlpath.substr(0, hash_pos) : urlpath;
        urlport = (colon_pos = urldomain.find(":")) != std::string::npos ? urldomain.substr(colon_pos + 1) : "";
        urldomain = urldomain.substr(0, colon_pos != std::string::npos ? colon_pos : urldomain.length());
        this->_domain = _toLower(urldomain);
        this->_query = (qmark_pos = urlpath.find("?")) != std::string::npos ? urlpath.substr(qmark_pos + 1) : "";
        this->_path = qmark_pos != std::string::npos ? urlpath.substr(0, qmark_pos) : urlpath;
        this->_port = urlport.length() == 0 ? default_port : (uint16_t)_atoi(urlport);
    };

private:
    static inline std::string _trim(const std::string &input)
    {
        std::string str = input;
        size_t endpos = str.find_last_not_of(" \t\n\r");
        if (std::string::npos != endpos)
        {
            str = str.substr(0, endpos + 1);
        }
        size_t startpos = str.find_first_not_of(" \t\n\r");
        if (std::string::npos != startpos)
        {
            str = str.substr(startpos);
        }
        return str;
    };
    static inline std::string _toLower(const std::string &input)
    {
        std::string str = input;
        std::transform(str.begin(), str.end(), str.begin(), ::tolower);
        return str;
    };
    static inline int _atoi(const std::string &input)
    {
        int r;
        std::stringstream(input) >> r;
        return r;
    };
};

// using std::cerr;
// using std::cout;
// using std::endl;
// int main(int argc, char **argv)
// {
//     HTTPURL u("https://Mail.google.com:80/mail/?action=send#action=send");
//     cout << "protocol: " << u.protocol << endl;
//     cout << "domain: " << u.domain << endl;
//     cout << "port: " << u.port << endl;
//     cout << "path: " << u.path << endl;
//     cout << "query: " << u.query << endl;
//     return 0;
// }

#ifdef _MSC_VER
#pragma warning(pop)
#endif

}//namespace YY