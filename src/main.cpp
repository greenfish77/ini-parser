#include "headers.hpp"

int main(int argc, const char** argv) try {
	// log initialize.
	init2(true);

	info("***********");
	info("* started *");
	info("***********");

	if (argc != 2) throw std::exception("ini_parser.exe <ini_file_path>.");

	return 0;
} catch (std::exception& e) {
	std::string w = e.what();
	error("<yellow>" + w + "</yellow>", true);
	return 1;
}

// under windows, memory leak can be reported.
#ifdef _WIN32
static struct DUMP_MEMORY_LEAK {DUMP_MEMORY_LEAK() {_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF|_CRTDBG_LEAK_CHECK_DF);}} g_dml;
#endif
