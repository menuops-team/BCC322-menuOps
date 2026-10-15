# MenuOps

> Sistema de gestão de fichas técnicas, cálculo automático de CMV e precificação dinâmica para hamburgueria artesanal.

Trabalho da disciplina **BCC322 – Engenharia de Software I** (DECOM/UFOP), implementado a partir do Documento de Especificação de Requisitos MenuOps v1.2 (TerraLAB, 2026).

O controle financeiro e a precificação do cardápio da hamburgueria (cerca de 35 itens entre hambúrgueres e porções) hoje são feitos de forma manual: fichas técnicas guardadas de cabeça ou em cadernos, sem mensuração imediata do impacto de reajustes de insumos críticos no CMV. Como boa parte dos insumos é perecível e ninguém sabe a quanto tempo eles estão guardados, o insumo mais novo acaba sendo usado primeiro, fazendo com que o mais antigo estraga sem registro, e o custo real de cada prato fica maior do que a ficha técnica indica.

O MenuOps automatiza esse ciclo: a entrega do fornecedor é registrada como lote (preço, quantidade, data, validade estimada); o sistema reavalia diariamente os lotes (o que consumir com prioridade, o que está impróprio); a ficha técnica de cada prato é combinada com os preços dos lotes efetivamente usados, rateando as perdas, apresentando o **CMV teórico** e o **CMV real**, e o preço de venda é sugerido a partir da margem alvo, com um **farol de margem** (verde/amarelo/vermelho) apontando os pratos que exigem reajuste. O produto organiza-se nos módulos **MenuEstoque** (recebimento e controle de lotes) e **MenuWeb** (cadastros, fichas técnicas, CMV, painel e relatórios), com acesso por perfil (Estoquista, Chef de Cozinha, Gestor/Dono, Administrador do Sistema).

Para a especificação completa de requisitos, atores, serviços e critérios de aceitação, consulte o [DER](doc/DER_menuOPS.pdf).



## Instalação

Pré-requisitos: `g++` com suporte a C++17, GNU `make` e `git`.

OS X & Linux:

```sh
# Debian/Ubuntu
sudo apt install g++ make git
# Fedora/RHEL
sudo dnf install gcc-c++ make git
git clone https://github.com/yuri4417/BCC322_menuOPS
make build
```

Windows:

```sh
# Instale o MSYS2 com mingw-w64 (g++) e make,
# e execute os comandos abaixo no terminal Git Bash/MSYS2:
git clone https://github.com/yuri4417/BCC322_menuOPS
make build
```

## Exemplo de uso

Compilar a aplicação:

```sh
make build
make run
```

Rodar as suítes de teste:

```sh
make run_unit
make run_func
```

Gerar a biblioteca dinâmica e o executável vinculado a ela:

```sh
make build_dll_app
```

Limpar os binários gerados:

```sh
make clean
```

## Configuração para Desenvolvimento

Instale as dependências de desenvolvimento (`g++`, `make`, `git` — ver seção de instalação) e use os alvos do `Makefile` para compilar e testar, nas plataformas Linux e Windows (via Git Bash/MSYS2):

```sh
make build       # compila src/*.cpp -> bin/app(.exe)
make run         # compila e executa a aplicação
make run_unit    # compila e executa os testes unitários (test/unit)
make run_func    # compila e executa os testes funcionais (test/func)
make build_o     # gera os objetos bin/*.o
make build_a     # gera a biblioteca estática bin/app.a
make build_so    # gera a biblioteca compartilhada bin/app.so (Linux)
make build_dll   # gera a biblioteca dinâmica bin/app.dll (Windows)
make clean       # remove os binários de bin/
```

## Histórico de lançamentos

* 0.0.1
  * Trabalho em andamento

## Meta

Seu Nome – [@SeuNome](https://twitter.com/...) – [SeuEmail@exemplo.com](mailto:SeuEmail@exemplo.com)

Projeto acadêmico, sem licença comercial. Veja `DER_menuOPS.pdf` para mais informações.

## Contributing

1. Faça o _fork_ do projeto (<https://github.com/yourname/yourproject/fork>)
2. Crie uma _branch_ para sua modificação (`git checkout -b feature/fooBar`)
3. Faça o _commit_ (`git commit -am 'Add some fooBar'`)
4. _Push_ (`git push origin feature/fooBar`)
5. Crie um novo _Pull Request_
