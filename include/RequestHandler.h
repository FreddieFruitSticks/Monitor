#include "pistache/endpoint.h"
using namespace Pistache;

class RequestHandler : public Http::Handler {
public:
	HTTP_PROTOTYPE(RequestHandler)
public:
	void onRequest(const Http::Request& request, Http::ResponseWriter response);
private:
	std::string prettifyJson(std::string json);
};