#include <myproto/big_data.pb.h>
#include <myproto/addressbook.grpc.pb.h>
#include <myproto/collection.grpc.pb.h>

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
		std::cout << "******" << std::endl
		          << "Server" << std::endl
		          << "******" << std::endl;

		// This vector will hold all data which will be transferred from client platform
		std::vector<int32_t> server_dummy_data_set;

		demo_grpc::Large_Data_Request request;
		demo_grpc::Large_Data_Response response;

		// This variable will track what is the size of the data which is passed from client
		uint32_t server_track_client_data_size_before_reading = 0;

		while (stream->Read(&request)){
			for(int64_t i = 0; i < request.chunk_data_length(); i++)
			{
				// This condition checks the iteration number with the vector data size of the client.
				// if client data size has reached then reading will be stopped
				if(server_track_client_data_size_before_reading < request.client_data_stream_size()){
					server_dummy_data_set.push_back(request.chunk_data_client_request(i));
					server_track_client_data_size_before_reading++;
				}
				else{
				break;
			}
			}
		}
		std::cout << std::endl;

		std::cout << "server_dummy_data_set.size(): " << server_dummy_data_set.size() << std::endl;
		std::cout << "Sum of server_dummy_data_set : " << std::accumulate(server_dummy_data_set.begin(), server_dummy_data_set.end(), 0ULL) << std::endl;

		if(server_dummy_data_set.size() == request.client_data_stream_size())
		{
			std::cout << "Prepared data in server is identical to the data size transferred by client" << std::endl;
		}
		else
		{
			throw std::runtime_error("Caution !!! Data sample is mismatched");
		}

		// set proto message for server side
		response.set_server_data_stream_size(server_dummy_data_set.size());

		std::cout << std::endl
		          << "Information of Client platform" << std::endl
		          << "******************************" << std::endl
		          << "Data size transferred by client  : " << request.client_data_stream_size() << std::endl
		          << "Number of required chunk         : " << request.required_chunk() << std::endl
		          << "Number of data in each chunk     : " << request.chunk_data_length() << std::endl;

		int64_t track_index = 0; // Tracks the current index of the server_dummy_data_set

		// Server starts streaming the data chunk by chunk to the client. Here, server is doing a dummy operation on clients data.
		// Here, it multiplies by 2

		// This variable will track client side data size during write back to client
		uint32_t server_track_client_data_size_before_writing = 0;
		for (int64_t i = 0; i < request.required_chunk(); i++)
		{
			for(int64_t j = track_index * request.chunk_data_length(); j < request.chunk_data_length() + track_index * request.chunk_data_length(); j++)
			{
				if(server_track_client_data_size_before_writing < request.client_data_stream_size())
				{
					response.add_chunk_data_server_response(server_dummy_data_set[j] * 2);
					server_track_client_data_size_before_writing++;
				}
				else{
				break;
			}
			}
			track_index++;
			stream->Write(response);

			// message field will be cleared after passing one chunk. It provides a fresh repeated field in the next iteration to pass a new chunk
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
