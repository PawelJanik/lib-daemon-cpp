/*
 * Library to make daemons in c++ projects.
 * 
 * Copyright (C) 2020  Paweł Janik <email>
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

#include "../include/daemoncpp.h"

#include <unistd.h>
#include <syslog.h>
#include <sys/stat.h>
#include <ctime>

DaemonCpp::DaemonCpp(char * appName, char * logFileName)
{
	running = 1;
	
	openlog(appName, LOG_CONS | LOG_NDELAY | LOG_PERROR | LOG_PID, LOG_USER);
	
	if(logFileName != nullptr)
	{
		logName = logFileName;
		isLogConfig = true;
		syslog(LOG_INFO, "Daemon logs was configured");
		
	}
	else
	{
		isLogConfig = false;
	}
}

DaemonCpp::~DaemonCpp()
{
	syslog(LOG_INFO, "Stoping daemon");
	log("Close syslog");
	closelog();
	log("Stop daemon");
}

int DaemonCpp::demonize()
{
	syslog(LOG_INFO, "Starting daemon");
	log("Starting daemon");

	pid_t pid, sid;

	//Fork the Parent Process
	pid = fork();

	if (pid < 0) { exit(EXIT_FAILURE); }

	//We got a good pid, Close the Parent Process
	if (pid > 0) { exit(EXIT_SUCCESS); }

	//Change File Mask
	umask(0);

	//Create a new Signature Id for our child
	sid = setsid();
	if (sid < 0) { exit(EXIT_FAILURE); }

	//Change Directory
	//If we cant find the directory we exit with failure.
	if ((chdir("/")) < 0) { exit(EXIT_FAILURE); }

	//Close Standard File Descriptors
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
	
	std::signal(SIGTERM, handleSignal);
	 
	running = 1;
	
	return sid;
}

void DaemonCpp::handleSignal(int sig)
{
	signalStatus = sig;
}

void DaemonCpp::log(std::string log)
{
	time_t now = time(0);
	tm *ltm = localtime(&now);
	
	std::string timeString;
	timeString.append(std::to_string(1900 + ltm->tm_year)).append("-");
	timeString.append(std::to_string(ltm->tm_mon + 1)).append("-");
	timeString.append(std::to_string(ltm->tm_mday)).append(" ");

	timeString.append(std::to_string(ltm->tm_hour)).append(":");
	timeString.append(std::to_string(ltm->tm_min)).append(":");
	timeString.append(std::to_string(ltm->tm_sec));	
	
	std::string logString;
	logString.append(timeString).append(" - ").append(log).append("\n");
	
	if(isLogConfig)
	{
		logFile.open(logName, std::ios::out | std::ios::app);
		logFile << logString;
		logFile.close();
	}
	else
	{
		syslog(LOG_INFO, logString.c_str());
	}
}

void DaemonCpp::setLoopFunction(void ( userLoop)())
{
	loop = userLoop;
}


void DaemonCpp::exec()
{
	while (running == 1) 
	{
		(*loop)();
		
		if(signalStatus == SIGTERM)
		{
			running = 0;
		}
	}
}

