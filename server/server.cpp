#include <myproto/address.pb.h>
#include <myproto/addressbook.grpc.pb.h>

#include <grpc/grpc.h>
#include <grpcpp/server_builder.h>

#include <iostream>
using namespace std;

class AddressBookService final : public demo_grpc::AddressBook::Service {
    public:
        virtual ::grpc::Status GetAddress(::grpc::ServerContext* context, const ::demo_grpc::C_Request* request, ::demo_grpc::S_Response* response)
        {
            std::cout << "Server: GetAddress for \"" << request->name() << "\"." << std::endl;

            if (request->cl_x() < 5)
                cout << "request->cl_x(): " << request->cl_x() << endl;
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
    grpc::ServerBuilder builder;
    builder.AddListeningPort("0.0.0.0:50051", grpc::InsecureServerCredentials());

    AddressBookService my_service;
    builder.RegisterService(&my_service);

    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    server->Wait();

    return 0;
}
