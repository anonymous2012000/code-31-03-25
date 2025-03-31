/*
// This code:
(1) Constructs number_of_poly unforgeable polynomals with a random root
(2) Computes the linear combination (i.e., sum) of these unforgeable polynomials
(3) Checks if the linear combination (i.e., sum) of these unforgeable polynomials is valid
(4) It times steps (1)--(3).
*/
//*********************************************************************
#include <NTL/ZZXFactoring.h>
using namespace NTL;
using namespace std;

// Function to convert a vector of ZZ_p coefficients to a ZZ_pX polynomial
ZZ_pX vectorToPolynomial(const vector<ZZ_p>& coeffs) {
  ZZ_pX poly;
  for (long i = 0; i < coeffs.size(); i++) {
    SetCoeff(poly, i, coeffs[i]);
  }
  return poly;
}

// Function to generate a vector of random ZZ_p coefficients with 128-bit values
vector<ZZ_p> generateRandomCoeffs(long degree, int field_size) {
  vector<ZZ_p> coeffs(degree + 1);
  for (long i = 0; i <= degree; i++) {
    ZZ random_coeff = RandomBits_ZZ(field_size); // Generate 128-bit random ZZ
    coeffs[i] = to_ZZ_p(random_coeff);    // Convert ZZ to ZZ_p
  }
  return coeffs;
}

// Function to compute (zeta * pi + gamma) mod p
ZZ_pX construct_unforgeable_polynomial(const ZZ_pX& zeta, const ZZ_pX& pi, const ZZ_pX& gamma) {
  return (zeta * pi + gamma);
}


// Function to check if (numerator - gamma)(r) == 0
bool verify_unforgeable_polynomial(const ZZ_pX& numerator, const ZZ_pX& gamma, const ZZ_p& r) {
    ZZ_pX diff_poly = numerator - gamma;
    ZZ_p eval_result = eval(diff_poly, r);
    return (eval_result == 0);
}


// Function to sum all zp polynomials
ZZ_pX sumAllPolynomials(const vector<ZZ_pX>& zp_set) {
    ZZ_pX sum;
    for (const auto& zp : zp_set) {
        sum += zp;
    }
    return sum;
}

int main() {
    // Set the modulus for the finite field (example: modulus 7)
    ZZ p;
    //p = conv<ZZ>("57896044618658097711785492504343953926634992332820282019728792003956564820063");// 256-bit
    //p = conv<ZZ>("247742461804815734743746154725192129101");// 128-bit-- uncomment this and comment out the next line if the field size should be 128 bits
    p = conv<ZZ>("18446744073709551557");// 64-bit
    int field_size = 64;
    int number_of_poly = 1;
    long degree_pi = 10; // Degree of the polynomial \pi(x)
    ZZ_p::init(p);
    double start_1 = 0, start_2 = 0, start_3 = 0;
    double end_1 = 0, end_2 = 0, end_3 = 0;
    double diff_1 = 0, diff_2 = 0, diff_3 = 0;
    float count_1 = 0, count_2 = 0, count_3 = 0;
    float count_expr = 0;
    int number_of_experiments = 100;
    // Set the degrees of the polynomials
    long degree_zeta = 1; // Degree of the polynomial \zeta(x)
    for(int j = 0; j< number_of_experiments; j++){
      vector<ZZ_pX> zp_set; // Store all zp polynomials
      vector<ZZ_pX> gamma_set; // Store all zp polynomials
      // Generate random coefficients for the polynomials
      ////////////////////////
      ZZ random_r = RandomBnd(p);  // r < p
      ZZ_p r = to_ZZ_p(random_r);
      ZZ_pX zeta;
      SetCoeff(zeta, 1, ZZ_p(1));   // x term
      SetCoeff(zeta, 0, -r);        // constant term is -r
      for(int i = 0; i < number_of_poly; i++){
        vector<ZZ_p> pi_coeffs = generateRandomCoeffs(degree_pi, field_size);
        vector<ZZ_p> gamma_coeffs = generateRandomCoeffs(degree_pi + 1, field_size);
        // Convert the two vectors to ZZ_pX polynomials
        if(i == 0){
          start_1 = clock();
        }
        ZZ_pX pi = vectorToPolynomial(pi_coeffs);
        ZZ_pX gamma = vectorToPolynomial(gamma_coeffs);
        ZZ_pX zp = construct_unforgeable_polynomial(zeta, pi, gamma);
        if(i == 0){
          end_1 = clock();
        }
        if(i == 0){
          diff_1 = end_1 - start_1;
          count_1 += diff_1/(double) CLOCKS_PER_SEC;
        }
        zp_set.push_back(zp);
        gamma_set.push_back(gamma);
      }
      // Compute the sum of all unforgeable polynomials: \theta_1, ..., \theta_n. Note that in this code, \theta is defined as zp.
      start_2 = clock();
      ZZ_pX sum_zp = sumAllPolynomials(zp_set);
      end_2 = clock();
      diff_2 = end_2 - start_2;
      count_2 += diff_2/(double) CLOCKS_PER_SEC;
      start_3 = clock();
      // Compute the sum of all masking polynomials gamma_1, ..., gamma_n
      ZZ_pX sum_gamma = sumAllPolynomials(gamma_set);
      // Perform divisibility check-- i.e., zeta devides sum_zp - sum_gamma, i.e., zeta || zp - gamma, where || denotes divisible.
      // bool is_divisible = verify_unforgeable_polynomial(sum_zp, zeta, sum_gamma);
      //cout<<"\n Random root: "<<r<<endl;
      bool is_valid = verify_unforgeable_polynomial(sum_zp, sum_gamma, r);
      end_3 = clock();
      diff_3 = end_3 - start_3;
      count_3 += diff_3/(double) CLOCKS_PER_SEC;
      //cout << "\n Is valid (Y = 1, N = 0): " << is_valid << endl;
      // long degree_zp = deg(zp);
      // cout << "\n Degree of zp: "<<degree_zp<<endl;
    }
    cout << "\n**********\n" << endl;
    //degree_pi
    cout << "\n Degree_pi: " <<degree_pi<< endl;
    cout << "\n Field_size: " <<field_size<< endl;
    cout << "\n The number of experiments: " <<number_of_experiments<< endl;
    cout << "\n The number of polynomials: " <<number_of_poly<< endl;
    cout << "\n**********\n" << endl;
    cout<<"\n Average runtime (in ms) for Constructing an Unforgeable Polynomial of Degree "<<degree_pi+1<<" is: "<<1000*(count_1/number_of_experiments)<<endl;
    cout << "\n.........\n" << endl;
    cout<<"\n Average runtime (in ms) for Linear Combination of all Unforgeable Polynomial of Degree "<<degree_pi+1<<" is: "<<1000*(count_2/number_of_experiments)<<endl;
    cout << "\n.........\n" << endl;

    cout<<"\n Average runtime (in ms) for Verifying an Unforgeable Polynomial of Degree "<<degree_pi+1<<" is: "<<1000*(count_3/number_of_experiments)<<endl;
    cout << "\n=========" << endl;
    return 0;
}
