#include "client_zone.h"

void grpc_client::check_value_request()
{
	demo_grpc::C_Request request_;
	demo_grpc::S_Response response_;

	unsigned int value_;
	std::cout << "Provide a valid value less than or equal to 5 : ";
	std::cin >> value_;

	request_.set_init_val(value_);
	request_.set_area_field(detected_area_name);
    request_.set_choose_area(button);

	grpc::ClientContext context;
	grpc::Status status = stub->GetAddress(&context, request_, &response_);

	if (status.ok())
	{
		// Output result
		std::cout << "Value of " << request_.init_val() << " is received from Server" << std::endl;
		std::cout << "Doubled Value:     " << response_.double_init_val() << std::endl;
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