#include <address.pb.h>
#include <addressbook.grpc.pb.h>

#include <grpcpp/grpcpp.h>
#include <grpcpp/create_channel.h>

#include <iostream>
using namespace std;

int main(int argc, char* argv[])
{
	// Setup request
	demo_grpc::C_Request query;
	demo_grpc::S_Response result;
	query.set_name("John");

	int32_t x;
	cout << "Provide the value of cl_x: ";
	cin >> x;
	query.set_cl_x(x);

	// Call
	auto channel = grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials());
	std::unique_ptr<demo_grpc::AddressBook::Stub> stub = demo_grpc::AddressBook::NewStub(channel);
	grpc::ClientContext context;
	grpc::Status status = stub->GetAddress(&context, query, &result);

	if (status.ok()) {
		// Output result
		std::cout << "I got:" << std::endl;
		std::cout << "Name: " << result.name() << std::endl;
		std::cout << "City: " << result.city() << std::endl;
		std::cout << "Zip:  " << result.zip() << std::endl;
		std::cout << "Street: " << result.street() << std::endl;
		std::cout << "Country: " << result.country() << std::endl;
		return 0;
	} else {
	  // ouch!
	  // lets print the gRPC error message
	  // which is "Length of `Name` cannot be more than 10 characters"
	  std::cout << status.error_message() << std::endl;
	  // lets print the error code, which is 3
	//   std::cout << status.error_code() << std::endl;
	  // want to do some specific action based on the error?
	//   if(status.error_code() == StatusCode::INVALID_ARGUMENT) {
	//     // do your thing here
	//   }
	  return 0;
	}
}
