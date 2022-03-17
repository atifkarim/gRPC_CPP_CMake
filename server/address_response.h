// This function sets the value of protobuf message from Server side
void Set_Value_Address(const ::demo_grpc::C_Request* request, ::demo_grpc::S_Response* response)
{
	response->set_name("Peter Peterson");
	response->set_zip("12345");
	response->set_country("Superland");
}

void Set_Check_Value(const ::demo_grpc::C_Request* request, ::demo_grpc::S_Response* response)
{
	if (request->init_val() < 5)
		std::cout << "Client is requested for value: " << request->init_val() << std::endl;
	else
		throw std::runtime_error("Please chose less than 5");

	response->set_double_init_val(request->init_val() * 2);
}