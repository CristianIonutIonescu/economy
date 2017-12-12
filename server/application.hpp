#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "datatypes.pb.h"

class Application {
    public:
        Application(int argc, char** argv);
        void Start();
        void Stop();
    private:
        
};
#endif