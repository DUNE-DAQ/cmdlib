/**
 * @file stdinCommandFacility.cpp CommandFacility implementation
 * that reads commands from std::cin
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */
#include "cmdlib/CommandFacility.hpp"
#include "cmdlib/Issues.hpp"

#include <logging/Logging.hpp>
#include <nlohmann/json.hpp>
#include <cetlib/BasicPluginFactory.h>

#include <thread>
#include <chrono>
#include <map>
#include <memory>
#include <string>
#include <fstream>

using namespace dunedaq::cmdlib;
using namespace std::chrono_literals;
using json = nlohmann::json;

    // Throw if a file can not be opened.  Provide "mode" of "reading"
    // or "writing" and provide erroneous filename as args.
    ERS_DECLARE_ISSUE(cmdlib, BadFile,
                     "Can not open file for " << mode << ": " << filename,
                      ((std::string)filename)
                      ((std::string)mode))
    

class stdinCommandFacility : public CommandFacility
{
public:
  explicit stdinCommandFacility(std::string uri) : CommandFacility(uri) { 
    // Allocate resources as needed
    auto col = uri.find_last_of(':');
    auto sep = uri.find("://");
    std::string fname;
    if (col == std::string::npos || sep == std::string::npos) { // assume filename
      fname = uri;
    } else {
      fname = uri.substr(sep+3);
    }

    TLOG_LOG() <<"Loading commands from file: " << fname;
   
    std::ifstream ifs;
    ifs.open(fname, std::fstream::in);
    if (!ifs.is_open()) {
      throw dunedaq::cmdlib::BadFile(ERS_HERE, fname, "reading");
    } 

    try {
      m_raw_commands = json::parse(ifs);
    } catch (const std::exception& ex) {
      throw dunedaq::cmdlib::CannotParseCommand(ERS_HERE, ex.what());
    }
    std::ostringstream avaostr;
    avaostr << "Available commands:";
    for (auto it = m_raw_commands.begin(); it != m_raw_commands.end(); ++it) {
      std::string idstr(it.value()["id"]);
      m_available_commands[idstr] = it.value();
      avaostr << " | " << idstr;
    }
    m_available_str = avaostr.str();
  }

  // Implementation of the runner
  void run(std::atomic<bool>& end_marker) {
    TLOG_DEBUG(1) << "Entered commands will be launched on CommandedObject...";
    std::string cmdid;
    while (end_marker) { //until runmarker
      TLOG_LOG() << m_available_str;
      // feed commands from cin
      std::cin >> cmdid;
      if (std::cin.eof()) {
        break;
      }
      if ( m_available_commands.find(cmdid) == m_available_commands.end() ) {
        std::ostringstream s;
	s << "Command " << cmdid << " is not available...";
        ers::error (CannotParseCommand(ERS_HERE, s.str());
      } else {
        TLOG_LOG() << "Executing " << cmdid << " command...";
        inherited::execute_command(m_available_commands[cmdid], cmdmeta_t());
      }
    }
    TLOG_DEBUG(1) << "Command handling stopped.";
  }

protected:
  typedef CommandFacility inherited;

  json m_raw_commands;
  std::map<std::string, json> m_available_commands;
  std::string m_available_str;

  // Implementation of completion_handler interface
  void completion_callback(const cmdobj_t& cmd, cmdmeta_t& meta) {
    TLOG_LOG() << "Command " << cmd << "\nexecution resulted with: " << meta["result"];
  }

};

extern "C" {
    std::shared_ptr<dunedaq::cmdlib::CommandFacility> make(std::string uri) {
        return std::shared_ptr<dunedaq::cmdlib::CommandFacility>(new stdinCommandFacility(uri));
    }
}
