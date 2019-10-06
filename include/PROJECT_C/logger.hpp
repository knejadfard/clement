#pragma once

#include <boost/core/null_deleter.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <fstream>

namespace PROJECT_C {

namespace log = boost::log;
using text_ostream_backend = log::sinks::text_ostream_backend;

class logger {

  log::sources::severity_logger_mt<log::trivial::severity_level> logger_;
  bool initialized_ = false;
  std::string file_name_;

public:
  logger() : file_name_{"output.log"} { initialize(); }

  logger(std::string const& file_name) : file_name_{file_name} { initialize(); }

  void initialize() {
    if (!initialized_) {
      boost::shared_ptr<log::core> core = log::core::get();
      boost::shared_ptr<text_ostream_backend> backend = boost::make_shared<text_ostream_backend>();
      backend->add_stream(boost::shared_ptr<std::ostream>(&std::clog, boost::null_deleter()));
      backend->add_stream(boost::shared_ptr<std::ostream>(new std::ofstream(file_name_)));

      // Enable auto-flushing after each log record written
      backend->auto_flush(true);

      // Wrap it into the frontend and register in the core.
      // The backend requires synchronization in the frontend.
      typedef log::sinks::synchronous_sink<text_ostream_backend> sink_t;
      boost::shared_ptr<sink_t> sink(new sink_t(backend));

      log::add_common_attributes();

      log::formatter fmt = log::expressions::stream
                           << log::expressions::format_date_time<boost::posix_time::ptime>(
                                  "TimeStamp", "%Y-%m-%d %H:%M:%S")
                           << " [" << log::trivial::severity << "] " << log::expressions::smessage;

      sink->set_formatter(fmt);
      core->add_sink(sink);

      core->set_filter(log::trivial::severity >= log::trivial::debug);

      initialized_ = true;
    }
  }

  void info(const std::string& message) { BOOST_LOG_SEV(logger_, log::trivial::info) << message; }

  void error(std::string const& message) { BOOST_LOG_SEV(logger_, log::trivial::error) << message; }

  void debug(std::string const& message) { BOOST_LOG_SEV(logger_, log::trivial::debug) << message; }
};

logger main_logger{"server.log"};

} // namespace PROJECT_C
