int main()
{
	char* conf;

	conf = conf_get();
	if(!conf_initialized) {
		/* initialize conf */
	}

	conf_update();
	if(conf_updated()) {
		conf_release();
		conf = conf_get();
		if(!conf_initialized) {
			/* initialize conf */
		}
	}

	conf_release();
}

