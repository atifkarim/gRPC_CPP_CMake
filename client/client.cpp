#include <address.pb.h>
#include <addressbook.grpc.pb.h>

#include <grpcpp/grpcpp.h>
#include <grpcpp/create_channel.h>

#include "random_function.h"

#include <iostream>
using namespace std;

int main(int argc, char* argv[])
{
	std::string client_address = "localhost:50051";
	std::cout << "Address of client: " << client_address << std::endl;

	// Setup request
	demo_grpc::C_Request query;
	demo_grpc::S_Response result;

	// Value is statically set here
	query.set_name("John");

	// Value to check the error handler part.
	int32_t x;
	cout << "Requesting init_val: ";
	cin >> x;
	query.set_init_val(x);

	// ssl_tsl try from gRPC source code
	constexpr char kCaCertPath[] = "/home/atif/grpc/src/core/tsi/test_creds/ca.pem";
	grpc::SslCredentialsOptions ssl_opts;
	ssl_opts.pem_root_certs = ReadFile(kCaCertPath);
	grpc::ChannelArguments args;
	args.SetString(GRPC_SSL_TARGET_NAME_OVERRIDE_ARG, "foo.test.google.fr");

	// Call
	// auto channel = grpc::CreateChannel(client_address, grpc::InsecureChannelCredentials());
	auto channel = grpc::CreateCustomChannel(client_address, grpc::SslCredentials(ssl_opts), args);

	std::unique_ptr<demo_grpc::AddressBook::Stub> stub = demo_grpc::AddressBook::NewStub(channel);
	grpc::ClientContext context;
	grpc::Status status = stub->GetAddress(&context, query, &result);

	if (status.ok()) {
		// Output result
		std::cout << "Response from Server:" << std::endl;
		std::cout << "Name:              " << result.name() << std::endl;
		std::cout << "City:              " << result.city() << std::endl;
		std::cout << "Zip:               " << result.zip() << std::endl;
		std::cout << "Street:            " << result.street() << std::endl;
		std::cout << "Country:           " << result.country() << std::endl;
		std::cout << "Doubled Init Val:  " << result.double_init_val() << std::endl;
		return 0;
	}
	else
	{
		// lets print the gRPC error message
		// std::cout << status.error_message() << std::endl;

		// lets print the error code, which is 3
		// std::cout << status.error_code() << std::endl;

		// want to do some specific action based on the error?
		if(status.error_code() == grpc::StatusCode::INVALID_ARGUMENT)
		{
			std::cout << status.error_message() << std::endl;
		}

	return 0;
	}
}
