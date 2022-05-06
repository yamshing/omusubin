
#include "omusubin.h" 
#include <cmath>
#include <sstream>
 
#ifdef __MINGW64__
#include <windows.h>
#elif __APPLE__
#include <mach-o/loader.h> 
 
#else
#include <elf.h>
#endif


void Omusubin::ParseConf(std::ifstream& conf_fs){
	std::string line;
	bool file_start = false;
	m_file_id_vec = {};
	m_file_type_vec = {};
	m_file_path_vec = {};
	 
	 
	while (std::getline(conf_fs,line))
	{

		if (line.find("FILE") == 0) {
			file_start = true;
		}else if (line.find("END_FILE") == 0) {
			file_start = false;
		}


		 
		if (file_start) {
			std::istringstream iss(line);
			std::string a, b, c;
			while(iss >> a >> b >> c){
				m_file_id_vec.push_back(a);
				m_file_type_vec.push_back(b);
				m_file_path_vec.push_back(c);
				 
				//std::cout << "a << ',' << b << ',' << c (in omusubin.cpp) " << a << ',' << b << ',' << c << std::endl;
			}
		}
		 
		if (line.find("TARGET") == 0) {
			 
			std::istringstream iss(line);
			std::string a, b;
			while(iss >> a >> b){
				m_target_file_name = b;
			}
		}
		
	}
	for (auto file_id: m_file_path_vec) {
		//std::cout << "file_id (in omusubin.cpp) " << file_id << std::endl;
	}
	 
	//std::cout << "m_target_file_name (in omusubin.cpp) " << m_target_file_name << std::endl;
	 
}
void Omusubin::InsertFileToTarget(){
	 
	std::ifstream target_fs(m_target_file_name);
	if (target_fs.good()) {
		 
		std::ifstream is( m_target_file_name, std::ios::in | std::ios::binary );
		std::ofstream os( m_target_file_name + "_omusubin.exe", std::ios::out | std::ios::binary );
		os << is.rdbuf();
		os.close();

		double file_size_byte = GetExeSize(m_target_file_name);
		 
		if (file_size_byte == 0) {
			std::cout << "exe size error  " << std::endl;
			return;
		}
		 
		//std::cout << "file_size_byte (in omusubin.cpp) " << m_target_file_name << ',' <<  file_size_byte << std::endl;
		 
		int file_size_aligned = ceil(file_size_byte / 16.0) * 16;
		 
		int diff_byte = file_size_aligned - file_size_byte;
		 
		//std::cout << "diff_byte (in omusubin.cpp) " << diff_byte << std::endl;
		 
		std::ofstream os_app( m_target_file_name  + "_omusubin.exe", std::ios::app | std::ios::binary );
		 
		int input_zero = 0;
		 
		for (int i = 0; i < diff_byte; ++i) {
			os_app.write(reinterpret_cast<char*>(&input_zero), 1);
		}
		 
		os_app.write("OMSBN",5);
		 
		for (int i = 0; i < 11; ++i) {
			os_app.write(reinterpret_cast<char*>(&input_zero), 1);
		}
		for (int i = 0; i < m_file_id_vec.size(); ++i) {
			std::string file_path_str = m_file_path_vec[i];
			 
			std::vector<std::string> file_split_vec;
			 
			std::istringstream f(file_path_str);
			std::string s;
			 
			while (getline(f, s, ',')) {
				file_split_vec.push_back(s);
			}
			 
			for (std::string file_path : file_split_vec) {
				{
					std::string file_id = m_file_id_vec[i];

					//std::cout << "file_id (in omusubin.cpp) " << file_id << std::endl;

					int file_id_size = file_id.size();
					int padding_size = 0;
					if (file_id_size <= 16) {
						padding_size = 16 - file_id_size;
					}
					os_app.write(file_id.c_str(),file_id_size);
					for (int j = 0; j < padding_size; ++j) {
						os_app.write(reinterpret_cast<char*>(&input_zero), 1);
					}
				}

				{
					std::string file_type = m_file_type_vec[i];

					//std::cout << "file_type (in omusubin.cpp) " << file_type << std::endl;

					int file_type_size = file_type.size();
					int padding_size = 0;
					if (file_type_size <= 16) {
						padding_size = 16 - file_type_size;
					}
					os_app.write(file_type.c_str(),file_type_size);
					for (int j = 0; j < padding_size; ++j) {
						os_app.write(reinterpret_cast<char*>(&input_zero), 1);
					}
				}

				std::ifstream file_is( file_path, std::ios::in | std::ios::binary );

				if (file_is.good()) {

					//ifstr.seekg( 0, std::ios_base::end );

					std::stringstream file_ss;
					file_ss <<  file_is.rdbuf();

					uint64_t file_size = file_is.tellg();

					int file_padding_size = 16 - ((file_size + 8) % 16);

					os_app.put((file_size>>56)& 0xFF);
					os_app.put((file_size>>48)& 0xFF);
					os_app.put((file_size>>40)& 0xFF);
					os_app.put((file_size>>32)& 0xFF);
					os_app.put((file_size>>24)& 0xFF);
					os_app.put((file_size>>16)& 0xFF);
					os_app.put((file_size>>8)& 0xFF);
					os_app.put((file_size )& 0xFF);

					os_app << file_ss.str();

					for (int j = 0; j < file_padding_size; ++j) {
						os_app.write(reinterpret_cast<char*>(&input_zero), 1);
					}

				}
			} //end for file_split_vec
		}
		 
	}else{
		std::cout << "target does not exists "  << std::endl;
	}
	 
	 
} 
 
bool Omusubin::InsertFromConf(){
	std::string conf_name = "./omusubin.conf";
	 
	InsertFromConf(conf_name);
}
bool Omusubin::InsertFromConf(std::string& conf_name){
	
	 
	std::ifstream conf_fs(conf_name);
	 
	if (conf_fs.good()) {
		 
		std::cout << "conf_name ok (in omusubin.cpp) " << conf_name << std::endl;
		ParseConf(conf_fs);
		InsertFileToTarget();
		 
		return true;
	}else{
		std::cout << "no conf file" << std::endl;
		return false;
	}

}

bool Omusubin::Insert(std::string& target_file_name, std::vector<std::string>& file_id_vec, std::vector<std::string>& file_type_vec, std::vector<std::string>& file_path_vec){
	 
	m_target_file_name = target_file_name; 
	m_file_id_vec = file_id_vec;
	m_file_type_vec = file_type_vec;
	m_file_path_vec = file_path_vec;
	 
	InsertFileToTarget();
	return true;
	 
}
 
double Omusubin::GetExeSize(std::string& target_file_name)
{
	double index = 0; 
	 
#ifdef __MINGW64__
	IMAGE_NT_HEADERS64* header;
	IMAGE_DOS_HEADER* dosheader;
	 
	std::ifstream is_header( target_file_name, std::ios::in | std::ios::binary );
	std::istreambuf_iterator<char>it_header(is_header);
	int size = 4096;
	 
	unsigned char buf[size];
	 
	for (int i = 0; i < size; ++i) {
		buf[i] = *it_header;
		it_header ++;
	}
	
	dosheader =  (IMAGE_DOS_HEADER*)buf;
	if (dosheader->e_magic!=IMAGE_DOS_SIGNATURE) {
		std::cout << "no dos header " << std::endl;
		return -1;
	}
	header = (IMAGE_NT_HEADERS64*)(buf + dosheader->e_lfanew);
	 
	if (header->Signature != IMAGE_NT_SIGNATURE) {
		std::cout << "no nt header " << std::endl;
		return -1;
	}
	IMAGE_SECTION_HEADER* sectiontable = (IMAGE_SECTION_HEADER*)((BYTE*)header + sizeof(IMAGE_NT_HEADERS64));
	DWORD maxpointer = 0;
	double exesize = 0;
	 
	//std::cout << "header->FileHeader.NumberOfSections (in bin_win.cpp) " << header->FileHeader.NumberOfSections << std::endl;
	 
	for(int i = 0; i < header->FileHeader.NumberOfSections; ++i) {
		//std::cout << "sectiontable->Name (in bin_win.cpp) " << sectiontable->Name << std::endl;
		if(sectiontable->PointerToRawData > maxpointer) {
			maxpointer = sectiontable->PointerToRawData;
			//std::cout << "sectiontable->PointerToRawData (in bin_win.cpp) " << sectiontable->PointerToRawData << std::endl;
			//std::cout << "sectiontable->SizeOfRawData (in bin_win.cpp) " << sectiontable->SizeOfRawData << std::endl;
			exesize = sectiontable->PointerToRawData + sectiontable->SizeOfRawData;
		}
		sectiontable++;
	}
	is_header.close(); 
	 
	index = exesize;
	 
#elif __APPLE__
	struct mach_header_64* mach_header; 
	 
	std::cout << "mach header ready (in omusubin.cpp) "  << std::endl;
	 
	std::ifstream is_header( target_file_name, std::ios::in | std::ios::binary );
	std::istreambuf_iterator<char>it_header(is_header);
	int size = 64;
	unsigned char buf[size];
	for (int i = 0; i < size; ++i) {
		buf[i] = *it_header;
		it_header ++;
	}
	mach_header = (mach_header_64 *)buf;
	 
	std::cout << "mach_header->ncmds (in omusubin.cpp) " << mach_header->ncmds << std::endl;
	std::cout << "mach_header->sizeofcmds (in omusubin.cpp) " << mach_header->sizeofcmds << std::endl;
	 
	is_header.close();
	 
#else

	Elf64_Ehdr *ehdr;
	std::ifstream is_header( target_file_name, std::ios::in | std::ios::binary );
	std::istreambuf_iterator<char>it_header(is_header);
	int size = 64;
	unsigned char buf[size];
	for (int i = 0; i < size; ++i) {
		buf[i] = *it_header;
		it_header ++;
	}
	
	ehdr = (Elf64_Ehdr *)buf;
	is_header.close();
	 
	int section_header_offset = ehdr->e_shoff;
	int section_header_size = ehdr->e_shentsize;
	int section_header_num = ehdr->e_shnum;
	 
	index = (section_header_offset + section_header_num * section_header_size);
	 
	//std::cout << "get exe size (in omusubin.cpp) " << index << std::endl;
	 
#endif

	return index;
}
 

#ifdef __MINGW64__

#elif __APPLE__
 
#else
 
void Omusubin::dump_ehdr(Elf64_Ehdr *ehdr){
	int i;
	printf("  ehdr->e_ident = ");
	for (i = 0; i < EI_NIDENT; i++) {
		printf("%02x ", ehdr->e_ident[i]);
	}
	printf("\n");
	DUMP(ehdr->e_type);
	DUMP(ehdr->e_machine);
	DUMP(ehdr->e_version);
	DUMP(ehdr->e_entry);
	DUMP(ehdr->e_phoff);
	DUMP(ehdr->e_shoff);
	DUMP(ehdr->e_flags);
	DUMP(ehdr->e_ehsize);
	DUMP(ehdr->e_phentsize);
	DUMP(ehdr->e_phnum);
	DUMP(ehdr->e_shentsize);
	DUMP(ehdr->e_shnum);
	DUMP(ehdr->e_shstrndx);
	printf("\n");
}
#endif

 
bool Omusubin::Load(std::string& target_file_name){
	 
	unsigned int index = GetExeSize(target_file_name);
	if (index % 16 != 0) {
		//header offset is not at the head of the line
		index += 8;
	} 
	 
	std::ifstream is_index( target_file_name, std::ios::in | std::ios::binary );
	std::istreambuf_iterator<char>it_index(is_index), it_end;
	for (double i = 0; i < index; ++i) {
		it_index ++;
	}
	bool omusubi_not_found = (it_index == it_end);
	if (omusubi_not_found) {
		return false;
	}
	 
	std::vector<unsigned char> buffer(it_index, {});
	std::stringstream ss;
	std::vector<unsigned char> val_row_vec;
	 
	std::vector<std::string> id_vec{};
	 
	int type_index = 0;
	 
	//std::cout << "buffer[0] (in omusubin.cpp) " << buffer[0] << buffer[1] << buffer[2] << buffer[3] << buffer[4] << std::endl;

	if (buffer[0] == 'O' && buffer[1] == 'M' && buffer[2] == 'S' && buffer[3] == 'B' && buffer[4] == 'N') {
		//std::cout << "omusubin found (in omusubin.cpp) " << std::endl;
	}
	 
	int buf_i = 16;
	int data_i = 0;
	int max_data_num = 9999;
	 
	for (int j = 0; j < max_data_num ; ++j) {
		std::stringstream id_ss;
		std::stringstream type_ss;
		 
		for (int i = 0; i < 16; ++i) {
			if (buffer[buf_i + i]!=0) {
				//id_vec[data_i].push_back(buffer[buf_i + i]);
				id_ss << buffer[buf_i + i];
			}
		}
		buf_i += 16;
		 
		for (int i = 0; i < 16; ++i) {
			if (buffer[buf_i + i]!=0) {
				type_ss << buffer[buf_i + i];
			}
		}
		 
		buf_i += 16;
		 
		//std::cout << "id load (in omusubin.cpp) " << id_ss.str() << std::endl;
		//std::cout << "id load (in omusubin.cpp) " << type_ss.str() << std::endl;
		 
		id_vec.push_back(id_ss.str());

		uint64_t data_size;
		 
		data_size = (( (uint64_t)(buffer[buf_i + 7]) & 0xFF) | 
				( (uint64_t)(buffer[buf_i + 6]) & 0xFF)<<8  | 
				( (uint64_t)(buffer[buf_i + 5]) & 0xFF)<<16 | 
				( (uint64_t)(buffer[buf_i + 4]) & 0xFF)<<24 | 
				( (uint64_t)(buffer[buf_i + 3]) & 0xFF)<<32 | 
				( (uint64_t)(buffer[buf_i + 2]) & 0xFF)<<40 |
				( (uint64_t)(buffer[buf_i + 1]) & 0xFF)<<48 |
				( (uint64_t)(buffer[buf_i + 0]) & 0xFF)<<56) ;
		 
		buf_i += 8;
		std::vector<unsigned char> char_val_vec{};
		 
		for (int i = 0; i < data_size; ++i) {
			char_val_vec.push_back(buffer[buf_i + i]);
		}
		 
		//val_vec.push_back(char_val_vec);
		 
		if (m_bin_data_map.find(id_ss.str()) != m_bin_data_map.end()) {
			 
			m_bin_data_map[id_ss.str()].insert(m_bin_data_map[id_ss.str()].end(), char_val_vec.begin(), char_val_vec.end());
			 
		}else{
			m_bin_data_map[id_ss.str()] = char_val_vec;
		}
		 
		 
		m_file_extention_map[id_ss.str()] = type_ss.str(); 
		 
		buf_i += data_size;
		 
		int space_i = 0;
		for (int i = 0; i < 16; ++i) {
			if (buffer[buf_i + i] == 0) {
				space_i ++;
			}else{
				break;
			}
		}
		 
		buf_i += space_i; 
		 
		if (buf_i >= buffer.size()) {
			break;
		}
		
	}
	
	/*for (auto val : val_vec) {
		std::stringstream ss;
		for (unsigned int v : val) {
			//ss << (char)(v);
			printf("%x ", v);
		}
		//std::cout << "ss.str() (in omusubin.cpp) " << ss.str() << std::endl;
	}
	*/
	 
	return true;
	 
}

