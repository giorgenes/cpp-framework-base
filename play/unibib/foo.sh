#!/bin/bash

> headers.txt
curl -kL -b headers.txt -D headers.txt -G 'http://biblioteca.unisinos.br/autenticador/autenticador.php'
curl -vk -b headers.txt -d 'vinculo=935247&senha=110861&destinoSucesso=http%3A%2F%2Fbiblioteca.unisinos.br%2Fautenticador%2Fautenticador.php%3Fflag%3Dindex.php&destinoCancelamento=http%3A%2F%2Fbibiloteca.unisinos.br%2Fbiblioteca_s%2Fphp%2Fsair.php&FORMULARIOAUTENTICACAO=1' \
	'https://autenticador.unisinos.br/AutenticadorClientes/Autenticar'
