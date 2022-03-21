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

// following function will help to find out the number of total chunk
/**
 * \param [in] uint64_t num_of_container_content Total size of the data container. Eg: vec.size()
 * \param [in] preferred_chunk_size_in_kibyte Size in Kibyte which will be carried by 1 chunk
 * \param [in] preferred_chunk_size_in_kibyte_holds_integer_num Total number of content in each chunk
 *
 */
#include <cmath>
uint32_t total_chunk_counter(uint64_t num_of_container_content,
                             uint64_t preferred_chunk_size_in_kibyte,
                             uint64_t &preferred_chunk_size_in_kibyte_holds_integer_num)
{
	uint64_t cotainer_size_in_kibyte = (32ULL * num_of_container_content) / 1024;
	preferred_chunk_size_in_kibyte_holds_integer_num = (num_of_container_content * preferred_chunk_size_in_kibyte) / cotainer_size_in_kibyte;

	float total_chunk = static_cast<float>(num_of_container_content) / preferred_chunk_size_in_kibyte_holds_integer_num;
	return std::ceil(total_chunk);
}