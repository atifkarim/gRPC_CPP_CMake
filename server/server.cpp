#include <myproto/address.pb.h>
#include <myproto/addressbook.grpc.pb.h>

#include <grpcpp/grpcpp.h>
#include <grpcpp/server_builder.h>

#include <iostream>

void Check_Value(const ::demo_grpc::C_Request* request)
{
	if (request->cl_x() < 5)
		std::cout << "Check_Value request->cl_x(): " << request->cl_x() << std::endl;
	else
		throw std::runtime_error("********** BAD VALUE **********");
}

class AddressBookService final : public demo_grpc::AddressBook::Service {
	public:
		virtual ::grpc::Status GetAddress(::grpc::ServerContext* context, const ::demo_grpc::C_Request* request, ::demo_grpc::S_Response* response)
		{
			std::cout << "Server: GetAddress for \"" << request->name() << "\"." << std::endl;

			if (request->cl_x() < 5)
				std::cout << "request->cl_x(): " << request->cl_x() << std::endl;
			else
				return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT, "PLEASE set cl_x value less than 5 !!!!!!!!!!");

			response->set_name("Peter Peterson");
			response->set_zip("12345");
			response->set_country("Superland");

			return grpc::Status::OK;
		}
};

int main(int argc, char* argv[])
{
	std::cout << "grpc Version: " << grpc::Version() << std::endl;
	grpc::ServerBuilder builder;
	builder.AddListeningPort("0.0.0.0:50051", grpc::InsecureServerCredentials());

	AddressBookService my_service;
	builder.RegisterService(&my_service);

	std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
	server->Wait();

	return 0;
}
