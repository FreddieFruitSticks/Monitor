#include "RequestHandler.h"
#include "pistache/http_header.h"
#include <json.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

void RequestHandler::onRequest(const Http::Request& request, Http::ResponseWriter response){

	if (request.resource() == "/ping"){
		if(request.method() == Http::Method::Get){
			// auto header = request.headers().tryGet<Pistache::Http::Header::ContentType>();
			response.send(Http::Code::Ok, "Pong");
		}
	}
	if(request.resource() == "/add"){
		if(request.method() == Http::Method::Post){
			// nlohmann::json body = nlohmann::json::parse("{ \"happy\": true, \"pi\": 3.141 }");

			if(request.body().size() > 150){
				std::ofstream deviceLogFile;			
				deviceLogFile.open("device_log_file", std::ios::app);
				deviceLogFile << request.body() << std::endl;
				deviceLogFile.close();

				response.send(Http::Code::Ok, "");
			}else{
				response.send(Http::Code::Internal_Server_Error, "");	
			}
		}else{
			response.send(Http::Code::Internal_Server_Error, "");
		}
	}
	if(request.resource() == "/read"){
		if(request.method() == Http::Method::Get){
			std::vector<std::string> data;
			std::ifstream inDeviceLogs("device_log_file");
			std::string singleLogEntry;
			while(std::getline(inDeviceLogs, singleLogEntry)){
				data.push_back(singleLogEntry);
			}
			std::string responseBody = RequestHandler::vectorToString(data);
			response.send(Http::Code::Ok, responseBody);
		}
	}
	response.send(Http::Code::Ok, "Received Request but could do nothing with it");
}

std::string RequestHandler::vectorToString(std::vector<std::string> entries) {
	std::vector<std::string>::iterator dataIterator;
	std::string response = "[";

	for(dataIterator = entries.begin(); dataIterator != entries.end(); dataIterator++){
		if(std::next(dataIterator) == entries.end()){
			response = response.append(*dataIterator);
		}else{
			response = response.append(*dataIterator).append(",");
		}
	}
	response.append("]");
	return response;
}