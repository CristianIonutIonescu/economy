#include "dateparser.hpp"
#include "transportservice.hpp"
#include <iostream>

int main() {
    economy::server::DataParser parser;
    parser.Init("../data.csv");
    parser.Print();
    auto period = parser.GetRange(economy::Date(11,11,2017), economy::Date(13,11,2017));
    std::cout<<period.size()<<std::endl;
    for(const auto &it: period) {
        std::cout<<it.first.ToString()<<"-"<<it.second<<std::endl;
    }
    return 0;
}