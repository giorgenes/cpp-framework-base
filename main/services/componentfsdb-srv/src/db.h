#ifndef __db_h
#define __db_h

namespace libany {
	namespace componentfsdbsrv {
		class DB {
			private:
				char __root[1024];
			public:
				void commit();
				void begin_transaction();

				DB();
				~DB();

				inline const char* root() const {
					return __root;
				};
		};
	}
}

#endif
