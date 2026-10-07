#include <iostream>
#include <fstream>

int number_of_data(const char* input_file){
    int num{}; std::fstream my_file; 
    my_file.open(input_file, std::ios::in);
    if (my_file.is_open()){ double i; 
        while(my_file >> i){ num++; } my_file.close(); 
    }
    else std::cout << "Unable to open file" << std::endl;
    return num;
}
void load_data(const char* input_file, double data[], int size){
    std::fstream my_file; 
    my_file.open(input_file, std::ios::in);
    if (my_file.is_open()){ 
        for(size_t i=0; i<size; i++){
            my_file >> data[i]; 
        } 
    my_file.close(); 
    }
    else std::cout << "Unable to open file" << std::endl;
}
void write_data(const char* output_file, int data[], int size){
    std::fstream my_file; 
    my_file.open(output_file, std::ios::out);
    if (my_file.is_open()){ 
        for(size_t i=0; i<size; i++ ){ 
        my_file << data[i] << std::endl; 
    } my_file.close(); }
    else std::cout << "Unable to open file" << std::endl;
}
