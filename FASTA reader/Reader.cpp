//FESTA Reader
#include <iostream>
#include <fstream>
using namespace std;

//Function accepts two aditional parameters:
// argv[1] as a name of input file
// argv[2] as a name of output file
int main( int argc, char **argv ){
	
	//creating and opening output stream
	ofstream myfile;
    myfile.open(argv[2]);  
	
    if( argc <= 2 ){
        std::cerr << "Some parameters are missing. " << std::endl;
        return -1;
    }
 
    std::ifstream input(argv[1]);
    if(!input.good()){
        std::cerr << "Error while opening: '"<<argv[1]<<" " << std::endl;
        return -1;
    }
 
    std::string line, name, content;
    while( std::getline( input, line ).good() ){
        if( line.empty() || line[0] == '>' ){ 
            if( !name.empty() ){ 
                myfile << content;                
                name.clear();
            }
            if( !line.empty() ){
                name = line.substr(1);
            }
            content.clear();
		} else if( !name.empty() ){
            if( line.find(' ') != std::string::npos ){ 
                name.clear();
                content.clear();
            }
             else {
				 //add line if regular
                content += line;
            }
        }
    }
    
    if( !name.empty() ){ 
        myfile <<  content ;
    }
    myfile.close();
    return 0;
}
