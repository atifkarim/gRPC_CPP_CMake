void Double_Value(const ::demo_grpc::C_Request* request, ::demo_grpc::S_Response* response)
{
	if (request->init_val() >= 0 && request->init_val() < 5)
		std::cout << "Client is requested for value: " << request->init_val() << std::endl;
	else
		throw std::runtime_error("Please chose less than 5");

	response->set_double_init_val(request->init_val() * 2.0);
}
