
#include "../../SimpleJust.h"

#include "../../lib/filehandler/filehandler.h"
#include "../../lib/matrix/matrix.hpp"

/* ******************************************************** */
/* Examples */
/* ******************************************************** */
/*
    file:
    Option;Outvariable;Inputvariable_A;x11,x12.x21,x22:
    Option;Outvariable;Inputvariable_A;x11,x12.x21,x22:
    ...:
    Option;Outvariable;Inputvariable_A;operation;Inputvariable_B

    Options#
    A : Create matrix
    C : Comment
    M : Matrix multiplication
    S : Scalar multiplication
    T : Transpose
    R : Row operation
        M : Scalar multiplication
        B : Row shift
        S : Row addtion multiplum 
    L : LUP Solve(A, b)
    
    A# Create Matrix A
    A;A;2,3,1.4,-1,-2
        -> c++: int v1[2][3] = { {2, 3, 1}, {4, -1, -2} }; Matrix A(MatrixFactory<2,3>(v1));

    B# Comment
    C;What ever you want;To ;Type

    C# Matrix multiplication
    C;Create two matrix A and B
    A;A;1,-2.2,0.3,3:
    A;B;2,1.-1,1:
    C;:
    C;Multiply them and store in a new matrix V:
    M;V;A;*;B

    D# Scalar multiplication
    A;A;1,-2.2,0.3,3:
    C;Multiply by 3 and store in A:
    S;A;A;*;3

    E# Transpose
    C;Transpose matrix A
    T;A;A

    C;Transpose matrix A and store in V
    T;V;A

    F# Row operations
    0 1 2 3       4              5    6       7
    R;A;A;[M|B|S];[scalar|times];rowA;[rowB|];[T|F]

    A;A;-2,-2,4.1,-2,3:
    R;V1;A;M;0.5;1:
    R;V2;V1;B;_;1;2:
    R;V3;V2;S;2;1;2;F

    G# LUP solve
    A;A;2,-3,1.-1,1,-1.3,2,2:
    C;:
    A;b;-1.2.3:
    L;X;A;B
*/
#ifndef __CALCULATOR__
#define __CALCULATOR__
namespace linalg
    {
    std::map<std::string, Matrix> decode_file(const char *filename);

    void pretty_print(std::map<std::string, Matrix> obj);

    void create_matrix(std::string, std::string, std::map<std::string, Matrix>&);

    void arithmics(char&, std::string, std::string, std::string, std::map<std::string, Matrix>&);

    void scalar(char&, std::string, std::string,  std::string, std::map<std::string, Matrix>&);
    };
#endif