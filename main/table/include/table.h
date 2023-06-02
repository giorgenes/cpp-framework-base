#ifndef __libany__table__table_h
#define __libany__table__table_h

#include <libany/stream/stream.h>
#include <libany/memory/buffer.h>

namespace libany {
	namespace table {
		class TableWriter {
			private:
				::libany::stream::Stream& _stream;
				int _nfields;
				int _ntuples;
				int _count;
			public:
				TableWriter(::libany::stream::Stream& stream, int nf, int nt);
				
				void write_field_name(const char*);
				void write_field_value(const char*);
		};

		class Table {
			private:
				int _nfields;
				int _ntuples;
				::libany::memory::Buffer _fields;
				::libany::memory::Buffer _buf;
				
				const char* getfield(int, int) const;
			public:
				void build_from_stream(::libany::stream::Stream&);

				const char* field_name(int) const;
				const char* field_value(int, int) const;

				inline int nfields() const {
					return _nfields;
				};
				
				inline int ntuples() const {
					return _ntuples;
				};

				Table();
				~Table();
		};
	}
}

#endif
