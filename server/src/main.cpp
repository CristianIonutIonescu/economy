#include <iostream>
#include "application.hpp"

#include <csignal>

void signalHandler( int signum ) {
   economy::server::Application::s_close = true; 
}

int main(int argc ,char **argv) {
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);
    
    try {
        economy::server::Application app(argc, argv);
        app.Run();
        app.Close();
    }catch(const std::runtime_error  &ex) {
        std::cerr<<ex.what();
        return 1;
    }    
    return 0;
}