#include <iostream>

namespace epric{

void error(char* message, char code){
    std::cout << message << '\n';
    exit(code);
}

void error(char* message){
    std::cout << message << '\n';
    exit(EXIT_FAILURE);
}

}//end of namespace epric