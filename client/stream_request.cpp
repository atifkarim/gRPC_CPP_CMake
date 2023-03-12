#include <ctime>

/*
How many integer in 1GB?
Probable Answer:

32 bit comprises 1 integer
So, convert 1GB to bit and devide by 32
1 GB = 1*1024*1024*1024*8 = 8589934592 bits
so, total integer = (1*1024*1024*1024*8) / 32 = 268435456

*/

// This function prepare necessary parameter to send Client request to the Server
void big_data_request(demo_grpc::C_Request &request_)
{
	srand((unsigned) time(nullptr));

	// std::vector<int32_t> billion_data;
	// int64_t vector_size = 2ULL*1024*1024*1024;
	// int64_t vector_size = 1610612736;
	// int64_t vector_size = 2684354560;

	for(int64_t i = 0; i < 1024*1024; i++)
	{
		request_.mutable_client_data_collection()->add_data_collection(5);
		// request_.mutable_client_data_collection()->add_data_collection(rand() % 19 + (-9));
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