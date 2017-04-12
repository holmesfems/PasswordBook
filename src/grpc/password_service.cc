/*
 * password_service.cc
 * Copyright (C) 2017 Jerry Jia <jerryjiahaha@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include <iostream>
#include <memory>

#include <grpc/grpc.h>
#include <grpc++/server.h>
#include <grpc++/server_builder.h>
#include <grpc++/server_context.h>
#include <grpc++/security/server_credentials.h>

#include "password_service.grpc.pb.h"

#include "../cmdSeparator.h"
#include "../passwordManager.h"
#include "../passwordGenerator.h"
#include "iniParser.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerReader;
using grpc::ServerReaderWriter;
using grpc::ServerWriter;
using grpc::Status;

using passwordservice::PasswordService;
using passwordservice::Empty;
using passwordservice::Item;

class PasswordServiceImpl final: public PasswordService::Service {
    public:
        explicit PasswordServiceImpl(int argc, char *argv[]) {

            std::string configName = (argc > 1 ? argv[1] : "password_service.ini");
            config = iniParser(configName.c_str(), "password_service");

            _pwdManager = PasswordManager::FactoryCreateManager();
            _pwdManager->openDB(config.get("dbpath"));

            // _pwdGenerator = new PasswordGenerator();
        }
        virtual ~PasswordServiceImpl() {
            delete _pwdManager;
        }
        Status ListItems(ServerContext *context, const Empty *nil, ServerWriter<Item> *writer) override {
            (void)context, nil;

            std::vector<std::string> lists = _pwdManager->getDomainList();
            for (int count = 0; count < lists.size(); count++ ) {
                Item item;
                item.set_domain(lists[count]);
                item.set_id(count);
                writer->Write(item);
            }

            return Status::OK;
        }
    private:
        iniParser config;
        PasswordManager::PasswordManager *_pwdManager = nullptr;
        // PasswordGenerator *_pwdGenerator;
};

int main(int argc, char *argv[]) {
    std::string server_address("127.0.0.1:23333");
    PasswordServiceImpl service(argc, argv);

    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "server listening on " << server_address << std::endl;
    server->Wait();

    return 0;
}

/* vim: set et: */
