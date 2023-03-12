#include <iostream>

// This function prepare data block to initialize and set value for corresponding protobuf message
void set_stream_value(const ::demo_grpc::C_Request* request, ::demo_grpc::S_Response* response)
{
	std::cout << "**********############################### Stream Data is in Server" << std::endl;
	std::cout << "Client has sent : " << request->client_data_collection().index() << std::endl;
	response->mutable_server_data_collection()->set_index(78);

	if (request->client_data_collection().data_collection_size() == 0)
		throw std::runtime_error("Vector Container is Empty");

	// int64_t vector_size = 2ULL*1024*1024*1024;
	int64_t vector_size = 1610612736;
	// int64_t vector_size = 2684354560;
	for(int64_t i = 0; i < 1024*1024; i++)
	{
		// std::cout << "val : " << request->client_data_collection().data_collection(i) << std::endl;
		response->mutable_server_data_collection()->add_data_collection(request->client_data_collection().data_collection(i) * 2);
	}

	std::cout << "Server is prepared stream data of size " << response->server_data_collection().data_collection_size() << std::endl;

	// for (auto x : response->server_data_collection().data_collection())
	// {
	// 	std::cout << "val : " << x << std::endl;
	// }

}
