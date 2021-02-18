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

#include <ers/ers.h>
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

    ERS_INFO("Loading commands from file: " << fname);
    try {
      std::ifstream ifs;
      ifs.open(fname, std::fstream::in);
      if (!ifs.is_open()) {
        throw dunedaq::cmdlib::CommandParserError(ERS_HERE, "Can't open command file!");
      }
      m_raw_commands = json::parse(ifs);
    } catch (const std::exception& ex) {
      throw dunedaq::cmdlib::CommandParserError(ERS_HERE, ex.what());
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
    ERS_INFO("Entered commands will be launched on CommandedObject...");
    std::string cmdid;
    while (end_marker) { //until runmarker
      ERS_INFO(m_available_str);
      // feed commands from cin
      std::cin >> cmdid;
      if (std::cin.eof()) {
        break;
      }
      if ( m_available_commands.find(cmdid) == m_available_commands.end() ) {
        ERS_INFO("Command " << cmdid << " is not available...");
      } else {
        ERS_INFO("Executing " << cmdid << " command...");
        inherited::execute_command(m_available_commands[cmdid], cmdmeta_t());
      }
    }
    ERS_INFO("Command handling stopped.");
  }

protected:
  typedef CommandFacility inherited;

  json m_raw_commands;
  std::map<std::string, json> m_available_commands;
  std::string m_available_str;

  // Implementation of completion_handler interface
  void completion_callback(const cmdobj_t& cmd, cmdmeta_t& meta) {
    ERS_INFO("Command " << cmd << "\nexecution resulted with: " << meta["result"]);
  }

};

extern "C" {
    std::shared_ptr<dunedaq::cmdlib::CommandFacility> make(std::string uri) {
        return std::shared_ptr<dunedaq::cmdlib::CommandFacility>(new stdinCommandFacility(uri));
    }
}
