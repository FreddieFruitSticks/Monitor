#include "RequestHandler.h"
#include "pistache/endpoint.h"
#include <signal.h>

volatile int flag = true;

void signalHandler(int signal){
	std::cout << "Signal: " << signal << std::endl;
	flag = false;
}

int main(){
    signal(SIGINT, signalHandler);
    signal(SIGILL, signalHandler);
    signal(SIGKILL, signalHandler);

    Pistache::Address addr(Pistache::Ipv4::any(), Pistache::Port(8081));
    auto opts = Pistache::Http::Endpoint::options()
    			.threads(1)
    			.flags(Pistache::Tcp::Options::ReuseAddr);

    Http::Endpoint server(addr);
    server.init(opts);
    server.setHandler(Http::make_handler<RequestHandler>());
    server.serveThreaded();

    while(flag){
    	sleep(1);
    }
    server.shutdown();
    std::cout << "ending server" << std::endl;
    return 0;
}