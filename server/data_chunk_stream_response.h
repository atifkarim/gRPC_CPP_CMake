/**
 * @file data_chunk_stream_response.h
 * @author MD Atif Bin Karim
 * @brief This is the service response algorithm from server to pass stream data using chunk
 * @date 21-03-2023
 */

#include <numeric>

void Data_Stream_Response(::grpc::ServerReaderWriter< ::demo_grpc::Large_Data_Response, ::demo_grpc::Large_Data_Request>* stream)
{
	// Instance of request and response from client & server respectively
	demo_grpc::Large_Data_Request request;
	demo_grpc::Large_Data_Response response;

	// This vector will hold all data which will be transferred from client platform
	std::vector<int32_t> server_dummy_data_set;

	// This variable will track what is the size of the data which is passed from client
	uint32_t server_track_client_data_size_before_reading = 0;

	// Read operation
	while (stream->Read(&request))
	{
		// Iterate till the chunk data length is reached
		for(int64_t i = 0; i < request.chunk_data_length(); i++)
		{
			// This condition checks the iteration number with the vector data size of the client.
			// if client data size has reached then reading will be stopped
			if(server_track_client_data_size_before_reading < request.client_data_stream_size())
			{
				server_dummy_data_set.push_back(request.chunk_data_client_request(i));
				server_track_client_data_size_before_reading++;
			}
			else
			{
				break;
			}
		}
	}

	std::cout << std::endl;

	if(server_dummy_data_set.size() == request.client_data_stream_size())
	{
		std::cout << "Prepared data in server is identical to the data size transferred by client" << std::endl;
	}
	else
	{
		throw std::runtime_error("Caution !!! Data sample is mismatched");
	}

	// set proto message for server side
	response.set_server_data_stream_size(server_dummy_data_set.size());

	std::cout << std::endl
	          << "Information of Server platform" << std::endl
	          << "******************************" << std::endl
	          << "server_dummy_data_set.size(): " << server_dummy_data_set.size() << std::endl
	          << "Sum of server_dummy_data_set : " << std::accumulate(server_dummy_data_set.begin(), server_dummy_data_set.end(), 0ULL) << std::endl
	          << std::endl
	          << "Information of Client platform" << std::endl
	          << "******************************" << std::endl
	          << "Data size transferred by client  : " << request.client_data_stream_size() << std::endl
	          << "Number of required chunk         : " << request.required_chunk() << std::endl
	          << "Number of data in each chunk     : " << request.chunk_data_length() << std::endl;

	int64_t track_index = 0; // Tracks the current index of the server_dummy_data_set

	// Server starts streaming the data chunk by chunk to the client. Here, server is doing a dummy operation on clients data.
	// Here, it multiplies by 2 each data of the client.

	// This variable will track client side data size during write back to client
	uint32_t server_track_client_data_size_before_writing = 0;
	for (int64_t i = 0; i < request.required_chunk(); i++)
	{
		for(int64_t j = track_index * request.chunk_data_length(); j < request.chunk_data_length() + track_index * request.chunk_data_length(); j++)
		{
			if(server_track_client_data_size_before_writing < request.client_data_stream_size())
			{
				response.add_chunk_data_server_response(server_dummy_data_set[j] * 2);
				server_track_client_data_size_before_writing++;
			}
			else
			{
				break;
			}
		}
		track_index++;
		stream->Write(response);

		// message field will be cleared after passing one chunk. It provides a fresh repeated field in the next iteration to pass a new chunk
		response.clear_chunk_data_server_response();
	}

	//  Prints name of the job which uis requested by client to process
	std::cout << request.name() << " Job is done from Server Side" << std::endl;

}
