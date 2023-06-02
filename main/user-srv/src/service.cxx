#include <libany/usersrv/user.h>

class Client : public ::libany::rcpp::Client {
	public:
		virtual ::libany::rcpp::Transaction* new_transaction();
		virtual void begin();
		virtual void end();
		virtual void exec();

		virtual ~Client();
};

using namespace ::libany::usersrv;

::libany::rcpp::Transaction* Client::new_transaction()
{
	return 0;
}

void Client::begin()
{
}

void Client::end()
{
}

void Client::exec()
{
}

Client::~Client()
{
}

::libany::rcpp::Client* User::new_client()
{
	return new Client();
}
