
# Dependencies

* GMP: https://gmplib.org/
* NTL: https://www.shoup.net/ntl

# Runnig a Test

* First, clone the above libraries, and the U-PSI file. 
* Second, install the libraries and unzip the file. 
* Third, run the following command lines in order:

      cd code-31-03-25-master
    
      g++ -std=c++11 main-UP.cpp  -o test  -lntl -lgmpxx -lgmp
    
      ./test
