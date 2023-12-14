#include "grpc-lib.hpp"

namespace GRPCLib
{
    /**
     * The function `RunServer()` starts a gRPC server on address "0.0.0.0:50051" and waits for incoming
     * requests.
     */
    void RunServer()
    {
        std::string server_address("127.0.0.1:50051");
        OHLCServiceImpl service;

        grpc::ServerBuilder builder;
        builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
        builder.RegisterService(&service);
        std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
        server->Wait();
    }

    void TestServer(const vector_message_list list)
    {
        std::shared_ptr<grpc::Channel> channel = grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials());
        std::unique_ptr<OHLCService::Stub> stub = OHLCService::NewStub(channel);

        google::protobuf::Empty reply;
        grpc::ClientContext context;

        grpc::Status status = stub->SendMyVectorList(&context, list, &reply);
        std::cout << "status" << std::endl;
        if (status.ok())
        {
            std::cout << "RPC Call successful." << std::endl;
        }
        else
        {
            std::cerr << "RPC failed with status: " << status.error_code() << ": " << status.error_message() << std::endl;
        }
    }

} // namespace GRPCLib
