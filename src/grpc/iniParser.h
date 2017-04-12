#ifndef INI_PARSER_H_
#define INI_PARSER_H_

#include <iostream>
#include <exception>
#include <fstream>
#include <map>

/**
 * \class iniParser
 * \brief parse ini file into a handler
 *
 * please note that key should not have whitespace
 * valid format:
 * [section]
 * ; comment format 1
 * # comment format 2
 * key1=val1
 * key2  =val2
 * key3=  val3
 * key4 = val4
 *
 */

static std::string trim(std::string buf) {
	while ( buf.length() > 0 && buf[0] == ' ') {
		buf.erase(0, 1);
	}
	while ( buf.length() > 0 && buf.back() == ' ') {
		buf.pop_back();
	}
	return buf;
}

class iniParser {
	public:
		typedef std::map<std::string, std::string> keyValue;
		enum readState {
			lineBegin,
			lineReading,
			lineComment,
		};
		enum procesState {
			processNone,
			processing,
			processEnd,
		};
		iniParser() {}
		iniParser(const char* filename, const char* _section) {
			if ( _section != NULL ) {
				section = _section;
			}
			loadIni(filename);
		}
		void loadIni( const char* filename ) {
			process_state = processNone;
			read_state = lineBegin;
			std::ifstream fs;
			fs.open(filename, std::ifstream::in);
			if ( fs.fail() ) {
				std::cerr << "Fail_to_Open_File: " << filename << std::endl;
				return;
			}
			char c;
			std::string buff;
			while ( fs.get(c) && process_state != processEnd ) {
				//		std::cout << c;
				if ( c == '\n' || c == '\r' ) {
					process(buff);
					read_state = lineBegin;
				}
				else if ( read_state == lineComment ) {
					continue;
				}
				else if ( c == ' ' && read_state == lineBegin ) {
					continue;
				}
				else if ( ( c == '#' || c == ';' ) && read_state == lineBegin ) {
					read_state = lineComment;
				}
				else {
					read_state = lineReading;
					buff += c;
				}
			}
		}
		std::string get(const char *key, std::string _default = "") {
			keyValue::iterator it = key_val.find(key);
			if ( it == key_val.end() ) {
				return _default;
			}
			else {
				return it->second;
			}
		}
	private:
		void process(std::string &buf) {
			if ( buf.empty() ) return;
			//	std::cout << "parsing:" << buf.c_str() << std::endl;
			std::string new_section;
			if ( buf[0] == '[' ) {
				if ( process_state == processing ) {
					process_state = processEnd;
					return;
				}
				size_t i = 1;
				size_t len = buf.length();
				while ( i < len ) {
					char c = buf[i++];
					if ( c == ']' ) break;
					new_section += c;
				}
				// XXX I should check i and len
				//		std::cout << "\033[1;33m" << "section: \033[0m" << new_section << std::endl;
				if ( new_section.compare(section) == 0 ) {
					process_state = processing;
				}
			}
			else if ( process_state == processing ) {
				processKeyVal(buf);
			}
			buf.clear();
		}
		void processKeyVal(std::string &buf) {
			std::size_t found = buf.find('=');
			if ( found == buf.npos ) {
				std::cerr << "invalid line " << buf << std::endl;
				return;
			}
			std::string key = trim(buf.substr(0, found - 1));
			try {
				std::string val = trim(buf.substr(found + 1));
				//		std::cout << "key: " << key << ", val: " << val << std::endl;
				key_val[key] = val;
			}
			catch ( const std::exception& err ) {
				;
			}
		}
		keyValue key_val;
		readState read_state;
		procesState process_state;
		std::string section;
};


#endif
