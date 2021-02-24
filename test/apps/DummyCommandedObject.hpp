/**
 * @file dummy_commanded_object_.hpp Dummy commanded object
 * implementation of the CommandedObject interface
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */
#ifndef CMDLIB_TEST_APPS_DUMMYCOMMANDEDOBJECT_HPP_
#define CMDLIB_TEST_APPS_DUMMYCOMMANDEDOBJECT_HPP_

#include "cmdlib/CommandedObject.hpp"

#include <logging/Logging.hpp>

#include <stdexcept>
#include <string>

class DummyCommandedObject : public dunedaq::cmdlib::CommandedObject
{
public:
  void execute(const dunedaq::cmdlib::cmdobj_t& command) {
    if (command.dump() == "{\"asd\":true}") {
      TLOG() << command.dump() << " is a REALLY slow command";
      std::this_thread::sleep_for(std::chrono::seconds(5));
      throw std::runtime_error("Testing throw.");
    } else {
      TLOG() << "I executed cmd: " << command.dump();
    }
  }
};

#endif // CMDLIB_TEST_APPS_DUMMYCOMMANDEDOBJECT_HPP_
