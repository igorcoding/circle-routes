#ifndef _CIRCLE_ROUTES_MAIN_H_
#define _CIRCLE_ROUTES_MAIN_H_

#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include <Poco/Util/Application.h>
//#include "RootHandler.h"
//#include "DataHandler.h"

namespace croutes_server {
    class RootHandler: public Poco::Net::HTTPRequestHandler
    {
    public:
        void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) {
            Poco::Util::Application& app = Poco::Util::Application::instance();
            app.logger().information("Request from " + request.clientAddress().toString());

            response.setChunkedTransferEncoding(true);
            response.setContentType("text/html");
            std::ostream& ostr = response.send();
            ostr << "<html><head><title>HTTP Server powered by POCO C++ Libraries</title></head>";
            ostr << "<body>";
            ostr << "</body></html>";
        }
    };

    class RequestHandlerFactory : public Poco::Net::HTTPRequestHandlerFactory {
    public:
        RequestHandlerFactory() {
        }

        Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest& request) {
//            if (request.getURI() == "/")
                return new RootHandler();
//            else
//                return new DataHandler();
        }
    };

}

#endif //_CIRCLE_ROUTES_MAIN_H_
