#include <big_data.pb.h>
#include <addressbook.grpc.pb.h>

#include <grpcpp/grpcpp.h>
#include <grpcpp/create_channel.h>

#include "address_request.h"
#include "stream_request.h"
#include "random_function.h"

#include <iostream>
using namespace std;

// This function prompts the user to set value for the required area
void Client_Request(demo_grpc::C_Request &request_)
{
	unsigned int choose_area_ ;
	std::cout << "Choose Desired Area" << std::endl
	          << "-------------------" << std::endl
	          << "Address         : 0" << std::endl
	          << "Check Value     : 1" << std::endl
	          << "Stream Data     : 2" << std::endl;

	std::cin >> choose_area_;
	request_.set_choose_area(choose_area_);

	switch(request_.choose_area())
	{
		case 0: address_request(request_); break;
		case 1: check_value_request(request_); break;
		case 2: big_data_request(request_); break;
		default: throw std::runtime_error("No Service is added against this Wish"); break;
	}
}

// According to Client Request this function display the value of protobuf message
void Server_Response(demo_grpc::C_Request &request_, const demo_grpc::S_Response &response_)
{
	switch(request_.choose_area())
	{
		case 0: address_response(request_, response_); break;
		case 1: check_value_response(request_, response_); break;
		case 2: big_data_response(request_, response_); break;
	}
}

int main(int argc, char* argv[])
{
	std::string client_address = "localhost:50051";
	std::cout << "Address of client: " << client_address << std::endl;

	// Setup request
	demo_grpc::C_Request query;
	demo_grpc::S_Response result;

	Client_Request(query);

	// ssl_tsl try from gRPC source code
	/*
	constexpr char kCaCertPath[] = "/home/atif/grpc/src/core/tsi/test_creds/ca.pem";
	grpc::SslCredentialsOptions ssl_opts;
	ssl_opts.pem_root_certs = ReadFile(kCaCertPath);
	grpc::ChannelArguments args;
	args.SetString(GRPC_SSL_TARGET_NAME_OVERRIDE_ARG, "foo.test.google.fr");
	*/

	// Call
	// auto channel = grpc::CreateChannel(client_address, grpc::InsecureChannelCredentials());

	// following code to set maximum limit from client side

	auto cargs = grpc::ChannelArguments();
	// cargs.SetMaxReceiveMessageSize(3ULL * 1024 * 1024 * 1024); // 1 GB
	// cargs.SetMaxSendMessageSize(3ULL * 1024 * 1024 * 1024);

	// following found here:
	cargs.SetMaxReceiveMessageSize(-1); // 1 GB
	cargs.SetMaxSendMessageSize(-1);
	auto channel = grpc::CreateCustomChannel(client_address, grpc::InsecureChannelCredentials(), cargs);


	// following code is for gRPC ssl tsl code
	// auto channel = grpc::CreateCustomChannel(client_address, grpc::SslCredentials(ssl_opts), args);

	std::unique_ptr<demo_grpc::AddressBook::Stub> stub = demo_grpc::AddressBook::NewStub(channel);
	grpc::ClientContext context;
	grpc::Status status = stub->GetAddress(&context, query, &result);

	if (status.ok())
	{
		Server_Response(query, result);
	}
	else
	{
		if(status.error_code() == grpc::StatusCode::INVALID_ARGUMENT)
		{
			std::cout << status.error_message() << std::endl;
		}
		else
		{
			std::cout << status.error_message() << std::endl;
		}
	}

	return 0;
}
