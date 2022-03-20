// # include <vector>
#include <ctime>

// This function prepare necessary parameter to send Client request to the Server
void big_data_request(demo_grpc::C_Request &request_)
{
	srand((unsigned) time(nullptr));

	// std::vector<int32_t> billion_data;

	for(uint32_t i = 0; i < 268435456; i++)
	{
		request_.mutable_client_data_collection()->add_data_collection(rand() % 19 + (-9));
	}

	std::cout << "Stream Data is in Client" << std::endl;
	std::cout << "Client make a stream data of size " << request_.client_data_collection().data_collection_size() << std::endl;
	request_.mutable_client_data_collection()->set_index(58);

	// for (auto x : request_.client_data_collection().data_collection())
	// {
	// 	std::cout << "val : " << x << std::endl;
	// }
}

// This function display server response in the console
void big_data_response(demo_grpc::C_Request &request_, const demo_grpc::S_Response &response_)
{
	// Output result
	std::cout << "Status of Streaming Data is received from Server" << std::endl;
	std::cout << "Server has sent index data " << response_.server_data_collection().index() << std::endl;
	std::cout << "Server sent a stream data of size " << response_.server_data_collection().data_collection_size() << std::endl;
}