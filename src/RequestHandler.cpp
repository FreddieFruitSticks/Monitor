#include "RequestHandler.h"
#include "pistache/http_header.h"
#include <json.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <regex>

std::vector<std::string> delimiter(const std::string stringIn, char delim){
	std::vector<std::string> tokens;
	std::stringstream ss(stringIn);
	std::string subString;
	while(std::getline(ss, subString, delim)){
		tokens.push_back(subString);
	}
	return tokens;
}

std::vector<std::string> tail(FILE* in, int n){
	std::vector<std::string> data;
    int count = 0;  // To count '\n' characters
 
    // unsigned long long pos (stores upto 2^64 – 1
    // chars) assuming that long long int takes 8 
    // bytes
    unsigned long long pos;
	int SIZE = 150;
    char str[10*SIZE];
 
    // Go to End of file
    if (fseek(in, 0, SEEK_END)){
        perror("fseek() failed");
    }else{
        // pos will contain no. of chars in
        // input file.
        pos = ftell(in);
 
        // search for '\n' characters
        while (pos){
            // Move 'pos' away from end of file.
            if (!fseek(in, --pos, SEEK_SET)){
                if (fgetc(in) == '\n'){
 
                    // stop reading when n newlines
                    // is found
                    if (count++ == n)
                        break;
				}
            }
            else{
                perror("fseek() failed");
			}
        }
 
        // print last n lines
        // printf("Printing last %d lines -\n", n);
        while (fgets(str, sizeof(str), in)){
			data.push_back(str);
		}
    }
	return data;
}
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

			if(request.body().size() > 50){
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
	if(std::regex_match(request.resource(), std::regex("/read/[1-9][0-9]*"))){
		std::vector<std::string> tokens = delimiter(request.resource(), '/');
		int numberOfLines =std::stoi(tokens.back());

		if(request.method() == Http::Method::Get){
			std::vector<std::string> data;
			FILE* fp = fopen("device_log_file", "rt");
			data = tail(fp, numberOfLines);
			// std::ifstream inDeviceLogs("device_log_file");
			// std::string singleLogEntry;
			// int lineNumber = 1;
			// while(std::getline(inDeviceLogs, singleLogEntry)){
			// 	// std::vector<std::string>::iterator it = data.begin();
  			// 	// it = data.insert(it, singleLogEntry);
			// 	if (lineNumber <= numberOfLines){
			// 		data.push_back(singleLogEntry);
			// 		lineNumber++;
			// 	}else{
			// 		break;
			// 	}
			// }
			fclose(fp);
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