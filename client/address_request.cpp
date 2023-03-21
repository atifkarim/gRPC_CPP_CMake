#include "client_zone.h"

// This function prepare address related param (eg: user name) to transfer from Client to the Server
void grpc_client::Address_Request()
{
	demo_grpc::C_Request request_;
	demo_grpc::S_Response response_;

	std::string name_;
	std::cout << "Provide User Name : ";
	std::cin >> name_;

	request_.set_name(name_);
	request_.set_area_field(detected_task);
	request_.set_choose_area(button);

	grpc::ClientContext context;
	grpc::Status status = stub->GetAddress(&context, request_, &response_);

	if (status.ok())
	{
		// Output result
		std::cout << "Data regarding the user " << request_.name() << " is received from Server" << std::endl;
		std::cout << "Name:              " << response_.name() << std::endl;
		std::cout << "City:              " << response_.city() << std::endl;
		std::cout << "Zip:               " << response_.zip() << std::endl;
		std::cout << "Street:            " << response_.street() << std::endl;
		std::cout << "Country:           " << response_.country() << std::endl;
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
}
