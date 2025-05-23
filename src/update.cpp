/* Update logic and stats from py/lua */
#include "visualdb.h"

std::ifstream queryfile;

void update() 
{
	queryfile.open("python/query.txt");
	if (queryfile.is_open()) { // ok, proceed with output
		query = std::string((std::istreambuf_iterator<char>(queryfile)), std::istreambuf_iterator<char>());
		//std::cout << "ok the query file is loaded... " + query << std::endl;
		queryfile.close();
	}
	
}