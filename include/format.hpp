#ifndef HEADER_FORMAT_HPP
#define HEADER_FORMAT_HPP

// example)
// F1("hello, %0", "world")
// F2("hello, %0, %1, %0", "world", 3.14)
#define F1(fmt, a)				f(fmt, {a})
#define F2(fmt, a, b)			f(fmt, {a, b})
#define F3(fmt, a, b, c)		f(fmt, {a, b, c})
#define F4(fmt, a, b, c, d)		f(fmt, {a, b, c, d})
#define F5(fmt, a, b, c, d, e)	f(fmt, {a, b, c, d, e})

// "17.000000"     -> "17"
// "171474.457100" -> "171474.4571"
inline std::string dbl_to_string(_in double v) {
	std::string ret = std::to_string(v);
	ret.erase(ret.find_last_not_of("0") + 1);
	if (ret.back() == '.') ret.pop_back();
	return ret;
}

inline int atoi_ex(_in const char* s, _out size_t& len) {
	int r = 0;
	for (len=0; s[len] != '\0'; len++) {
		if (('0' <= s[len]) and (s[len] <= '9')) {
			// number.
			r = r * 10 + s[len] - '0';
			continue;
		}
		break;
	}
	return r;
}

// format message.
// r = f("[%0][%1][%0][%%1][%9]", {1, "hello"}) => "[1][hello][1][%1][%9]"
inline std::string f(_in const std::string& fmt, _in const std::vector<std::variant<std::monostate,int,unsigned int,int64_t,size_t,double,std::string>>& params) {
	const char* text = nullptr;
	size_t i = 0;
	size_t len = 0;
	size_t variant_index = 0;
	size_t tag_length = 0;
	int tag_index = 0;
	std::string r;

	if (fmt.empty()) return "";

	text = fmt.c_str();
	len  = fmt.length();
	for (i=0; i<len; i++) {
		tag_index = -1;
		if ((text[i] == '%') and (text[i+1] != '%')) {
			tag_index = atoi_ex(&text[i+1], tag_length);
			if ((tag_index < 0) or (tag_index >= (int)params.size())) {
				// out of tag_index.
				tag_index = -1;
			}

			if (tag_length == 0) {
				// format error.
				tag_index = -1;
			}
		} else if ((text[i] == '%') and (text[i+1] == '%')) {
			r += "%";
			i += 1;
			continue;
		}

		if (tag_index >= 0) {
			// tag found.
			variant_index = params[tag_index].index();
			try {
				if      (1 == variant_index) r += std::to_string(std::get<1>(params[tag_index]));	// <int>
				else if (2 == variant_index) r += std::to_string(std::get<2>(params[tag_index]));	// <unsigned int>
				else if (3 == variant_index) r += std::to_string(std::get<3>(params[tag_index]));	// <int64_t>
				else if (4 == variant_index) r += std::to_string(std::get<4>(params[tag_index]));	// <size_t>
				else if (5 == variant_index) r += dbl_to_string	(std::get<5>(params[tag_index]));	// <float>   9.900000 -> 9.9
				else if (6 == variant_index) r +=				 std::get<6>(params[tag_index]);	// <string>
			} catch (const std::bad_variant_access&) {}

			// next.
			i += tag_length;
		} else {
			r += text[i];
		}
	}

	return r;
}

#endif // HEADER_FORMAT_HPP
