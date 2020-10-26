#include <string>
#include <iostream>
#include "cmdlib/Utils.hpp"

static
void run_dump(std::string url)
{
    // parse_url(url);
    auto ret = dunedaq::url::parse(url);
    std::cout << "[" << url << "]" << std::endl;
    std::cout << "Scheme: " << ret.scheme << std::endl;
    std::cout << "Domain: " << ret.domain << std::endl;
    std::cout << "Port:   " << ret.port << std::endl;
    std::cout << "Path:   " << ret.path << std::endl;
    std::cout << "Queries: [" << ret.queries.size() << "]" << std::endl;
    for (const auto& [key,val] : ret.queries) {
        std::cout << "\t\"" << key << "\" = \"" << val << "\"" << std::endl;
    }
    std::cout << "-------------------------------" << std::endl;

}

int main(int argc, char* argv[])
{
    run_dump("file:zero.json");
    run_dump("file://two.json?illegal=yes&fileis=domain");
    run_dump("file:///three.json");
    run_dump("file:////four.json?microsoft=sucks&dont=doit");
    run_dump("relative.json?fmt=jstream&another=42");
    run_dump("/dev/stdin?fmt=jstream&another=42");
    run_dump("/dev/stdin?fmt=json&foo=bar");
    run_dump("http://example.com:4321/path?foo=bar");
    // not supported:
    //run_dump("http://user:pass@example.com:4321/path?foo=bar");
    run_dump("mailto:someone@example.com");
    for (int iarg=1; iarg < argc; ++iarg) {
        run_dump(argv[iarg]);
        //const std::string maybe = argv[iarg];
    }
    return 0;
}
