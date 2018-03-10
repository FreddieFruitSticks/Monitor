#include "RequestHandler.h"
#include <json.hpp>

void RequestHandler::onRequest(const Http::Request& request, Http::ResponseWriter response){

	if (request.resource() == "/test"){
		if(request.method() == Http::Method::Get){
			response.send(Http::Code::Ok, "test worked");
			return;
		}
	}
	if(request.resource() == "/add"){
		if(request.method() == Http::Method::Post){
			std::string body = request.body();
			nlohmann::json reading = nlohmann::json::parse("{ \"happy\": true, \"pi\": 3.141 }");

			response.send(Http::Code::Ok, request.body(), MIME(Text, Plain));
		}
	}
	response.send(Http::Code::Ok, "Received Request but could do nothing with it");
}

std::string RequestHandler::prettifyJson(std::string json){
	return "";
}