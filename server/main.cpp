#include <iostream>
#include "application.hpp"

int main(int argc ,char **argv) {
    try {
        economy::server::Application app(argc, argv);
        app.Run();
    }catch(const std::runtime_error  &ex) {
        std::cerr<<ex.what();
        return 1;
    }
    
    return 0;
}