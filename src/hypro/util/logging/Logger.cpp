#include "Logger.h"

namespace hypro {

	int initializeLogging(int& i) {
		carl::logging::logger().configure("logfile", "hypro.log");
		carl::logging::logger().filter("logfile")
	    ("hypro", carl::logging::LogLevel::LVL_WARN)
		("hypro.representations", carl::logging::LogLevel::LVL_WARN)
		;

	  	carl::logging::logger().configure("stdout", std::cout);
		carl::logging::logger().filter("stdout")
	    ("hypro", carl::logging::LogLevel::LVL_TRACE)
		("hypro.representations", carl::logging::LogLevel::LVL_WARN)
		;

	  	carl::logging::logger().resetFormatter();

	  	// avoid warning of unused variable by casting it to void (thus it is used at least once).
	  	(void)i;

	  	return 0;
	}

} // namespace hypro
