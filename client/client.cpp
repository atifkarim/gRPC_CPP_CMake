#include <big_data.pb.h>
#include <addressbook.grpc.pb.h>

#include <grpcpp/grpcpp.h>
#include <grpcpp/create_channel.h>

#include "address_request.h"
#include "stream_request.h"
#include "random_function.h"

#include <iostream>
#include <numeric>
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

// following function make large vector and then chunk to send via stream from client to server
void Stream_Data_Chunk_Request(demo_grpc::Large_Data_Request &request_,
                               demo_grpc::Large_Data_Response &response_,
                               uint64_t preferred_chunk_size_in_kibyte)
{
	std::vector<int32_t> large_vector;
	std::cout << "Creating a Vector in Client" << std::endl;
	for(int64_t i = 0; i < 1024 * 10; i++)
	{
		large_vector.push_back(1);
		// request_.mutable_client_dummy_stream_data()->add_stream_data_pack(1);
	}
	std::cout << "Finished vector creation in Client" << std::endl;
	std::cout << "sum_of_elems large_vector: " << std::accumulate(large_vector.begin(), large_vector.end(), 0) << std::endl;

	uint64_t preferred_chunk_size_in_kibyte_holds_integer_num = 0;
	uint32_t total_chunk = total_chunk_counter(large_vector.size(), preferred_chunk_size_in_kibyte, preferred_chunk_size_in_kibyte_holds_integer_num);
	std::cout << "Client : total_chunk: " << total_chunk << std::endl;
	std::cout << "Client : Each chunk holds: " << preferred_chunk_size_in_kibyte_holds_integer_num << " number of integer" << std::endl;

	std::string ip_address = "localhost:50051";
	auto channel = grpc::CreateChannel(ip_address, grpc::InsecureChannelCredentials());
	std::unique_ptr<demo_grpc::AddressBook::Stub> stub = demo_grpc::AddressBook::NewStub(channel);
	grpc::ClientContext context;
	std::shared_ptr<::grpc::ClientReaderWriter< ::demo_grpc::Large_Data_Request, ::demo_grpc::Large_Data_Response> > stream(stub->Stream_Chunk_Service(&context));
	// grpc::Status status = stub->Stream_Chunk_Service(&context);

	int32_t temp_count = 0;
	while(total_chunk > 0)
	{
		for (int64_t i = temp_count * preferred_chunk_size_in_kibyte_holds_integer_num; i < preferred_chunk_size_in_kibyte_holds_integer_num + temp_count * preferred_chunk_size_in_kibyte_holds_integer_num; i++)
		{
			request_.add_large_data_collection_request(large_vector[i]);
		}
		temp_count++;
		total_chunk--;

		stream->Write(request_);

		request_.clear_large_data_collection_request();
	}

	stream->WritesDone();

	std::cout << "Client make a stream data of size " << request_.large_data_collection_request_size() << std::endl;

	grpc::Status status = stream->Finish();

	if (status.ok())
		std::cout << "Server is GOOD !!! and sent response_size: " << response_.response_size() << " which is dummy" <<std::endl;
	else
		std::cout << "Server is in trouble" << std::endl;
}

int main(int argc, char* argv[])
{
	std::string client_address = "localhost:50051";
	std::cout << "Address of client: " << client_address << std::endl;

	// Setup request
	demo_grpc::C_Request query;
	demo_grpc::S_Response result;

	Client_Request(query);

	demo_grpc::Large_Data_Request stream_chunk_request_;
	demo_grpc::Large_Data_Response stream_chunk_response_;

	uint64_t preferred_chunk_size_in_kibyte = 64;
	Stream_Data_Chunk_Request(stream_chunk_request_, stream_chunk_response_, preferred_chunk_size_in_kibyte);

	// ssl_tsl try from gRPC source code
	/*
	constexpr char kCaCertPath[] = "/home/atif/grpc/src/core/tsi/test_creds/ca.pem";
	grpc::SslCredentialsOptions ssl_opts;
	ssl_opts.pem_root_certs = ReadFile(kCaCertPath);
	grpc::ChannelArguments args;
	args.SetString(GRPC_SSL_TARGET_NAME_OVERRIDE_ARG, "foo.test.google.fr");
	*/

	// Call
	auto channel = grpc::CreateChannel(client_address, grpc::InsecureChannelCredentials());

	// following code to set maximum limit from client side

	// auto cargs = grpc::ChannelArguments();
	// cargs.SetMaxReceiveMessageSize(3ULL * 1024 * 1024 * 1024); // 1 GB
	// cargs.SetMaxSendMessageSize(3ULL * 1024 * 1024 * 1024);

	// following found here:
	/*
	cargs.SetMaxReceiveMessageSize(-1); // 1 GB
	cargs.SetMaxSendMessageSize(-1);
	auto channel = grpc::CreateCustomChannel(client_address, grpc::InsecureChannelCredentials(), cargs);
	*/


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
