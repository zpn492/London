#ifndef FILEHANDLER_H_
#define FILEHANDLER_H_

#include "../../SimpleJust.h"

namespace filehandler
    {
    extern bool exists_file(const char *filename);
    
    /*
     * NIX way to get file size without seeking to the end and back
     * struct stat filestatus;
     * stat(filename, &filestatus);
     * size_t total_size = filestatus.st_size;
     */
    extern std::streampos file_size(const char *filename);
    
    /* Read an entire file into a string */
    extern std::string get_file_contents(const char *filename);

    extern std::string get_file_chunk(const char *filename, 
    size_t begin, size_t end);

    extern bool create_file(const char *filename);

    extern bool delete_file(const char *filename);
	
	/* Split a string by charator fx '\n' newline */
    extern std::vector<std::string> split_string(std::string &s, char delimeter);

	/* Divide a string into chunks
	 * Make sure dividing wont create more rows or columns
	 * strings below a length of 10.000 wont be divided.
     */
    extern std::vector<std::string> divide_string(std::string s1, int times);

	/* Write a string into a file
     * If the file already exists, it will be erased before insertion.
     */
    extern void write_file(const char *filename, std::string text);

    extern void print_file(const char *filename);

    extern std::string int_to_string(int value);

    extern std::string double_to_string(double value);
	
	extern void replace(std::string &s, char before, char after);

    extern std::string space(std::string s, int space);

    /* This methods creates .js file
     * this file is ready to be integrated with boostrap: https://mdbootstrap.com/javascript/charts/ 
     * Format: 
     * labels: [keys[0]->key, .., keys[n]->key] 
     * data: [keys[0]->occurrences, .., keys[n]->occurrences]
     */
    extern void histogram(const char *filename, std::vector<std::string> labels, std::vector<double> dataset);

    /* This methods creates .js file
     * this file is ready to be integrated with boostrap: https://mdbootstrap.com/javascript/charts/ 
     * Format: 
     * labels: [keys[0]->key, .., keys[n]->key] 
     * data: [keys[0]->occurrences, .., keys[n]->occurrences]
     */
    extern void linechart(const char *filename, std::vector<std::string> labels, std::vector<double> dataset);
	
    };
#endif