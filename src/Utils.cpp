#include "cmdlib/Utils.hpp"
#include "cmdlib/Issues.hpp"
#include <regex>

using namespace dunedaq;

// Helper to parse URL query string, eg "foo=bar&baz=quax"
static 
url::queries_t parse_queries(std::string qs)
{
    url::queries_t ret;
    if (qs.empty()) {
        return ret;
    }
    while(true) {
        auto sep = qs.find_first_of("&");
        std::string one;
        if (sep == qs.npos) {
            one = qs;
        }
        else {
            one = qs.substr(0, sep);
        }
        auto kitr = one.find_first_of("=");
        if (kitr == one.npos) {
            ret[one] = "";
        }
        else {
            auto key = one.substr(0,kitr);
            auto val = one.substr(kitr+1);
            ret[key] = val;
        }
        if (sep == qs.npos) {
            break;
        }
        qs = qs.substr(sep+1);
    }
    return ret;
}

static
std::string get_match(const std::cmatch& what, size_t ind) {
    return std::string(what[ind].first, what[ind].second);
}

url::Parts url::parse(std::string urlstr)
{
    // taken from random SE's plus add port finding
    std::regex ex (
        R"(^(([^:\/?#]+):)?(//([^\/:?#]*)(:([0-9]+))?)?([^?#]*)(\?([^#]*))?(#(.*))?)",
        std::regex::extended
        );
    std::cmatch what;

    if(regex_match(urlstr.c_str(), what, ex)) {
        return url::Parts{
            get_match(what, 2), // scheme
            get_match(what, 4), // domain
            get_match(what, 6), // port
            get_match(what, 7), // path
            parse_queries(get_match(what, 9))};
    }
    throw cmdlib::MalformedUriError(ERS_HERE, "failed to parse URI", urlstr);
    //throw std::runtime_error("failed to parse: " + urlstr);
}
