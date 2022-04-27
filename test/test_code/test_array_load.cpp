#include "omusubin.h"
#include "test_util.h"
 
#include "test.h"
#include <iostream>
#include <string>
 
void test_array_load_bin()
{
	Omusubin omusubin;
	 
#ifdef __MINGW64__

	//std::string test_conf = "./test_asset/test1_omusubin_win.conf";
	//std::string target_bin_file_name = "./test_asset/a.exe_omusubin.exe";
#else

	std::string test_conf = "./test_asset/test_array_load_omusubin.conf";
	std::string target_bin_file_name = "./test_asset/a.out_omusubin.exe";
#endif

	std::cout << "test array load (in test_array_load.cpp) "  << std::endl;
	 
	omusubin.InsertFromConf(test_conf);
	 
	
	omusubin.Load(target_bin_file_name);
	std::vector<unsigned char> out_bin;
	std::string data_name = "script";
	 
	omusubin.GetBinDataByKey(data_name, out_bin);
	 
	std::stringstream ss;
	 
	for (int i = 0; i < out_bin.size(); ++i) {
		ss << out_bin[i];
	}
	std::string text = ss.str();
	 
	std::string expected = "abcdefg\nhijklmn\n\n";
	 
	ASSERTSTR(expected, text, "array text load ok");
	 
}
 
