#include <daemoncpp.h>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds
#include <fstream>
#include <string>

DaemonCpp * daemon;

const char sensorPath[] = "/sys/class/thermal/thermal_zone1/temp";

void loop()
{
	std::ifstream tempFile;
	tempFile.open(sensorPath);
	std::string temp;
	if (tempFile.is_open()) 
	{
		while (!tempFile.eof()) 
		{
			tempFile >> temp;
		}
	}
	tempFile.close();
	
	temp.insert(2,".");
	daemon->log(temp);

	std::this_thread::sleep_for (std::chrono::seconds(1));
}

int main(int argc, char **argv) 
{
	daemon = new DaemonCpp("CPUtempLogger", "/var/log/cputemplogger.log");
	daemon->demonize();
	daemon->setLoopFunction(loop);
	
	daemon->exec();
	
	return 0;
}
