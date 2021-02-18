/**
 * @file CommandFacility.hpp CommandFacility base definitions
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */
#ifndef CMDLIB_INCLUDE_CMDLIB_COMMANDFACILITY_HPP_
#define CMDLIB_INCLUDE_CMDLIB_COMMANDFACILITY_HPP_

#include "CommandedObject.hpp"
#include "Issues.hpp"

#include <cetlib/BasicPluginFactory.h>
#include <cetlib/compiler_macros.h>

#include <tbb/concurrent_queue.h>

#include <future>
#include <functional>
#include <atomic>
#include <memory>
#include <string>

#ifndef EXTERN_C_FUNC_DECLARE_START
#define EXTERN_C_FUNC_DECLARE_START                                                                                    \
  extern "C"                                                                                                           \
  {
#endif

/**
 * @brief Declare the function that will be called by the plugin loader
 * @param klass Class to be defined as a DUNE Command Facility
 */
#define DEFINE_DUNE_COMMAND_FACILITY(klass)                                                                            \
  EXTERN_C_FUNC_DECLARE_START                                                                                          \
  std::unique_ptr<dunedaq::cmdlib::CommandFacility> make()                                                             \
  {                                                                                                                    \
    return std::unique_ptr<dunedaq::cmdlib::CommandFacility>(new klass());                                             \
  }                                                                                                                    \
  } 

namespace dunedaq::cmdlib {

/**
 * @brief Interface needed by DAQ apps and services for command handling
 */
class CommandFacility
{
public:
  explicit CommandFacility(std::string /*uri*/) {}
  ~CommandFacility();
  CommandFacility(const CommandFacility&) = 
    delete; ///< CommandFacility is not copy-constructible
  CommandFacility& operator=(const CommandFacility&) =
    delete; ///< CommandFacility is not copy-assignable
  CommandFacility(CommandFacility&&) =
    delete; ///< CommandFacility is not move-constructible
  CommandFacility& operator=(CommandFacility&&) =
    delete; ///< CommandFacility is not move-assignable

  //! Meant to be called once from main
  void set_commanded(CommandedObject& commanded);

  //! Meant to be called once from main (implementation specific)
  virtual void run(std::atomic<bool>& end_marker) = 0;

  //! Feed commands from the implementation.
  void execute_command(const cmdobj_t& cmd, cmdmeta_t meta);

protected:
  //! Must be implemented to handling the results of the commands
  virtual void completion_callback(const cmdobj_t& cmd, cmdmeta_t& meta) = 0; 

private:

  //! The glue between commanded and completion callback
  void handle_command(const cmdobj_t& cmd, cmdmeta_t meta);

  void executor();

  //! Commanded Object to run execute with received commands as parameters
  mutable CommandedObject* m_commanded_object = nullptr;

  //! Completion queue for reqistered tasks
  typedef tbb::concurrent_queue<std::future<void>> CompletionQueue;
  CompletionQueue m_completion_queue;

  //! Request callback function signature
  typedef std::function<void(const cmdobj_t&, cmdmeta_t)> CommandCallback;
  CommandCallback m_command_callback = nullptr;

  //! Single thrad is responsible to trigger tasks 
  std::atomic<bool> m_active;

  std::thread m_executor;

};

std::shared_ptr<CommandFacility>
makeCommandFacility(std::string const& uri)
{
  auto sep = uri.find("://");
  std::string scheme;
  if (sep == std::string::npos) { // simple path
      scheme = "stdin";
  } else { // with scheme
      scheme = uri.substr(0, sep);
  }
  std::string plugin_name = scheme + "CommandFacility";
  static cet::BasicPluginFactory bpf("duneCommandFacility", "make");
  std::shared_ptr<CommandFacility> cf_ptr;
  try {
    cf_ptr = bpf.makePlugin<std::shared_ptr<CommandFacility>>(plugin_name, uri);
  } catch (const cet::exception &cexpt) {
    throw CommandFacilityCreationFailed(ERS_HERE, uri, cexpt);
  } catch (const ers::Issue &iexpt) {
    throw CommandFacilityCreationFailed(ERS_HERE, uri, iexpt);
  } catch (...) {  // NOLINT JCF Jan-27-2021 violates letter of the law but not the spirit
    throw CommandFacilityCreationFailed(ERS_HERE, uri, "Unknown error.");
  }
  return cf_ptr;
}

} // namespace dunedaq::cmdlib

#endif // CMDLIB_INCLUDE_CMDLIB_COMMANDFACILITY_HPP_
