#define BOOST_TEST_MODULE cmdlib_unit_tests
#include <boost/test/unit_test.hpp>

#include "cmdlib/CommandFacility.hpp"
#include "cmdlib/CommandResult.hpp"

#include <chrono>
#include <condition_variable>
#include <mutex>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

using namespace dunedaq::cmdlib;

namespace {

class RecordingCommandFacility : public CommandFacility
{
public:
  explicit RecordingCommandFacility(const std::string& uri)
    : CommandFacility(uri)
  {}

  void run(std::atomic<bool>&) override {}

  bool wait_for_callbacks(std::size_t expected, std::chrono::milliseconds timeout)
  {
    std::unique_lock<std::mutex> lock(m_mutex);
    return m_condition.wait_for(lock, timeout, [this, expected] { return m_callbacks.size() >= expected; });
  }

  std::pair<cmdobj_t, cmd::CommandReply> last_callback()
  {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_callbacks.back();
  }

protected:
  void completion_callback(const cmdobj_t& cmd, cmd::CommandReply& meta) override
  {
    {
      std::lock_guard<std::mutex> lock(m_mutex);
      m_callbacks.emplace_back(cmd, meta);
    }
    m_condition.notify_one();
  }

private:
  std::mutex m_mutex;
  std::condition_variable m_condition;
  std::vector<std::pair<cmdobj_t, cmd::CommandReply>> m_callbacks;
};

class SuccessfulCommandedObject : public CommandedObject
{
public:
  void execute(const cmdobj_t& command) override
  {
    m_last_command = command;
    ++m_calls;
  }

  int calls() const { return m_calls; }

private:
  int m_calls{ 0 };
  cmdobj_t m_last_command{};
};

class ThrowingCommandedObject : public CommandedObject
{
public:
  void execute(const cmdobj_t&) override { throw std::runtime_error("intentional test failure"); }
};

} // namespace

BOOST_AUTO_TEST_CASE(command_result_defaults_and_ctor)
{
  dune::daq::ccm::CommandResult def("unset", -999, "unset");
  BOOST_CHECK_EQUAL(def.m_answer_addr, "unset");
  BOOST_CHECK_EQUAL(def.m_answer_port, -999);
  BOOST_CHECK_EQUAL(def.m_result, "unset");

  dune::daq::ccm::CommandResult custom("127.0.0.1", 6000, "OK");
  BOOST_CHECK_EQUAL(custom.m_answer_addr, "127.0.0.1");
  BOOST_CHECK_EQUAL(custom.m_answer_port, 6000);
  BOOST_CHECK_EQUAL(custom.m_result, "OK");
}

BOOST_AUTO_TEST_CASE(command_facility_success_path_sets_reply_fields)
{
  RecordingCommandFacility fac("test://");
  SuccessfulCommandedObject obj;
  fac.set_commanded(obj, "unit-app");

  cmdobj_t cmd = { { "action", "ping" } };
  fac.execute_command(cmd, cmd::CommandReply{});

  BOOST_REQUIRE(fac.wait_for_callbacks(1, std::chrono::seconds(2)));
  auto [completed_cmd, reply] = fac.last_callback();

  BOOST_CHECK_EQUAL(completed_cmd.dump(), cmd.dump());
  BOOST_CHECK(reply.success);
  BOOST_CHECK_EQUAL(reply.result, "OK");
  BOOST_CHECK_EQUAL(reply.appname, "unit-app");
  BOOST_CHECK_EQUAL(obj.calls(), 1);
}

BOOST_AUTO_TEST_CASE(command_facility_exception_path_sets_failure_fields)
{
  RecordingCommandFacility fac("test://");
  ThrowingCommandedObject obj;
  fac.set_commanded(obj, "unit-app");

  cmdobj_t cmd = { { "action", "explode" } };
  fac.execute_command(cmd, cmd::CommandReply{});

  BOOST_REQUIRE(fac.wait_for_callbacks(1, std::chrono::seconds(2)));
  auto [completed_cmd, reply] = fac.last_callback();

  BOOST_CHECK_EQUAL(completed_cmd.dump(), cmd.dump());
  BOOST_CHECK(!reply.success);
  BOOST_CHECK_NE(reply.result.find("intentional test failure"), std::string::npos);
  BOOST_CHECK_EQUAL(reply.appname, "unit-app");
}
