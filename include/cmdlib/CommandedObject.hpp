/**
 * @file CommandedObject.hpp CommandedObject interface
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */
#ifndef CMDLIB_INCLUDE_CMDLIB_COMMANDEDOBJECT_HPP_
#define CMDLIB_INCLUDE_CMDLIB_COMMANDEDOBJECT_HPP_

#include <nlohmann/json.hpp>

namespace dunedaq::cmdlib {

typedef nlohmann::json cmdobj_t;
typedef nlohmann::json cmdmeta_t;

/**
 * @brief Interface needed by commanded objects in the DAQ
 */
class CommandedObject
{
public:
  //! Pure virtual execute member
  virtual void execute(const cmdobj_t& command) = 0; 
};

} // namespace dunedaq::cmdlib

#endif // CMDLIB_INCLUDE_CMDLIB_COMMANDEDOBJECT_HPP__
