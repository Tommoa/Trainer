#include <ostream>

namespace errors { 
	enum class types {
		invalid_set,
		invalid_file_type
	};
	void dispatcher(std::ostream& out);
}
