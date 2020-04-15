/*
 * Library to make daemons in c++ projects.
 * 
 * Copyright (C) 2020  Paweł Janik <pawel@robotyka.net.pl>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef DAEMON_H
#define DAEMON_H

#include <fstream>
#include <csignal>

namespace
{
  volatile std::sig_atomic_t signalStatus;
}

/**
 * @todo Class to run daemon in c++ project
 */
class DaemonCpp
{
public:
	/**
	* @brief Konstruktor
	* 
	* @param appName p_appName: applicaton name to use in syslog
	* @param logFileName p_logFileName: path to file deamon logs, for example /var/log/daemon.log
	*/
	DaemonCpp(char * appName, char * logFileName = nullptr);

	/**
	* @brief Destructor
	* 
	*/
	~DaemonCpp();

	/**
	* @brief Logging function
	* 
	* @param log p_log: string with log
	*/
	void log(std::string log);
	/**
	* @brief Demonize applications
	* 
	* @return int: Return SID
	*/
	int demonize();
	/**
	* @brief Run daemon loop and wait for SIGTERM signal 
	* 
	*/
	void exec();
	
	/**
	* @brief Set user function to loop
	* 
	* @param userLoop p_userLoop: user loop function
	*/
	void setLoopFunction(void(userLoop)());
	
private:
	void static handleSignal(int sig);
	
	char * logName;
	bool isLogConfig;
	
	std::ofstream logFile;
	
	int running;
	
	void (*loop)();
};

#endif // DAEMON_H
