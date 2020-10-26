/**
 * @file Utils.hpp cmdlib utilities
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */
#ifndef CMDLIB_INCLUDE_CMDLIB_UTILS_HPP_ 
#define CMDLIB_INCLUDE_CMDLIB_UTILS_HPP_ 

#include <string>
#include <map>

namespace dunedaq {

    namespace url {

        using queries_t = std::map<std::string, std::string>;
        struct Parts {
            std::string scheme;
            std::string domain;
            std::string port;
            std::string path;
            queries_t queries;
        };

        /**
           Parse many but not all URI forms given by
           https://www.ietf.org/rfc/rfc2396.txt

           A file is specified as with a URI with a "file:" scheme or as a path.

           With "file:" scheme:

           - absolute :: file:///data/foo.json
           - relative :: file:foo.json

           That relative notation is an extension to rfc2396.
           O.w. the file scheme places after the "://" a
           "authoritative" aka "domain.  Thus two or four or more
           slashes are illegal:

           - illegal :: file://foo.json    # this makes "foo.json" a "domain"
           - illegal :: file:////foo.json  # we are not MicroSoft

           As a schemeless path:

           - absolute :: /data/foo.json
           - relative :: foo.json

           Both "file:" scheme and schemeless forms may take a URL
           query parameter string.  

           - example :: file:///data/foo.json?fmt=jstream
        */
        Parts parse(std::string url);

    }


} // namespace dunedaq  

#endif // CMDLIB_INCLUDE_CMDLIB_UTILS_HPP_ 
