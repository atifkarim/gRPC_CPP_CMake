/**
 * @brief This function sets the value of protobuf message from Server side
 *
 * @param request
 * @param response
 */
void Set_Value_Address(const ::demo_grpc::C_Request* request, ::demo_grpc::S_Response* response)
{
	response->set_name("Peter Peterson");
	response->set_zip("12345");
	response->set_country("Superland");
}
