#include <myproto/big_data.pb.h>
#include <myproto/addressbook.grpc.pb.h>

#include <grpcpp/grpcpp.h>
#include <grpcpp/server_builder.h>

#include "address_response.h"
#include "stream_response.h"
#include "random_function.h"

#include <iostream>

class AddressBookService final : public demo_grpc::AddressBook::Service {
	public:
		virtual ::grpc::Status GetAddress(::grpc::ServerContext* context, const ::demo_grpc::C_Request* request, ::demo_grpc::S_Response* response)
		{
			switch (request->choose_area())
			{
			case 0:
				try
				{
					Set_Value_Address(request, response);
				}
				catch(const std::exception& e)
				{
					std::cout << e.what() << std::endl;
					return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT, e.what());
				}
			break;

			case 1:
				try
				{
					Set_Check_Value(request, response);
				}
				catch(const std::exception& e)
				{
					std::cout << e.what() << std::endl;
					return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT, e.what());
				}
			break;

			case 2:
				try
				{
					set_stream_value(request, response);
				}
				catch(const std::exception& e)
				{
					std::cout << e.what() << std::endl;
					return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT, e.what());
				}
			break;
			}

			std::cout << "Information of " << request->choose_area() << " is sent to Client" << std::endl;
			return grpc::Status::OK;
		}

	virtual ::grpc::Status Stream_Chunk_Service(::grpc::ServerContext* context, ::grpc::ServerReaderWriter< ::demo_grpc::Large_Data, ::demo_grpc::Large_Data>* stream)
	{
		// stream->Large_Data;
		return grpc::Status::OK;
	}
};

void RunServer()
{
	std::cout << "grpc Version: " << grpc::Version() << std::endl;
	std::string server_address = "localhost:50051";
	std::cout << "Address of server: " << server_address << std::endl;

	// ssl_tsl try from gRPC source code
	/*
	constexpr char kServerCertPath[] = "/home/atif/grpc/src/core/tsi/test_creds/server1.pem";
	constexpr char kServerKeyPath[]  = "/home/atif/grpc/src/core/tsi/test_creds/server1.key";
	constexpr char kCaCertPath[]     = "/home/atif/grpc/src/core/tsi/test_creds/ca.pem";
	grpc::SslServerCredentialsOptions ssl_opts;
	ssl_opts.pem_key_cert_pairs.push_back({ReadFile(kServerKeyPath), ReadFile(kServerCertPath)});
	ssl_opts.pem_root_certs =ReadFile(kCaCertPath);
	*/

	grpc::ServerBuilder builder;
	builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
	// builder.AddListeningPort(server_address, grpc::SslServerCredentials(ssl_opts));

	// follwoing for get and send max size message
	// builder.SetMaxMessageSize(3ULL * 1024 * 1024 * 1024);

	// builder.SetMaxReceiveMessageSize(3ULL * 1024 * 1024 * 1024);
	// builder.SetMaxSendMessageSize(3ULL * 1024 * 1024 * 1024);
	/*
	builder.SetMaxReceiveMessageSize(LONG_MAX);
	builder.SetMaxSendMessageSize(LONG_MAX);
	builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
	*/

	AddressBookService my_service;
	builder.RegisterService(&my_service);

	std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
	server->Wait();
}

int main(int argc, char* argv[])
{
	RunServer();
	return 0;
}
