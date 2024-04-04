module;

#include "../include/logfmtxx.hpp"

export module logfmtxx;

export namespace logfmtxx {
	export using logfmtxx::level;
	export using logfmtxx::logger;
	export using logfmtxx::field;

	export namespace details {
		export using logfmtxx::details::as_string_trait;
		export using logfmtxx::details::serializable_trait;
		export using logfmtxx::details::record;
		export using logfmtxx::details::serialize;
	};
}