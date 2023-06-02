#ifndef __vta_h
#define __vta_h

int vta_init(void);
int vta_finish(void);
int vta_begin(void);
int vta_end(void);

int 
vta_get(const char* p_tname, const char * const * p_key, const char** p_data);

int 
vta_set(
		const char* p_tname, 
		const char * const * p_key, 
		const char* const* p_data);

int vta_create_table(const char* p_name);

/* verifica se as chaves/dados configurados
 * sao iguais ao esperado,
 * utilizado em dsv para evitar q alquem troque
 * a ordem dos campos */
int vta_assert();

#endif
