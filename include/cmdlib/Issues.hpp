/**
 * @file Issues.hpp cmdlib specific ERS Issues
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */
#ifndef CMDLIB_INCLUDE_CMDLIB_ISSUES_HPP_ 
#define CMDLIB_INCLUDE_CMDLIB_ISSUES_HPP_ 

#include <ers/ers.h>
#include <string>

namespace dunedaq {

/**
 * @brief cmdlib specific issues
 * */

    ERS_DECLARE_ISSUE(cmdlib, MalformedUriError,
                      "Malformed uri: " << error << " uri:" << uri,
                      ((std::string)error)
                      ((std::string)uri))

    // Throw if a URI is given which is not supported by the code.
    // Provide the erroneous URI as arg.
    ERS_DECLARE_ISSUE(cmdlib, UnsupportedUri,
                      "Unsupported URI: " << uri,
                      ((std::string)uri))

    // Throw if a file can not be opened.  Provide "mode" of "reading"
    // or "writing" and provide erroneous filename as args.
    ERS_DECLARE_ISSUE(cmdlib, BadFile,
                      "Can not open file for " << mode << ": " << filename,
                      ((std::string)filename)
                      ((std::string)mode))

    // Throw if a stream of data is exhausted.  Use in methods that
    // are given semantics that imply the stream is not expected to
    // become exhausted.  Ie, GetNext() may throw but something like
    // GetNext(return_if_exhausted) -> value must not.  Name the
    // stream and describe the error in the args.
    ERS_DECLARE_ISSUE(cmdlib, StreamExhausted,
                      "Stream exhausted: " << stream_name << " err: " << err,
                      ((std::string)stream_name)
                      ((std::string)err))

    // Throw if a stream of data being consumed has values which are
    // not expected.  Give the stream name and error message as args.
    ERS_DECLARE_ISSUE(cmdlib, StreamCorrupt,
                      "Stream corrupt: " << stream_name << " err: " << err,
                      ((std::string)stream_name)
                      ((std::string)err))                  

    // Throw if an operation is the result of logically inconsistent
    // code.  Describe the error in the "what" arg.
    ERS_DECLARE_ISSUE(cmdlib, InternalError,
                      "Internal error: " << what,
                      ((std::string)what))    

    ERS_DECLARE_ISSUE(cmdlib, CommandParserError,
                      "Failed to parse command: " << error,
                      ((std::string)error))

    ERS_DECLARE_ISSUE(cmdlib, CommandedObjectExecutionError,
                      "CommandedObject execution error: " << error,
                      ((std::string)error)) 

    ERS_DECLARE_ISSUE(cmdlib, CommandFacilityError,
                      "CommandFacilityError: " << error,
                      ((std::string)error))

    ERS_DECLARE_ISSUE(cmdlib, CommandFacilityCreationFailed,
                      "Failed to create CommandFacility uri:" << uri,
                      ((std::string)uri))

} // namespace dunedaq  

#endif // CMDLIB_INCLUDE_CMDLIB_ISSUES_HPP_ 
