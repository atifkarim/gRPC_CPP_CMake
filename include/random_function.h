#include <fstream>
#include <sstream>

std::string read_keycert(const char* filename)
{
     std::ifstream file(filename);
     std::string temp;
     std::getline(file, temp);
     file.close();
     return temp;
}

void
read ( const std::string& filename, std::string& data )
{
        std::ifstream file ( filename.c_str (), std::ios::in );

	if ( file.is_open () )
	{
		std::stringstream ss;
		ss << file.rdbuf ();

		file.close ();

		data = ss.str ();
	}

	return;
}