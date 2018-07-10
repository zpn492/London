# linalg <br />

> Opstil matricer <br />
> Matrix addition og multiplikation <br />
> Transponer <br />
> Rækkeroperationer <br />
> Løs ligningssystem <br />

> Opstil to matricer <br />
```c++
// 
int v1[2][3] = {{2, 3, 1}, {4, -1, -2} };
int v2[2][3] = {{1, 3, -1}, {2, 1, 3} };

linalg::Matrix A(linalg::MatrixFactory<2,3>(v1));
linalg::Matrix B(linalg::MatrixFactory<2,3>(v2));
```

> Matrix addition og multiplikation <br />
```c++
A = A + 1;
A = A * 3;
A = A + B;
```

> Transponer <br />
```c++
A.Transpose();
```

> Rækkeoperationer <br />
```c++
// Skalar multikation af én rækkke
// Række 1 ganges med 0.5
A.row_scalar_multiplication(1, 0.5);

// Læg 3 gange række 2 til række 1
A.row_addition_multilpum(1, 2, 3, 'T');

// Træk 3 gange række 2 fra række 3 
A.row_addition_multilpum(3, 2, 3, '');

// Byt om på række 1 og række 2
A.row_move(1, 2);
```

> Løs ligningssystem <br />
```c++
int v1[3][3] = {{2, -3, 1}, {-1, 1, 1}, {3, 2, 2} };
int v2[3][1] = {{-1}, {2}, {3} };

linalg::Matrix A(linalg::MatrixFactory<3,3>(v1));
linalg::Matrix b(linalg::MatrixFactory<3,1>(v2));

Matrix m = linalg::lup_solve(A, b);
```