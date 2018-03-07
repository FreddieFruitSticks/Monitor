#include "HelloHandler.h"

void HelloHandler::onRequest(const Http::Request& request, Http::ResponseWriter response){
	response.send(Http::Code::Ok, "Hello, World");
}
