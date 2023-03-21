#include "client_zone.h"
#include "random_function.h"

// This function prompts the user to set value for the required area
void Client_Request(std::shared_ptr<grpc::Channel> channel)
{
	unsigned int task_number ;
	std::cout << "Choose task number" << std::endl
	          << "------------------" << std::endl
	          << "Address         : 0" << std::endl
	          << "Check Value     : 1" << std::endl
	          << "Stream Data     : 2" << std::endl;

	std::cin >> task_number;

	switch(task_number)
	{
		case 0: {grpc_client address_req(channel, task_number); address_req.address_request(); break;}
		case 1: {grpc_client value_req(channel, task_number); value_req.Double_Value_Request(); break;}
		case 2: {grpc_client stream_chunk_req(channel, task_number); stream_chunk_req.data_chunk_stream_request(); break;}
		default: throw std::runtime_error("No Service is added against this Wish"); break;
	}
}

int main(int argc, char* argv[])
{
	std::string client_address = "localhost:50051";
	std::cout << "Address of client: " << client_address << std::endl;

	// Call
	auto channel = grpc::CreateChannel(client_address, grpc::InsecureChannelCredentials());
	Client_Request(channel);

	return 0;
}
