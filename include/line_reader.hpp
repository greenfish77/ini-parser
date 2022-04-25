#ifndef HEADER_LINE_READER_HPP
#define HEADER_LINE_READER_HPP

class line_reader {
public:
	line_reader() {
#ifdef _WIN32
		h = INVALID_HANDLE_VALUE;
#endif
	}
	~line_reader() {close_by_mmf(); close_by_stream();}

public:
	// under windows, to read line by mmf is much faster than by stream.
	// under linux, set use_win32_mmf as false automatically.
	inline void open(_in const std::string& path, _in bool use_win32_mmf){
		close();
	#ifndef _WIN32
		use_win32_mmf = false;
	#endif
		this->use_win32_mmf = use_win32_mmf;
		if (this->use_win32_mmf) open_by_mmf(path);
		else open_by_stream(path);
	}

	inline void close(void){
		eof_ = false;
		if (use_win32_mmf) return close_by_mmf();
		return close_by_stream();
	}

	// return false when eof, and line is meaningless
	inline bool readline(_out std::string& line){
		bool r;
		if (use_win32_mmf) r = readline_by_mmf(line);
		else r = readline_by_stream(line);
		if (not r) eof_ = true;
		return r;
	}

	inline bool eof(void) {return eof_;}
	inline size_t get_mmf_pos(void) { return mmf_first; }
	inline size_t get_mmf_size(void) { return size; }
	inline const void* get_mmf_data(void) { return data; }

protected:
	bool use_win32_mmf = false;
	bool eof_ = false;

protected:
	inline void open_by_stream(_in const std::string& path) {
		// close first
		close_by_stream();
		
		// open
		file.open(path, std::fstream::in);
	}

	inline bool readline_by_stream(_out std::string& line) try {
		// check eof
		if (file.eof()) return false;
		// getline
		std::getline(file, line);
		if (line.empty() and file.eof()) return false;
		return true;
	} catch (...) {
		return false;
	}

	inline void close_by_stream(void) {
		file.close();
	}

	std::fstream file;

protected:
	inline void open_by_mmf(_in const std::string& path) {
#ifdef _WIN32
		// close first
		close_by_mmf();

		h = ::CreateFileA(path.c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (INVALID_HANDLE_VALUE == h) { close(); throw std::runtime_error(F1("fail to ::CreateFile, %0.", path)); }

		size = ::GetFileSize(h, NULL);
		if (INVALID_FILE_SIZE == size) { close(); throw std::runtime_error(F1("fail to ::GetFileSize, %0.", path)); }

		m = ::CreateFileMappingA(h, NULL, PAGE_READONLY, 0, size, NULL);
		if (not m) { close(); throw std::runtime_error(F1("fail to ::CreateFileMappingA, %0.", path)); }

		data = ::MapViewOfFile(m, FILE_MAP_READ, 0, 0, 0);
		if (not data) { close(); throw std::runtime_error(F1("fail to ::CreateFile, %0.", path)); }
#endif
	}

	inline bool readline_by_mmf(_out std::string& line) {
		if (not data) { line.clear(); return false; }
		std::string_view text((const char*)data, size);

		if (mmf_first < text.size()) {
			const auto mmf_second = text.find_first_of('\n', mmf_first);
			if (mmf_first != mmf_second) {
				line = text.substr(mmf_first, mmf_second - mmf_first);
				if (not line.empty() and (line.back() == '\r')) line.pop_back();
			}
			if (mmf_second == std::string_view::npos) { mmf_first = text.size(); return true; }
			mmf_first = mmf_second + 1;
		}
		else { line.clear(); return false; }

		return true;
	}

	inline void close_by_mmf(void) {
#ifdef _WIN32
		if (m) ::CloseHandle(m);
		if (h) ::CloseHandle(h);
#endif
		data = nullptr;
		m = nullptr;
#ifdef _WIN32
		h = INVALID_HANDLE_VALUE;
#else
		h = (void*)((size_t)0-1);
#endif
		mmf_first = 0;
	}
	void* h = (void*)((size_t)0-1);	// HANDLE, INVALID_HANDLE_VALUE
	void* m = nullptr;				// HANDLE
	unsigned long size = 0;			// DWORD
	void* data = nullptr;
	size_t mmf_first = 0;
};

#endif // HEADER_LINE_READER_HPP
