//
// Created by igor on 23/03/15.
//

#include "server.h"

#include <iostream>
#include <Poco/Net/HTTPServer.h>
#include <Poco/Util/ServerApplication.h>

class CircleNetApplication : public Poco::Util::ServerApplication {

};

namespace croutes_server {
    class CircleNetApplication : public Poco::Util::ServerApplication {
    protected:
        int main(const std::vector<std::string>&) {
            using namespace Poco::Net;

            Poco::Util::Application& app = Poco::Util::Application::instance();

            Poco::UInt16 port = 8889;
            HTTPServerParams* pParams = new HTTPServerParams;
            pParams->setMaxQueued(100);
            pParams->setMaxThreads(4);
            ServerSocket svs(port); // set-up a server socket
            HTTPServer srv(new RequestHandlerFactory(), svs, pParams);

            srv.start();
            app.logger().information("Server started on port " + std::to_string(port));

            waitForTerminationRequest();

            srv.stop();
            app.logger().information("Server stopped");

            return Application::EXIT_OK;
        }
    };
}


int main(int argc, char** argv) {
    croutes_server::CircleNetApplication app;
    return app.run(argc, argv);
}
