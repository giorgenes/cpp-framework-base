#include <libany/cgi/cgi.h>
#include <libany/cgi/stdcgi.h>
#include <libany/stdstream/urlencode.h>
#include <stdio.h>
#include <string.h>

class MyCGI : public ::libany::cgi::CGI {
	public:
		void write_header(const ::libany::cgi::CGIEnv&,
				::libany::cgi::Header&);
		void write_body(const ::libany::cgi::CGIEnv&);
};

void MyCGI::write_header(
		const ::libany::cgi::CGIEnv& env, ::libany::cgi::Header& header)
{
	header.write_content_type("text/xml");
}

void MyCGI::write_body(const ::libany::cgi::CGIEnv& env)
{
	int len;
	char buf[1024];
	::libany::stdstream::IUrlEncodeStream in(env.io);

	if(!env.path_info) {
		len = sprintf(buf, "<error>\n");
		env.io.write(buf, len);		
		len = sprintf(buf, "</error>\n");
		env.io.write(buf, len);		
		return;
	}
	if(strcmp(env.path_info, "/add")==0) {
		FILE* fp;
		char name[1024];

		while(in.get_next_pair()) {
			if(strcmp(in.key(), "name")==0) {
				len = in.read(name, sizeof(name));
				name[len] = 0;
			}
		}
		fp = fopen("krushitas.txt", "a");
		if(fp) {
			fprintf(fp, "%s\n", name);
			fclose(fp);
		}
		goto aqui;
	}
	else if(strcmp(env.path_info, "/list")==0) {
aqui:
		FILE* fp;
		char aux[1024];

		len = sprintf(buf, "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n");
		env.io.write(buf, len);		

		len = sprintf(buf, "<?xml-stylesheet type=\"text/xsl\" href=\"../krushitas.xsl\"?>\n");
		env.io.write(buf, len);		
		
		len = sprintf(buf, "<krushitas>\n");
		env.io.write(buf, len);
		
		fp = fopen("krushitas.txt", "r");
		if(fp) {
			while(fgets(aux, sizeof(aux), fp)!=NULL) {
				len = sprintf(buf, "<krushita>");
				env.io.write(buf, len);

				len = strlen(aux);
				env.io.write(aux, len-1);

				len = sprintf(buf, "</krushita>\n");
				env.io.write(buf, len);

			}
			fclose(fp);
		}
		len = sprintf(buf, "</krushitas>\n");
		env.io.write(buf, len);		
	}
	else {
		len = sprintf(buf, "<error>\n");
		env.io.write(buf, len);		
		len = sprintf(buf, "</error>\n");
		env.io.write(buf, len);		
	}
}


int main(int argc, char** argv)
{
	MyCGI c;
	::libany::cgi::StdCGI cgi(c);

	cgi.run(argc, argv);

	return 0;
}


