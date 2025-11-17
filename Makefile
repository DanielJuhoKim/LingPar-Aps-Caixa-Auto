# Makefile para o Mercadinho do Seu Prédinho

CC = gcc
FLEX = flex
BISON = bison
PYTHON = python3
VM = caixa_autoVM.py

COMPILADOR = mercadinho
EXTENSAO_ENTRADA = jota
EXTENSAO_SAIDA = asm

.PHONY: all clean run

all: $(COMPILADOR)

# Primeiro gerar os arquivos .c, depois compilar
lex.yy.c: lexer.l
	@$(FLEX) lexer.l

parser.tab.c parser.tab.h: parser.y
	@$(BISON) -d parser.y

$(COMPILADOR): lex.yy.c parser.tab.c
	@$(CC) lex.yy.c parser.tab.c -o $(COMPILADOR)

run: $(COMPILADOR)
	@echo "Uso: ./$(COMPILADOR) arquivo.$(EXTENSAO_ENTRADA) programa.$(EXTENSAO_SAIDA)"
	@echo "     $(PYTHON) $(VM) programa.$(EXTENSAO_SAIDA)"

clean:
	@rm -f *.o *.tab.* lex.yy.c $(COMPILADOR)
	@rm -f parser.tab.h parser.tab.c
	@rm -f *.$(EXTENSAO_SAIDA)

help:
	@echo "Comandos disponíveis:"
	@echo "  make all    - Compila o compilador"
	@echo "  make clean  - Limpa arquivos gerados"
	@echo "  make run    - Mostra uso"
	@echo ""
	@echo "Exemplo de uso:"
	@echo "  ./$(COMPILADOR) exemplo.$(EXTENSAO_ENTRADA) programa.$(EXTENSAO_SAIDA)"
	@echo "  $(PYTHON) $(VM) programa.$(EXTENSAO_SAIDA)"