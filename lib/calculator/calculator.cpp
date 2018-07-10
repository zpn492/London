#include "calculator.hpp"

using namespace linalg;

void linalg::create_matrix(std::string line, std::string name, std::map<std::string, Matrix> &outputdata)
    {
    std::vector<std::string> rows = filehandler::split_string(line, '.'); // rows

    std::vector<std::string> cols = filehandler::split_string(rows[0], ','); // cols

    std::vector<std::string> evalues = cols;

    Matrix m(rows.size(), cols.size()); m.fill(0);
    
    for(int j = 0; j < rows.size(); j++)
        {
        if(j==0)
            evalues = cols;
        else
            evalues = filehandler::split_string(rows[j], ',');

        for(int k = 0; k < evalues.size(); k++)           
            m.set_value(j+1,k+1,atof(evalues[k].c_str()));
        }
    outputdata.insert( std::pair<std::string, Matrix>(name, m) );
    };

void linalg::arithmics(char &operation, std::string a, std::string b, std::string name, std::map<std::string, Matrix> &outputdata)
    {
    Matrix A = outputdata.find(a)->second;
    Matrix B = outputdata.find(b)->second;
       
    switch(operation)
        {
        case '+':
            A + B;
        break;
        case '*':
            A * B;
        break;
        }
    outputdata.insert( std::pair<std::string, Matrix>(name, A) );
    
    };


void linalg::scalar(char &operation, std::string a, std::string num, std::string name, std::map<std::string, Matrix> &outputdata)
    {
    Matrix A = outputdata.find(a)->second;
    switch(operation)
        {
        case '+': A + atoi(num.c_str()); break;
        case '*': A * atoi(num.c_str()); break;
        }
    outputdata.insert( std::pair<std::string, Matrix>(name, A) );
    };
    
std::map<std::string, Matrix> linalg::decode_file(const char *filename)
    {
    Logger log;
    std::string s = filehandler::get_file_contents(filename, log);

    s.erase(std::remove(s.begin(), s.end(), '\r'), s.end());   
    s.erase(std::remove(s.begin(), s.end(), '\n'), s.end());  

    std::vector<std::string> lines = filehandler::split_string(s, ':');

    std::map<std::string, Matrix> lvalues;
    std::vector<std::string> rvalues;
    
    for(int i = 0; i < lines.size(); i++)
        {
        rvalues = filehandler::split_string(lines[i], ';');
        
        switch(rvalues[0][0])
            {
            case 'A': { linalg::create_matrix(rvalues[2], rvalues[1], lvalues); } break;
            case 'C': {/* Comment */} break;
            case 'M': { linalg::arithmics(rvalues[3][0], rvalues[2], rvalues[4], rvalues[1], lvalues); } break;
            case 'S': { linalg::scalar(rvalues[3][0], rvalues[2], rvalues[4], rvalues[1], lvalues); } break;
            case 'T':
                {
                Matrix A = lvalues.find(rvalues[2])->second;
                A.transpose();
                if(rvalues[2] == rvalues[1])
                    lvalues.find(rvalues[2])->second = A;
                else
                    lvalues.insert( std::pair<std::string, Matrix>(rvalues[1], A) );
                }
            break;
            case 'R':
                {
                Matrix A = lvalues.find(rvalues[2])->second;
                switch(rvalues[3][0])
                    {
                    case 'M':
                        A.row_scalar_multiplication(atoi(rvalues[5].c_str()), atof(rvalues[4].c_str()) );
                    break;
                    case 'B':
                        A.row_move(atoi(rvalues[5].c_str()), atoi(rvalues[6].c_str()) );
                    break;
                    case 'S':
                        A.row_addition_multilpum(atoi(rvalues[5].c_str()), atoi(rvalues[6].c_str()), atoi(rvalues[4].c_str()), rvalues[7][0] );
                    break;
                    }
                if(rvalues[2] == rvalues[1])
                    lvalues.find(rvalues[2])->second = A;
                else
                    lvalues.insert( std::pair<std::string, Matrix>(rvalues[1], A) );
                }
            break;
            case 'L':
                {
                Matrix A = lvalues.find(rvalues[2])->second;
                Matrix b = lvalues.find(rvalues[3])->second;
                Matrix X = lup_solve(A, b);
                if(rvalues[2] == rvalues[1])
                    lvalues.find(rvalues[2])->second = X;
                else
                    lvalues.insert( std::pair<std::string, Matrix>(rvalues[1], X) );
                }
            break;
            }
        }
        return lvalues;
    };

void linalg::pretty_print(std::map<std::string, Matrix> lvalues)
    {
    std::map<std::string,Matrix>::iterator it;
    for (it=lvalues.begin(); it!=lvalues.end(); ++it) 
        {
        std::cout << it->first.c_str() << ": ";
        it->second.pretty_print();
        }
    };