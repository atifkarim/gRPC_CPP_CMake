#include <myproto/address.pb.h>
#include <myproto/addressbook.grpc.pb.h>

#include <grpcpp/grpcpp.h>
#include <grpcpp/server_builder.h>

#include <iostream>

void Check_Value(const ::demo_grpc::C_Request* request)
{
	if (request->init_val() < 5)
		std::cout << "Client is requested for value: " << request->init_val() << std::endl;
	else
		throw std::runtime_error("Please chose less than 5");
}

class AddressBookService final : public demo_grpc::AddressBook::Service {
	public:
		virtual ::grpc::Status GetAddress(::grpc::ServerContext* context, const ::demo_grpc::C_Request* request, ::demo_grpc::S_Response* response)
		{
			std::cout << "Server: GetAddress for \"" << request->name() << "\"." << std::endl;

			// if (request->init_val() < 5)
			// 	std::cout << "request->init_val(): " << request->init_val() << std::endl;
			// else
			// 	return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT, "PLEASE set init_val value less than 5 !!!!!!!!!!");

			try
			{
				Check_Value(request);
			}
			catch(const std::exception& e)
			{
				std::cout << e.what() << std::endl;
				return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT, e.what());
			}

			response->set_name("Peter Peterson");
			response->set_zip("12345");
			response->set_country("Superland");

			return grpc::Status::OK;
		}
};

void RunServer()
{
	std::cout << "grpc Version: " << grpc::Version() << std::endl;
	std::string server_address = "localhost:50051";
	std::cout << "Address of server: " << server_address << std::endl;
	grpc::ServerBuilder builder;
	builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());

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
