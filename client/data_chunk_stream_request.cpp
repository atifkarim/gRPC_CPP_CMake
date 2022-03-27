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
	uint64_t data_size_kb = (32ULL * data_size) / 1024;
	sample = (data_size * chunk_size) / data_size_kb;

	float total_chunk_intermediate = static_cast<float>(data_size) / sample;
	total_chunk = std::ceil(total_chunk_intermediate);
}

// After producing large data set it passes them & get response from server as chunk
void grpc_client::data_chunk_stream_request()
{
	demo_grpc::Large_Data_Request request_;
	demo_grpc::Large_Data_Response response_;

	uint64_t chunk_size;
	std::cout << std:: endl << "Chunk size in KiloByte: ";
	std::cin >> chunk_size;

	if (static_cast<float>(chunk_size) / 1024 > 4){
		throw std::runtime_error("Chunk size is > 4 MB. Message field of protobuf is limited to 4 MB");
	}
	if (chunk_size % 64 != 0){
		throw std::runtime_error("Please provide chunk size in multiple of 64 otherwise Number of chunk will be fractional");
	}

	std::vector<int32_t> dummy_data_set;
	std::cout << "Client is creation dummy data set" << std::endl;
	for(int64_t i = 0; i < 1024 * 1024; i++)
	{
		dummy_data_set.push_back(2);
	}

	std::cout << "Informtaion of Dummy Data Set" << std::endl
	          << "************************************" << std::endl
	          << "Data Sample                   : " << dummy_data_set.size() << std::endl
	          << "Sum of Data Sample            : " << std::accumulate(dummy_data_set.begin(), dummy_data_set.end(), 0ULL) << std::endl
	          << "Size of Dummy Data Set(GB)    : " << static_cast<float>(dummy_data_set.size() * 32) / (1024*1024*1024) << std::endl;

	uint64_t sample = 0;
	uint32_t total_chunk = 0;
	get_chunk_number(dummy_data_set.size(), chunk_size, sample, total_chunk);

	std::cout << std::endl
	          << "Client to Server" << std::endl
	          << "----------------" << std::endl
	          << "Total Chunk                 : " << total_chunk << std::endl
	          << "Sample in each chunk   : " << sample << std::endl;

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
			request_.add_chunk_data_client_request(dummy_data_set[i]);
		}
		temp_count++;
		total_chunk--;

		stream->Write(request_);

		// message filed will be cleared after passing one chunk. It provides a fresh repeated field in the next iteration to pass a new chunk
		request_.clear_chunk_data_client_request();
	}

	stream->WritesDone();

	// Reading chunk response from server
	std::vector<int32_t> dummy_final_data_set;

	while (stream->Read(&response_))
	{
		for(int64_t i = 0; i < request_.chunk_data_length(); i++)
		{
			dummy_final_data_set.push_back(response_.chunk_data_server_response(i));
		}
	}

	std::cout << "dummy_final_data_set size " << request_.chunk_data_client_request_size() << std::endl;

	grpc::Status status = stream->Finish();

	if (status.ok()){
		std::cout << request_.name() << " is transmitted from Server" << std::endl;
		std::cout << std::endl
		          << "Server has responsed OK and stream/large data from server is stored in a vector in Client side" << std::endl
		          << "dummy_final_data_set size              : " << dummy_final_data_set.size() << std::endl
		          << "Sum of sample of dummy_final_data_set  : " << std::accumulate(dummy_final_data_set.begin(), dummy_final_data_set.end(), 0ULL) << std::endl
		          << std::endl;
	}

	else
		std::cout << "!!!!! Server is Failed to Stream !!!!!" << std::endl;
}