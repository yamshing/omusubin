#ifndef OMUSUBIN_H
#define OMUSUBIN_H 
#include <iostream>
#include <fstream>
#include <string> 
#include <vector> 
#include <iomanip>
 
#include <unordered_map>

#ifdef __MINGW64__

#else

#include <elf.h>
 
#endif


class Omusubin
{
public:
	Omusubin (){};
	virtual ~Omusubin (){};
	 
	bool InsertFromConf();
	 
	bool InsertFromConf(std::string& conf_name);
	 
	bool Insert(std::string& target_file_name, std::vector<std::string>& file_id_vec, std::vector<std::string>& file_type_vec,  std::vector<std::string>& file_path_vec);

	bool Load(std::string& target_file_name);
	 
	void GetBinDataByKey(std::string& key, std::vector<unsigned char>& out_vec)
	{
		out_vec = m_bin_data_map[key];
	}
	 
	void GetBinDataMap(std::unordered_map<std::string, std::vector<unsigned char>>& out_map)
	{
		out_map = m_bin_data_map;
	}
	 
	void GetFileExtensionByKey(std::string& key, std::string& file_extension)
	{
		file_extension = m_file_extention_map[key];
	}
	
	 
	 
private:

#ifdef __MINGW64__

#else
	void dump_ehdr(Elf64_Ehdr *ehdr);
#endif
	 
	void ParseConf(std::ifstream& conf_fs);
	void InsertFileToTarget();
	double GetExeSize(std::string& target_file_name);
	 
	std::vector<std::string> m_file_id_vec{};
	std::vector<std::string> m_file_type_vec{};
	std::vector<std::string> m_file_path_vec{};
	 
	std::unordered_map<std::string, std::vector<unsigned char>> m_bin_data_map{};
	std::unordered_map<std::string, std::string> m_file_extention_map{};
	 
	std::string m_target_file_name;
	 
};

#define DUMP(x) do {printf("  " #x " = %u(0x%x)\n", (uint32_t)x, (uint32_t)x);} while(0);


#endif

