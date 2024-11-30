// Virtual Memory Paging Simulation by Volodymyr Zabolontyi
//
// This program simulates the handling of page faults in virtual memory.
// It models a simple page table and main memory, tracks virtual to physical 
// memory mapping, and calculates the page fault ratio based on input memory references.

#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <tuple>
#include <cmath>
#include <vector>
#include <iomanip>

// Converts binary number to decimal (used for page table lookup)
int bin_to_dec(int number){
    std::string input_num;
    int out_num;
    input_num = std::to_string(number);
    std::reverse(input_num.begin(), input_num.end());
    out_num = 0;
    for(int i = 0; i < input_num.length(); i++){
        if (input_num[i] == '1'){
             out_num += pow(2,i);  // Sum powers of 2 for each '1' bit
        }      
    }
    return out_num;
}

// Page table class simulates page-to-frame mapping, tracking hits, misses, and memory state.
class Page_table{
    public:
    std::map<int, std::tuple<int, int>> reference_table;  // Maps page numbers to frame numbers and validation bit
    std::vector<int> main_memory;  // Stores current frame contents (-1 means empty)
    int mm_size, page_table_size;  // Memory sizes
    float hits = 0, misses = 0;  // Counters for page hits and misses
    
    // Initializes the page table and main memory
    void initialize(int size, int mm_size){
        page_table_size = size;  // Set page table size
        main_memory.clear();  // Clear main memory
        for(int i = 0; i < mm_size; i++){
            main_memory.push_back(-1);  // Mark all frames as empty (-1)
        }
        for(int i = 0; i < size; i++){
            reference_table[i] = std::make_tuple(-1,0);  // Initialize reference table: (-1, 0) means no frame, not validated
        }
    }

    // Searches for a page in main memory or adds it if not found
    int search_element(int page){
        page = bin_to_dec(page);  // Convert page address from binary to decimal
        if (std::get<1>(reference_table[page]) == 1){  // If page is already in memory (valid)
            hits++;  // Increment hit counter
            return std::get<0>(reference_table[page]);  // Return corresponding frame number
        } else {
            return add_element(page);  // Page fault: add the page to memory
        }
    }

    // Calculates the current page fault ratio
    float calculate_page_fault(){
        return misses / (hits + misses);  // Page fault ratio = misses / (hits + misses)
    }

    private:
    // Adds a page to main memory and updates the page table
    int add_element(int page){
        std::cout << "\n Page Fault Detected, current Page Fault Ratio: " << this->calculate_page_fault() << "%\n";
        misses++;  // Increment miss counter
        int correspondance = 0;  // Variable to store the frame number
        if (page <= page_table_size){  // Ensure the page is within the address space
            for(int i = 0; i < main_memory.size(); i++){
                if(main_memory[i] == -1){  // If there's an empty frame
                    main_memory[i] = 1;  // Load page into the frame
                    correspondance = i;
                    // Clear any previous reference to this frame in the page table
                    for(int i = 0; i < reference_table.size(); i++){
                        if(std::get<0>(reference_table[i]) == correspondance){
                            reference_table[i] = std::make_tuple(-1, 0);
                        }
                    }
                    break;
                } else if (i + 1 >= main_memory.size()){  // If no empty frames, replace the first one
                    correspondance = 0;
                    main_memory[0] = 1;  // Load page into the first frame
                    // Clear any reference to this frame in the page table
                    for(int i = 0; i < reference_table.size(); i++){
                        if(std::get<0>(reference_table[i]) == 0){
                            reference_table[i] = std::make_tuple(-1, 0);
                        }
                    }
                }
            }
            // Update page table with the new mapping (page -> frame)
            reference_table[page] = std::make_tuple(correspondance, 1);
            return correspondance;  // Return the frame number
        } else {
            std::cout << "Addressing index not existent in page_table";  // If page is out of bounds
        }
    }
};

// Main program to handle memory references and manage page table operations
int main(void){
    // Open input file for reading memory requests (use absolute paths on macOS)
    std::ifstream inputFile("/Users/volodymyrzabolotny/Desktop/CS/CSC 210/program4/input.txt");
    std::string line;
    int counter = 0, page_size, mm_size, vm_size;
    bool new_flag;
    std::tuple<double, double> vm_format, mm_format;
    Page_table page_table;

    // Process each line of the input file
    while (getline(inputFile, line)) {
        std::cout << "--- New line ---\n";
        std::cout << line << std::endl;
        counter++;  // Increment line counter

        // Read page size and memory size from the first two lines
        if (counter == 1) page_size = std::stoi(line);  // First line: page size
        else if (counter == 2) mm_size = std::stoi(line);  // Second line: memory size  
        else if (line == "NEW") new_flag = true;  // Flag to start a new memory reference set

        else if (new_flag == true){ 
            vm_size = std::stoi(line);  // Virtual memory size
            vm_format = std::make_tuple(log2(vm_size), log2(page_size));  // VM address format
            mm_format = std::make_tuple(log2(mm_size), log2(page_size));  // MM address format
            page_table.initialize(vm_size, mm_size);  // Initialize page table and memory
            new_flag = false;  // Reset flag
        }

        else {  // Process memory reference
            std::string request = line;
            std::tuple<int, int> vm_request = std::make_tuple(std::stoi(line.substr(0, std::get<0>(vm_format))), 
                                                              std::stoi(line.substr(std::get<0>(vm_format), std::get<1>(vm_format))));
            std::tuple<int, int> mm_request = std::make_tuple(page_table.search_element(std::get<0>(vm_request)), 
                                                               std::get<1>(vm_request));
        }

        // Output current page table and memory state
        for(int i = 0; i < page_table.reference_table.size(); i++){
            std::cout << "| " << i << "| " << std::get<0>(page_table.reference_table[i]) << "| " << std::get<1>(page_table.reference_table[i]) << "|" << "\n";
        }
        std::cout << "\nMain memory:\n";
        for(int i = 0; i < page_table.main_memory.size(); i++){
            std::cout << "| " << i << "| " << page_table.main_memory[i] << "|\n";
        }
    }
    inputFile.close();  // Close input file after processing
    return 0;
}
