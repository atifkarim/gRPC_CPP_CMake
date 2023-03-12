#include "client_zone.h"

// Calculates the number of chunk
/**
 * \param [in] data_size Total size of given/provided data which will be passed as chunk. Eg: myvec.size()
 * \param [in] chunk_size Chunk size in KiloByte. This chunk size will be given by the user. Right now less than 64 KB is not allowed
 * \param [in] sample Data sample in each chunk
 */
void grpc_client::get_chunk_number(uint64_t data_size,
                                   uint64_t chunk_size,
                                   uint64_t &sample,
                                   uint32_t &total_chunk)
{
	// Each integer size is 4 Byte. That's why N integer size will be N * 4 bit
	// To convert this into KB we have to divide the prior result with 1024. Eg: (vector.size() * 4) / 1024
	uint64_t data_size_kb = (4ULL * data_size) / 1024;

	float total_chunk_intermediate = static_cast<float>(data_size) / sample;
	total_chunk = std::ceil(total_chunk_intermediate);
}

// After producing large data set it passes them & get response from server as chunk
void grpc_client::data_chunk_stream_request()
{
	demo_grpc::Large_Data_Request request_;
	demo_grpc::Large_Data_Response response_;

	// This chunk size will be provided by the user. It will not be greater than 4 MB and will be provided in KB unit.
	// Eg: If you want to provide a chunk size of 3 MB you have to provide 3072 (3MB = 3 * 1024 KB)
	// Also, this value will be a multiple of 64. That means at least a chunk size should be provided which can carry
	// 64 KB data
	uint64_t chunk_size;
	std::cout << std:: endl << "Provide chunk size (KB). Not more than 4 MB. Will be multiple of 64: ";
	std::cin >> chunk_size;

	if (static_cast<float>(chunk_size) / 1024 > 4)
	{
		throw std::runtime_error("Chunk size is > 4 MB. Message field of protobuf is limited to 4 MB");
	}
	if (chunk_size % 64 != 0)
	{
		throw std::runtime_error("Please provide chunk size in multiple of 64 otherwise Number of chunk will be fractional");
	}

	// This vector is the container to hold dummy data created in client side
	std::vector<int32_t> dummy_data_set;

	// This is the size of the vector which will be chose by customer
	uint32_t vector_size;

	std::cout << "Dummy data will be create in client side. Please provide a size of vector: ";
	std::cin >> vector_size;
	for(int64_t i = 0; i < vector_size; i++)
	{
		dummy_data_set.push_back(2);
	}

	// Number of data in each chunk. Each integer is 4 byte. If chunk size is 64 KB that means "sample" will be number of
	// Integers resides in this each chunk size.
	// Convert this 64 KB to byte = 64 * 1024 Byte.
	// 4 byte is taken by 1 integer, so, 64 * 1024 bytes is taken by (64*1024)/4 integer
	uint64_t sample = (chunk_size * 1024)/4;

	// Number of chunk to transfer whole data set (here, dummy_data_set)
	uint32_t total_chunk = 0;

	get_chunk_number(dummy_data_set.size(), chunk_size, sample, total_chunk);

	std::cout << "Information in Client" << std::endl
	          << "*********************" << std::endl
	          << "Dummy data set size                       : " << dummy_data_set.size() << std::endl
	          << "Sum of the content of dummy data set      : " << std::accumulate(dummy_data_set.begin(), dummy_data_set.end(), 0ULL) << std::endl
	          << "Size of dummy data set's container(GB)    : " << static_cast<float>(dummy_data_set.size() * 32) / (1024*1024*1024) << std::endl
	          << "Number of data in each chunk              : " << sample << std::endl
	          << "Number of Total chunk                     : " << total_chunk << std::endl;

	// Proto Buffer message is preparing to pass to Server
	request_.set_chunk_data_length(sample);
	request_.set_client_data_stream_size(dummy_data_set.size());
	request_.set_required_chunk(total_chunk);
	request_.set_name(detected_area_name);

	grpc::ClientContext context;
	std::shared_ptr<::grpc::ClientReaderWriter< ::demo_grpc::Large_Data_Request, ::demo_grpc::Large_Data_Response> > stream(stub->Stream_Chunk_Service(&context));

	int32_t temp_count = 0;
	while(total_chunk > 0)
	{
		for (int64_t i = temp_count * sample; i < sample + temp_count * sample; i++)
		{
			// This condition checks the iteration number with the vector data size of the client
			// if client data size has reached then writing will be stopped
			if (i < dummy_data_set.size())
			{
				request_.add_chunk_data_client_request(dummy_data_set[i]);
			}
			else
			{
				break;
			}
		}
		temp_count++;
		total_chunk--;

		stream->Write(request_);

		// message field will be cleared after passing one chunk. It provides a fresh repeated field in the next iteration to pass a new chunk
		request_.clear_chunk_data_client_request();
	}

	stream->WritesDone();

	// Reading chunk response from server
	std::vector<int32_t> dummy_final_data_set;

	// This variable checks data size of client during read back from server
	uint32_t client_track_data_size_before_reading = 0;

	while (stream->Read(&response_))
	{
		for(int64_t i = 0; i < request_.chunk_data_length(); i++)
		{
			if (client_track_data_size_before_reading < dummy_data_set.size())
			{
				dummy_final_data_set.push_back(response_.chunk_data_server_response(i));
				client_track_data_size_before_reading++;
			}
			else
			{
				break;
			}
		}
	}

	grpc::Status status = stream->Finish();

	if (status.ok())
	{
		if (dummy_final_data_set.size() == response_.server_data_stream_size() | dummy_final_data_set.size() < response_.server_data_stream_size())
		{
			std::cout << std::endl;
			std::cout << "*************************************" << std::endl;
			std::cout << "Server successfully has sent all data" << std::endl;
			std::cout << "*************************************" << std::endl;

			std::cout << std::endl;
			std::cout << request_.name() << " transmission from Server is Successful" << std::endl
			          << "Server has responded OK and stream/large data from server is stored in a vector in Client side" << std::endl
			          << "dummy_final_data_set size              : " << dummy_final_data_set.size() << std::endl
			          << "Sum of sample of dummy_final_data_set  : " << std::accumulate(dummy_final_data_set.begin(), dummy_final_data_set.end(), 0ULL) << std::endl
			          << std::endl;
		}

	}
	else
		std::cout << "!!!!! Server is Failed to Stream !!!!!" << std::endl;
}
