
# Dependencies

* GMP: https://gmplib.org/
* NTL: https://www.shoup.net/ntl


# Using the library

* First, clone the above libraries. 
* Second, install the libraries and unzip the file. 
* Third, run the following command lines in order:

      cd code-31-03-25-main
    
      g++ -std=c++11 main-UP.cpp  -o test  -lntl -lgmpxx -lgmp
    
      ./test

  Note that to run `main-UP-wRR.cpp`, use the above procedure with the difference being that we replace

       g++ -std=c++11 main-UP.cpp  -o test  -lntl -lgmpxx -lgmp
  
  with

       g++ -std=c++11 main-UP-wRR.cpp  -o test  -lntl -lgmpxx -lgmp


  The libraries (i.e., `main-UP.cpp` and `main-UP-wRR.cpp`) provides you with the options to select different:

  * field size, i.e., 64 or 128
  * number of polynomials (denoted as `number_of_poly`)
  * degree of polynoials (denoted as `degree_pi`)
  * number of experiments (denoted as `number_of_experiments`)
 
Currently, the coefficients of the polynomial `pi` are selected uniformly at random. If you wish to use your own fixed polynomial `pi`, you can replace the current one with your custom choice. However, ensure that it is defined as a polynomial of type `ZZ_pX`, which is compatible with the NTL library. 

# Difference between `main-UP.cpp` and `main-UP-wRR.cpp`

The library `main-UP.cpp` uses the unforgeable polynoimals where the verification is based on polynomial factoriztion, whereas `main-UP-wRR.cpp` implements the unforgeable polynoimals with a random root whose verification is more efficient and is based on polynoimal evalaation. 
