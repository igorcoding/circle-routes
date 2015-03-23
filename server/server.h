#ifndef _CIRCLE_ROUTES_MAIN_H_
#define _CIRCLE_ROUTES_MAIN_H_

#include "nn_alg.h"
#include "prim_euler_alg.h"
#include "little_alg.h"

#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Util/Application.h>
#include <Poco/JSON/JSON.h>
#include <Poco/JSON/Object.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>

namespace croutes {
    template <typename T>
    ndata_ptr<T> read_data(Poco::JSON::Object::Ptr json) {

        using namespace Poco::JSON;

        size_t nodes_count = json->getValue<size_t>("size");
        Array::Ptr rows_arr = json->get("arr").extract<Array::Ptr>();


        auto d = std::make_shared<ndata<T>>(nodes_count);
        for (size_t i = 0; i < rows_arr->size(); ++i) {
            Array::Ptr cols_arr = rows_arr->get((unsigned) i).extract<Array::Ptr>();
            for (size_t j = 0; j < cols_arr->size(); ++j) {
                T dist = cols_arr->getElement<T>((unsigned) j);
                d->add_node((int32_t) i, (int32_t) j, dist);
            }
        }

        return d;
    }
}

namespace croutes_server {

    template <typename T>
    Poco::JSON::Object expirement(Poco::Util::Application& app, croutes::ndata_ptr<T> data) {
        using namespace Poco;

        croutes::algorithm<T>* algs[] = {
                new croutes::nn_alg<T>(),
                new croutes::prim_euler_alg<T>(),
                new croutes::little_alg<T>()
        };

        JSON::Object res;

        for (auto& alg : algs) {
            auto a = alg->compute(data);
            JSON::Object alg_obj;
            JSON::Array results_arr;

            for (const auto& b : a->bundles()) {
                JSON::Array json_bundle;
                for (const auto& bond : *b) {
                    JSON::Object json_bond;
                    json_bond.set("from", bond->from());
                    json_bond.set("to", bond->to());
                    json_bond.set("distance", bond->distance());
                    json_bundle.add(json_bond);
                }
                results_arr.add(json_bundle);
            }

            alg_obj.set("name", alg->russian_name());
            alg_obj.set("results", results_arr);
            res.set(alg->short_name(), alg_obj);


            delete alg;
            alg = nullptr;
        }
        return res;
    }

    class RootHandler: public Poco::Net::HTTPRequestHandler
    {
    public:
        void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) {
            Poco::Util::Application& app = Poco::Util::Application::instance();
            app.logger().information("[RootHandler] Request from " + request.clientAddress().toString());

            response.setChunkedTransferEncoding(true);
            response.setContentType("text/html");
            std::ostream& ostr = response.send();
            ostr << "<html><head><title>HTTP Server powered by POCO C++ Libraries</title></head>";
            ostr << "<body>";
            ostr << "</body></html>";
        }
    };

    class DataHandler: public Poco::Net::HTTPRequestHandler
    {
    public:
        void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) {
            Poco::Util::Application& app = Poco::Util::Application::instance();
            app.logger().information("[DataHandler] Request from " + request.clientAddress().toString());

            std::istream& is = request.stream();

            Poco::JSON::Parser parser;
            auto raw_arr = parser.parse(is);
            auto json_arr = raw_arr.extract<Poco::JSON::Object::Ptr>();


            auto data = croutes::read_data<double>(json_arr);
            Poco::JSON::Object res = expirement(app, data);


            response.setChunkedTransferEncoding(true);
            response.setContentType("application/json");
            std::ostream& ostr = response.send();

            res.stringify(ostr);
        }

    private:

    };

    class RequestHandlerFactory : public Poco::Net::HTTPRequestHandlerFactory {
    public:
        RequestHandlerFactory() {
        }

        Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest& request) {
            if (request.getURI() == "/")
                return new RootHandler();
            else if (request.getURI() == "/calculate")
                return new DataHandler();
            return new RootHandler();
        }
    };

}

#endif //_CIRCLE_ROUTES_MAIN_H_
