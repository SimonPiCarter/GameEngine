#include "engine/BlocEngine.h"

#include <iostream>

int main(int argc, char **argv)
{
	if(argc != 2)
	{
		std::cerr<<"expected call : "<<argv[0] << "path_to_data_folder"<<std::endl;
		return -1;
	}
	BlocEngine engine_l(argv[1]);

	engine_l.init();
	engine_l.run();

	return 0;
}