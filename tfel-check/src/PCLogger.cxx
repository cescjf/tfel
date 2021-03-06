/*!
 * \brief tfel_check-check Logger implementation
 *
 * \file PCLogger.cxx
 *
 * \author sb152252
 * \date 31 août 2009
 *
 * $Id$
 */

#include<iostream>
#include "TFELCheck/PCLogger.hxx"

namespace tfel_check {

  PCLogger::PCLogger() = default;
  PCLogger::PCLogger(PCLogger&&) = default;
  PCLogger::PCLogger(const PCLogger&) = default;
  PCLogger& PCLogger::operator=(PCLogger&&) = default;
  PCLogger& PCLogger::operator=(const PCLogger&) = default;
  PCLogger::~PCLogger() = default;
  
  PCLogger::PCLogger(const driver& logDriver) {
    this->addDriver(logDriver);
  }

  void PCLogger::addDriver(driver logDriver) {
    this->drivers.push_back(logDriver);
  }

  void PCLogger::addMessage(const std::string& message) {
    for (auto& d : this->drivers) {
      d->addMessage(message);
    }
  }

  void PCLogger::addSimpleTestResult(const std::string& testname, bool success,
				     const std::string& message) {
    this->addTestResult(testname, "", "", 0.0, success, message);
  }

  void PCLogger::addTestResult(const std::string& testname,
			       const std::string& step,
			       const std::string& command, float time,
			       bool success, const std::string& message) {
    for (auto& d : this->drivers) {
      d->addTestResult(testname, step, command, time, success, message);
    }
  }

  void PCLogger::terminate() {
    for (auto& d : this->drivers) {
      d->terminate();
    }
  }
}  // namespace tfel_check
