#ifndef TEST_UTIL_H
#define TEST_UTIL_H 
 
#include <iostream> 
#include <fstream> 
#include <iomanip> 
#include <vector>
#include <cmath>

static const char* TEST_RES_FILE_PATH = "test_res.log";

template<class T, class S>
bool equal(T a, S b)
{
	return (fabs(a - b) <= 0.001);
}
 
template<class T, class S>
void ASSERT(T expected, S actual, std::string comment) {
	 
	std::ofstream ofs(TEST_RES_FILE_PATH, ofs.out | ofs.app );
	 
	if (equal(expected,actual)) {
		 
		std::cout << "\033[1;32m "<< "OK--------" << comment   << "\033[0m" <<std::endl;
		ofs <<  "OK--------" << comment  <<std::endl;
		 
	}else{
		 
		int w = 20;
		std::cout << std::setprecision(15)  << "\033[1;31m " << "NO--------" << std::setw(w) << comment  << std::setw(w) << " expected :" << std::setw(w) << expected << std::setw(w) << " actual: " << std::setw(w) <<  actual << std::setw(w) << "\033[0m" << std::endl;
		ofs << std::setprecision(15)  <<  "NO--------" << std::setw(w) << comment  << std::setw(w) << " expected :" << std::setw(w) << expected << std::setw(w) << " actual: " << std::setw(w) <<  actual << std::setw(w) << std::endl;
		 
	}
	 
}

template<class T>
bool equal(std::vector<T>& a, std::vector<T>& b)
{
	bool res = true;
	if (a.size() != b.size()) {
		res = false;
	}
	for (int i = 0; i < a.size(); ++i) {
		if (!equal(a[i],b[i])) {
			res = false;
		}
	}
	 
	return res;
}
 
template<class T>
void ASSERT(std::vector<T>& expected, std::vector<T>& actual, std::string comment)
{
	std::ofstream ofs(TEST_RES_FILE_PATH, ofs.out | ofs.app );
	 
	if (equal(expected,actual)) {
		std::cout << "\033[1;32m "<< "OK--------" << comment   << "\033[0m" <<std::endl;
		ofs << "OK--------" << comment    <<std::endl;
	}else{
		std::cout << "\033[1;31m " << "NO--------" << comment  <<  "\033[0m" << std::endl;
		ofs <<  "NO--------" << comment  <<   std::endl;
	}
}
 
inline void ASSERTSTR(std::string expected, std::string actual, std::string comment)
{

  std::ofstream ofs(TEST_RES_FILE_PATH, ofs.out | ofs.app );

  if (expected == actual) {

    std::cout << "\033[1;32m "<< "OK--------" << comment   << "\033[0m" <<std::endl;
    ofs <<  "OK--------" << comment    <<std::endl;

  }else{

    int w = 20;
		std::cout <<  "\033[1;31m " << "NO--------" <<  comment   << " expected##" <<  expected <<  "##actual##" <<   actual << "##" <<  "\033[0m" << std::endl;
		ofs <<   "NO--------" <<  comment   << " expected##" <<  expected <<  "##actual##" <<   actual  << "##" <<  std::endl;
		 
  }
}
 

#endif
