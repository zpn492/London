#include "matrix.hpp"

namespace linalg
    {
    /* ******************************************************** */
    /* Operations */
    /* ******************************************************** */

    int Matrix::operator*(double v)
        {
        for(int i = 0; i < data.size(); i++)
            {
            for(int j = 0; j < data[i].size(); j++)
                data[i][j] *= v;
            }
        return 0;
        };
    
     int Matrix::operator+(double v)
        {
        for(int i = 0; i < data.size(); i++)
            {
            for(int j = 0; j < data[i].size(); j++)
                data[i][j] += v;
            }
        return 0;
        };

    int Matrix::operator%(double v)
        {
        for(int i = 0; i < data.size(); i++)
            {
            for(int j = 0; j < data[i].size(); j++)
                data[i][j] = std::fmod(data[i][j], v);
            }
        return 0;
        };   

    int Matrix::operator*(const Matrix &m) throw()
        {
        try
            {
            if(cols != m.rows)
                throw multiplication;

            Matrix mn(rows, m.cols);  mn.fill(0);
            double n;
            
            for(int i = 0; i < rows; i++)
                {
                for(int j = 0; j < m.cols; j++)
                    {
                    n = 0;
                    for(int k = 0; k < cols; k++)
                        {
                        n += data[i][k] * m.data[k][j];
                        }
                    mn.data[i][j] = n;
                    }
                }
            data = mn.data; rows = mn.rows; cols = mn.cols;
            }
        catch(std::exception& e)
            {
            std::cout << e.what() << " (Matrix A): (" << cols << ")" << " (Matrix B): (" << m.rows << ")" << std::endl;
            std::abort();
            }
        return 0;
        };

    int Matrix::operator+(const Matrix &m)
        {
        for(int i = 0; i < m.rows; i++)
            for(int j = 0; j < m.cols; j++)
                data[i][j] += m.data[i][j];
        return 0;
        };

    int Matrix::operator-(const Matrix &m)
        {
        for(int i = 0; i < m.rows; i++)
            for(int j = 0; j < m.cols; j++)
                data[i][j] -= m.data[i][j];
        return 0;
        };

    int Matrix::operator=(const Matrix &m)
        {
        data = m.data; rows = m.rows; cols = m.cols;
        return 0;
        };

    /* ******************************************************** */
    /* Matrix */
    /* ******************************************************** */

    Matrix::Matrix(int r, int c)
        {
        rows = r;
        cols = c;
        };

    Matrix::Matrix(const Matrix &m)
        {
        rows = m.rows;
        cols = m.cols;
        data = m.data;
        };

    Matrix::Matrix(std::vector<std::vector<double> > t_data)
        {
        rows = t_data.size();
        cols = t_data[0].size();
        data = t_data;
        };

    Matrix::Matrix(std::vector<std::vector<int> > t_data)
        {
		for(int i = 0; i < t_data.size(); i++)
			{
			std::vector<double> d(t_data[i].begin(), t_data[i].end());
			data.push_back(d);
			}
        };

    void Matrix::row_scalar_multiplication(int row, double value)
        {
        for(int i = 0; i < data[row-1].size(); i++)
            data[row-1][i] *= value;
        };
    
    void Matrix::row_addition_multilpum(int rowA, int rowB, int times, char isAddition)
        {
        if(isAddition == 'T')
            {
            for(int i = 0; i < data[rowA-1].size(); i++)
                data[rowA-1][i] += data[rowB-1][i]*times;
            }
        else
            {
            for(int i = 0; i < data[rowA-1].size(); i++)
                data[rowA-1][i] -= data[rowB-1][i]*times;
            }
        };

    void Matrix::row_move(int fromRow, int toRow)
        {
        std::vector<double> tmp = data[toRow-1];
        data[toRow-1] = data[fromRow-1];
        data[fromRow-1] = tmp;
        };
    
    /* ******************************************************** */
    /* Helper functions */
    /* ******************************************************** */

    void Matrix::fill(double value)
        {
        for(int i = 0; i < rows; i++)
            {
            data.push_back(std::vector<double>());
            for(int j = 0; j < cols; j++)
                data[i].push_back(value);
            }
        };

    void Matrix::transpose()
        {
        Matrix m(cols, rows); m.fill(0);
        for(int i = 0; i < cols; i++)
            for(int j = 0; j < rows; j++)
                m.set_value(i+1, j+1, data[j][i]);
        data = m.data; rows = m.rows; cols = m.cols;
        };
    
    void Matrix::set_value(int row, int col, double value) throw()
        {
        try
            {
            if(row > (int) rows || row < 1 || col > (int) cols || col < 1)
                throw outofbounds;
            data[row-1][col-1] = value;
            }
        catch(std::exception& e)  
            {
            std::cout << e.what() << " request: (" << row << ", " << col << ")" << " (rows, cols): (" << (int) rows << ", " << (int) cols << ")" << std::endl;
            std::abort();
            } 
        };

    double Matrix::get_value(int row, int col) throw()
        {
        try
            {
            if(row > (int) rows || row < 1 || col > (int) cols || col < 1)
                throw outofbounds;
            return data[row-1][col-1];
            }
        catch(std::exception& e)
            {
            std::cout << e.what() << " request: (" << row << ", " << col << ")" << " (rows, cols): (" << (int) rows << ", " << (int) cols << ")" << std::endl;
            std::abort();
            }
        };

    void Matrix::pretty_print()
        {
        std::cout << "[ ";
        
        for(int i = 0; i < data.size(); i++)
            {
            std::cout << "[";
            for(int j = 0; j < data[i].size()-1; j++)
                {
                std::cout << data[i][j] << ", ";                    
                }
            std::cout << data[i][data[i].size()-1];
            std::cout << "] ";
            }
        std::cout << "]" << std::endl;;
        };

    /* ******************************************************** */
    /* Solve system of linear equations */
    /* ******************************************************** */
        
    std::vector<Matrix> lu_decomposition(Matrix &A)
        {
        std::vector<Matrix> lu;
        Matrix TMP(A);
        Matrix L(A.rows, A.rows); L.fill(1);
        Matrix U(A.rows, A.rows); U.fill(1);

        int i, k, j;

        for(i = 0; i < A.rows; i++)
            for(j = 0; j < A.rows; j++) 
                {
                if(j < i) U.set_value(i+1,j+1,0);
                if(j > i) L.set_value(i+1,j+1,0);
                }

        for(k = 0; k < A.rows; k++)
            {
            U.set_value(k+1, k+1, A.get_value(k+1, k+1) ) ;
            for(i = k + 1; i < A.rows; i++)
                {
                L.set_value(i+1, k+1, A.get_value(i+1, k+1) / A.get_value(k+1, k+1) );
                U.set_value(k+1, i+1, A.get_value(k+1, i+1) );
                }
            for(i = k + 1; i < A.rows; i++)
                for(j = k + 1; j < A.rows; j++)
                    A.set_value(i+1, j+1, A.get_value(i+1, j+1) - L.get_value(i+1, k+1) * U.get_value(k+1, j+1) );
            }
        lu.push_back(L);
        lu.push_back(U);
        
        A = TMP;

        return lu;
        };

    Matrix ly_b_solve(Matrix &L, Matrix &b)
        {
        Matrix Y(L.rows, 1); Y.fill(0);
        int i, j; 
        double tmp;

        Y.set_value(1, 1, b.get_value(1, 1) );

        for(i = 1; i < L.rows; i++)
            {
            tmp = 0;
            for(j = 0; j < i; j++)
                tmp += L.get_value(i+1, j+1) * Y.get_value(j+1,1);
            Y.set_value(i+1, 1, b.get_value(i+1, 1) - tmp);
            }
        return Y;
        };

    Matrix ux_y_solve(Matrix &U, Matrix &Y)
        {
        Matrix X(U.rows, 1); X.fill(0);
        int i, j;
        double tmp;

        X.set_value(U.rows, 1, Y.get_value(U.rows, 1) / U.get_value(U.rows, U.rows) );
        
        for(i = U.rows-2; i > -1; i--)
            {
            tmp = 0;
            for(j = i+1; j < U.rows; j++)
                {                
                tmp += U.get_value(i+1, j+1) * X.get_value(j+1, 1); 
                }
            X.set_value(i+1, 1, (Y.get_value(i+1, 1) - tmp) / U.get_value(i+1, i+1) );
            }

        return X;
        };

    Matrix lup_solve(Matrix &A, Matrix &b)
        {
        std::vector<Matrix> lu = lu_decomposition(A);

        Matrix Y = ly_b_solve(lu[0], b);

        Matrix X = ux_y_solve(lu[1], Y);

        return X;
        };
    };

