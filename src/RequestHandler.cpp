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
			auto header = request.headers().tryGet<Pistache::Http::Header::ContentType>();
			// std::cout <<  << std::endl;
			if (header)
  				std::cout<<"header exist" << std::endl;
			// auto rawValue = Pistache::Http::Header::header_cast<const Pistache::Http::Header::ContentType>(request.headers().tryGet("Content-Type"));
			// if(*header == "application/json"){
				// std::cout << *header << std::endl;
			// }
			response.send(Http::Code::Ok, "Pong");
			return;
		}
	}
	if(request.resource() == "/add"){
		if(request.method() == Http::Method::Post){
			std::ofstream deviceLogFile;
			deviceLogFile.open("device_log_file", std::ios::app);

			// nlohmann::json body = nlohmann::json::parse("{ \"happy\": true, \"pi\": 3.141 }");
			// std::cout << body["happy"]<< std::endl;

			deviceLogFile << request.body() << std::endl;
			deviceLogFile.close();

			response.send(Http::Code::Ok, request.body(), MIME(Text, Plain));
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