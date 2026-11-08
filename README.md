# PintOS

Um sistema operacional experimental escrito em C23 e Assembly x86_64, que é do
caralho!

## Objetivos

Esse é um projeto de __resenha__, feito para descontrair e codar junto com uns
amigos do meu server do Discord.

Não leve nada aqui a sério, ou leve (lá ele).

Primariamente, o objetivo é fazer algo que rode em ambientes `virt` como o QEMU
por exemplo. Isso significa que o foco serão drivers para dispositivos VirtIO e
no geral vamos dar prioridade de estabilidade para sistemas virtualizados.

Como esse é um projeto bem "from scratch" (e não me venha dizer que pra ser eu
teria que criar os átomos e o universo) a ideia é fazer até o bootloader na
mão.

## Tech Stack

A tech stack desse projeto é bem compacta e simples.

* `meson` e `ninja` para build;
* `python3` para scripting (sem libs externas);
* `just` para command running;
* `qemu` para virtualização, debugging, disk images (qcow2);
* `clang` e `llvm` para compilar, linkar, formatar, lintar, e etc;
* `nasm` para Assembly x86_64;
* `linux` no geral para tooling auxilar, todos os scripts e tooling vai assumir
  ambiente linux.

Versões em específico não são estritamente requeridas no momento, tente ao menos
ter tudo up-to-date e se possível use algo como Fedora, Arch Linux, Gentoo ou
qualquer coisa que tenha pacotes atualizados.

## Licença

Esse software é livre, no sentido mais literal da palavra. E está sendo
disponibilizado para todos sob a licença [UNLICENSE](./UNLICENSE).

Basicamente, isso aqui é public domain. It's free real state, baby!

> Se não é Unlicense, o software é *sabor* livre.
