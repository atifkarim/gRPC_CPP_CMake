#include <myproto/big_data.pb.h>
#include <myproto/addressbook.grpc.pb.h>

#include <grpcpp/grpcpp.h>
#include <grpcpp/server_builder.h>

#include "address_response.h"
#include "stream_response.h"
#include "random_function.h"

#include <iostream>
#include <numeric>

class AddressBookService final : public demo_grpc::AddressBook::Service {
	public:
		virtual ::grpc::Status GetAddress(::grpc::ServerContext* context, const ::demo_grpc::C_Request* request, ::demo_grpc::S_Response* response)
		{
			switch (request->choose_area())
			{
			case 0:
				try
				{
					std::cout << "****************** HEY I AM HERE , CASE " << request->choose_area() <<" !!!!!  ******************" << std::endl;
					Set_Value_Address(request, response);
				}
				catch(const std::exception& e)
				{
					std::cout << e.what() << std::endl;
					return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT, e.what());
				}
			break;

			case 1:
				try
				{
					std::cout << "****************** HEY I AM HERE , CASE " << request->choose_area() <<" !!!!!  ******************" << std::endl;
					Set_Check_Value(request, response);
				}
				catch(const std::exception& e)
				{
					std::cout << e.what() << std::endl;
					return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT, e.what());
				}
			break;

			case 2:
				try
				{
					std::cout << "****************** HEY I AM HERE , CASE " << request->choose_area() <<" !!!!!  ******************" << std::endl;
					set_stream_value(request, response);
				}
				catch(const std::exception& e)
				{
					std::cout << e.what() << std::endl;
					return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT, e.what());
				}
			break;
			}

			std::cout << "Information of " << request->choose_area() << " is sent to Client" << std::endl;
			return grpc::Status::OK;
		}

	virtual ::grpc::Status Stream_Chunk_Service(::grpc::ServerContext* context,
	                                            ::grpc::ServerReaderWriter< ::demo_grpc::Large_Data_Response, ::demo_grpc::Large_Data_Request>* stream)
	{
		std::cout << "Server Side Stream Service has executed" << std::endl;

		std::vector<int32_t> server_dummy_data_set;

		demo_grpc::Large_Data_Request request;
		demo_grpc::Large_Data_Response response;

		while (stream->Read(&request)){
			for(int64_t i = 0; i < request.chunk_data_length(); i++){
				server_dummy_data_set.push_back(request.chunk_data_client_request(i));
			}
		}

		if(server_dummy_data_set.size() == request.client_data_stream_size()){
			std::cout << std::endl
			          << "Steam Data has transmitted rom Client to Server. All are stored in server_dummy_data_set" << std::endl
			          << "server_dummy_data_set size              : " << server_dummy_data_set.size() << std::endl
			          << "Sum of sample of  server_dummy_data_set : " << std::accumulate(server_dummy_data_set.begin(), server_dummy_data_set.end(), 0ULL) << std::endl;
		}

		std::cout << std::endl
		          << "Information from Client" << std::endl
		          << "-----------------------------------" << std::endl
		          << "Data Sample carried by chunk  : " << request.client_data_stream_size() << std::endl
		          << "Required chunks               : " << request.required_chunk() << std::endl
		          << "Data Sample for every chunk   : " << request.chunk_data_length() << std::endl;

		int64_t track_index = 0; // Tracks the current index of the server_dummy_data_set

		// Server starts streaming the data chunk by chunk to the client. Here, server is doing a dummy operation on clients data.
		// Here, it multiplies by 2
		for (int64_t i = 0; i < request.required_chunk(); i++){
			for(int64_t j = track_index * request.chunk_data_length(); j < request.chunk_data_length() + track_index * request.chunk_data_length(); j++){
				response.add_chunk_data_server_response(server_dummy_data_set[j] * 2);
			}
			track_index++;
			stream->Write(response);

			// message filed will be cleared after passing one chunk. It provides a fresh repeated field in the next iteration to pass a new chunk
			response.clear_chunk_data_server_response();
		}

		std::cout << "Streaming Job is done from Server Side" << std::endl;
		return grpc::Status::OK;
	}
};

void RunServer()
{
	std::cout << "grpc Version: " << grpc::Version() << std::endl;
	std::string server_address = "localhost:50051";
	std::cout << "Address of server: " << server_address << std::endl;

	grpc::ServerBuilder builder;
	builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());

	AddressBookService my_service;
	builder.RegisterService(&my_service);

	std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
	server->Wait();
}

int main(int argc, char* argv[])
{
	RunServer();
	return 0;
}
