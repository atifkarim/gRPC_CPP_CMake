#include <string>
#include <sstream>

// This function prepare address related param (eg: user name) to transfer from Client to the Server
void address_request(demo_grpc::C_Request &request_)
{
	std::string name_;
	std::cout << "Provide User Name : ";
	std::cin >> name_;

	request_.set_name(name_);
}

void check_value_request(demo_grpc::C_Request &request_)
{
	unsigned int value_;
	std::cout << "Provide a valid value less than or equal to 5 : ";
	std::cin >> value_;

	request_.set_init_val(value_);
}

// Server response prints in the console
void address_response(demo_grpc::C_Request &request_, const demo_grpc::S_Response &response_)
{
	// Output result
	std::cout << "Data regarding the user " << request_.name() << " is received from Server" << std::endl;

	std::cout << "Name:              " << response_.name() << std::endl;
	std::cout << "City:              " << response_.city() << std::endl;
	std::cout << "Zip:               " << response_.zip() << std::endl;
	std::cout << "Street:            " << response_.street() << std::endl;
	std::cout << "Country:           " << response_.country() << std::endl;
}

void check_value_response(demo_grpc::C_Request &request_, const demo_grpc::S_Response &response_)
{
	// Output result
	std::cout << "Value of " << request_.init_val() << " is received from Server" << std::endl;

	std::cout << "Doubled Value:     " << response_.double_init_val() << std::endl;
}