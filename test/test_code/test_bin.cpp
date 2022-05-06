#include "omusubin.h"
#include "test_util.h"
 
#include "test.h"
#include <iostream>
#include <sstream>
#include <string>
 
void test_save_load_bin()
{
	Omusubin omusubin;
	 
#ifdef __MINGW64__

	std::string test_conf = "./test_asset/test1_omusubin_win.conf";
	std::string target_bin_file_name = "./test_asset/a.exe_omusubin.exe";
#elif __APPLE__	
	 
	std::string test_conf = "./test_asset/test1_omusubin_mac.conf";
	std::string target_bin_file_name = "./test_asset/a_mac.out_omusubin.exe";
	 
#else

	std::string test_conf = "./test_asset/test1_omusubin.conf";
	std::string target_bin_file_name = "./test_asset/a.out_omusubin.exe";
#endif

	omusubin.InsertFromConf(test_conf);
	 
	/*omusubin.Load(target_bin_file_name);
	std::vector<unsigned char> out_bin;
	std::string data_name = "text1";
	 
	omusubin.GetBinDataByKey(data_name, out_bin);
	 
	std::stringstream ss;
	 
	for (int i = 0; i < out_bin.size(); ++i) {
		ss << out_bin[i];
	}
	std::string text = ss.str();
	 
	std::string expected = "abcdefg\n";
	 
	ASSERTSTR(expected, text, "text load ok");
	*/
	 
}
 


