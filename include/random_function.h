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

#include <fstream>

// helper function to use gRPC source code related to ssl tsl to read cert and key file
std::string ReadFile(const std::string& src_path) {
  std::ifstream src;
  src.open(src_path, std::ifstream::in | std::ifstream::binary);

  std::string contents;
  src.seekg(0, std::ios::end);
  contents.reserve(src.tellg());
  src.seekg(0, std::ios::beg);
  contents.assign((std::istreambuf_iterator<char>(src)),
                  (std::istreambuf_iterator<char>()));
  return contents;
}