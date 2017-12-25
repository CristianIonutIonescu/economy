#include <iostream>
#include "dataretriever.hpp"

using namespace economy;
using namespace economy::client;

int main(int argc ,char **argv) {
    DataRetriever test("localhost:8080");
    Date beg("12-10-2017");
    Date end("12-12-2017");

    try {
        auto data = test.GetData(beg,end);

        for(const auto it: data) {
            std::cout<<it.first.ToString()<<":"<<it.second<<std::endl;
        }
    }catch(const std::exception &ex) {
        std::cerr<<ex.what()<<std::endl;
        return 1;
    }

    return 0;
}