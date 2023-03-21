#include "client_zone.h"

void grpc_client::Double_Value_Request()
{
	demo_grpc::Number_Request request_;
	demo_grpc::Number_Response response_;

	float value_;
	std::cout << "Provide a positive value less than or equal to 5 : ";
	std::cin >> value_;

	request_.set_init_val(value_);
	request_.set_name(detected_task);

	grpc::ClientContext context;
	grpc::Status status = stub->ProcessNumber(&context, request_, &response_);

	if (status.ok())
	{
		// Output result
		std::cout << "Result of " << request_.name() << " is available now" << std::endl;
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
