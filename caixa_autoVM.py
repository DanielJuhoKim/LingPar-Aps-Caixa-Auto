# ============================================================
#  CAIXA AUTOVM - INTERPRETADOR PARA O ASSEMBLY DO MERCADINHO
# ============================================================

from dataclasses import dataclass
from typing import List, Dict, Tuple, Optional, Any
import re

@dataclass
class Instr:
    op: str
    args: Tuple[str, ...]

class Produto:
    def __init__(self, nome: str, qr: int, preco: float):
        self.nome = nome
        self.qr = qr
        self.preco = preco

class Deposito:
    def __init__(self, nome: str):
        self.nome = nome
        self.estoque: Dict[int, int] = {}  # qr -> quantidade

class Carrinho:
    def __init__(self, nome: str):
        self.nome = nome
        self.itens: Dict[int, int] = {}  # qr -> quantidade
        self.vendidos: Dict[int, int] = {}  # qr -> quantidade vendida

class VirtualMachine:
    """
    VM especializada para o sistema do Mercadinho
    """

    def __init__(self):
        print("\n========= Mercadinho do Jota no seu prédio =============\n")
        # Estruturas de dados
        self.produtos: Dict[int, Produto] = {}
        self.depositos: Dict[str, Deposito] = {}
        self.carrinhos: Dict[str, Carrinho] = {}
        self.variaveis: Dict[str, float] = {}
        self.pagamento_status: str
        
        # Registradores
        self.registers: Dict[str, float] = {
            "TOTAL": 0.0,
            "QTD": 0.0,
            "QR": 0.0,
            "TEMP": 0.0,
            "NOME": 0.0,  # Para nomes (usaremos hash)
            "PRECO": 0.0
        }
        
        # Controle de execução
        self.program: List[Instr] = []
        self.labels: Dict[str, int] = {}
        self.pc: int = 0
        self.halted: bool = False
        self.steps: int = 0
        self.stack: List[float] = []
        
        # Contadores
        self.transacoes_count: int = 0
        self.produtos_cadastrados: int = 0
        self.depositos_criados: int = 0
        self.carrinhos_criados: int = 0

    # --------------------------------------------------------
    #  CARREGAMENTO DE PROGRAMA - PARSER PARA ASSEMBLY GERADO
    # --------------------------------------------------------

    def load_program(self, source: str):
        """Carrega programa assembly gerado pelo parser"""
        self.program.clear()
        self.labels.clear()
        self.stack.clear()
        self.pc = 0
        self.halted = False
        self.steps = 0
        self.transacoes_count = 0
        self.produtos_cadastrados = 0
        self.depositos_criados = 0
        self.carrinhos_criados = 0
        
        # Resetar estruturas
        self.produtos.clear()
        self.depositos.clear()
        self.carrinhos.clear()
        self.variaveis.clear()
        
        # Resetar registradores
        for reg in self.registers:
            self.registers[reg] = 0.0

        lines = source.splitlines()
        instructions = []
        
        # Primeira passada: extrair instruções dos comentários
        for line in lines:
            line = line.strip()
            
            # Ignorar seções e diretivas assembly
            if line.startswith('section') or line.startswith('global') or line.startswith('extern'):
                continue
                
            # Ignorar labels x86
            if line.endswith(':') and not line.startswith(';'):
                continue
                
            # Extrair instruções de comentários
            comment_match = re.search(r'; (.*)', line)
            if comment_match:
                comment = comment_match.group(1).strip()
                
                # Converter comentários em instruções da VM
                if comment.startswith("Definir produto:"):
                    # ; Definir produto: Premium_A, QR: 1001, Preço: 92.90
                    match = re.search(r'Definir produto: (.+), QR: (\d+), Preço: ([\d.]+)', comment)
                    if match:
                        nome, qr, preco = match.groups()
                        instructions.append(Instr("DEFINIR_PRODUTO", (nome, qr, preco)))
                        
                elif comment.startswith("Criar depósito:"):
                    # ; Criar depósito: deposit1
                    match = re.search(r'Criar depósito: (.+)', comment)
                    if match:
                        nome = match.group(1)
                        instructions.append(Instr("CRIAR_DEPOSITO", (nome,)))
                        
                elif comment.startswith("Adicionar ao depósito"):
                    # ; Adicionar ao depósito deposit1: produto 1001, quantidade 10
                    match = re.search(r'Adicionar ao depósito (.+): produto (\d+), quantidade (\d+)', comment)
                    if match:
                        deposito, qr, qtd = match.groups()
                        instructions.append(Instr("ADICIONAR_DEPOSITO", (deposito, qr, qtd)))
                        
                elif comment.startswith("Guardar variável:"):
                    # ; Guardar variável: limite_cliente = 500.00
                    match = re.search(r'Guardar variável: (.+) = ([\d.]+)', comment)
                    if match:
                        nome, valor = match.groups()
                        instructions.append(Instr("GUARDAR_VARIAVEL", (nome, valor)))
                        
                elif comment.startswith("Declarar variável:"):
                    # ; Declarar variável: total_parcial
                    match = re.search(r'Declarar variável: (.+)', comment)
                    if match:
                        nome = match.group(1)
                        instructions.append(Instr("DECLARAR_VARIAVEL", (nome,)))
                        
                elif comment.startswith("Criar carrinho:"):
                    # ; Criar carrinho: cart1
                    match = re.search(r'Criar carrinho: (.+)', comment)
                    if match:
                        nome = match.group(1)
                        instructions.append(Instr("CRIAR_CARRINHO", (nome,)))
                        
                elif comment.startswith("Adicionar ao carrinho"):
                    # ; Adicionar ao carrinho cart1: produto 1001 do depósito deposit1, quantidade 2
                    match = re.search(r'Adicionar ao carrinho (.+): produto (\d+) do depósito (.+), quantidade (\d+)', comment)
                    if match:
                        carrinho, qr, deposito, qtd = match.groups()
                        instructions.append(Instr("ADICIONAR_CARRINHO", (carrinho, deposito, qr, qtd)))
                        
                elif comment.startswith("Vender do carrinho"):
                    # ; Vender do carrinho cart1: produto 1001, quantidade 1
                    match = re.search(r'Vender do carrinho (.+): produto (\d+), quantidade (\d+)', comment)
                    if match:
                        carrinho, qr, qtd = match.groups()
                        instructions.append(Instr("VENDER", (carrinho, qr, qtd)))
                        
                elif comment.startswith("Atribuir variável:"):
                    # ; Atribuir variável: total_parcial = 7.90
                    match = re.search(r'Atribuir variável: (.+) = ([\d.]+)', comment)
                    if match:
                        nome, valor = match.groups()
                        instructions.append(Instr("ATRIBUIR_VARIAVEL", (nome, valor)))
                        
                elif comment.startswith("Consultar variável:"):
                    # ; Consultar variável: total_parcial
                    match = re.search(r'Consultar variável: (.+)', comment)
                    if match:
                        nome = match.group(1)
                        instructions.append(Instr("CONSULTAR_VARIAVEL", (nome,)))
                        
                elif comment.startswith("Consultar produto:"):
                    # ; Consultar produto: QR 1001
                    match = re.search(r'Consultar produto: QR (\d+)', comment)
                    if match:
                        qr = match.group(1)
                        instructions.append(Instr("CONSULTAR_PRODUTO", (qr,)))
                        
                elif comment.startswith("Consultar carrinho completo:"):
                    # ; Consultar carrinho completo: cart1
                    match = re.search(r'Consultar carrinho completo: (.+)', comment)
                    if match:
                        nome = match.group(1)
                        instructions.append(Instr("CONSULTAR_CARRINHO", (nome,)))
                        
                elif comment.startswith("Consultar carrinho"):
                    # ; Consultar carrinho cart1[1001]
                    match = re.search(r'Consultar carrinho (.+)\[(\d+)\]', comment)
                    if match:
                        carrinho, qr = match.groups()
                        instructions.append(Instr("CONSULTAR_ITEM_CARRINHO", (carrinho, qr)))
                        
                elif comment.startswith("Consultar depósito"):
                    # ; Consultar depósito deposit1[1001]
                    match = re.search(r'Consultar depósito (.+)\[(\d+)\]', comment)
                    if match:
                        deposito, qr = match.groups()
                        instructions.append(Instr("CONSULTAR_DEPOSITO", (deposito, qr)))
                        
                elif comment.startswith("Pagamento:"):
                    # ; Pagamento: CRÉDITO - APROVADO
                    match = re.search(r'Pagamento: (.+) - (.+)', comment)
                    if match:
                        metodo, status = match.groups()
                        instructions.append(Instr("PAGAMENTO", (metodo, status)))
                        
                elif comment.startswith("Mensagem:"):
                    # ; Mensagem: "Testando tese.jota"
                    match = re.search(r'Mensagem: "(.+)"', comment)
                    if match:
                        mensagem = match.group(1)
                        instructions.append(Instr("MENSAGEM", (mensagem,)))
                        
                elif comment == "Finalização do sistema":
                    instructions.append(Instr("FINALIZAR", ()))
                    
        self.program = instructions

    # --------------------------------------------------------
    #  EXECUÇÃO DE INSTRUÇÕES
    # --------------------------------------------------------

    def step(self):
        """Executa uma instrução"""
        if self.halted:
            return
            
        if not (0 <= self.pc < len(self.program)):
            self.halted = True
            return

        instr = self.program[self.pc]
        self.steps += 1

        try:
            self._execute_instruction(instr)
            self.pc += 1
        except Exception as eRRO:
            print(eRRO)
            self.halted = True

    def _execute_instruction(self, instr: Instr):
        """Executa uma instrução específica"""
        op = instr.op
        args = instr.args

        if op == "DEFINIR_PRODUTO":
            nome, qr, preco = args
            self.definir_produto(nome, int(qr), float(preco))

        elif op == "CRIAR_DEPOSITO":
            nome, = args
            self.criar_deposito(nome)

        elif op == "ADICIONAR_DEPOSITO":
            deposito, qr, qtd = args
            self.adicionar_estoque(deposito, int(qr), int(qtd))

        elif op == "GUARDAR_VARIAVEL":
            nome, valor = args
            self.guardar_variavel(nome, float(valor))

        elif op == "DECLARAR_VARIAVEL":
            nome, = args
            self.declarar_variavel(nome)

        elif op == "CRIAR_CARRINHO":
            nome, = args
            self.criar_carrinho(nome)

        elif op == "ADICIONAR_CARRINHO":
            carrinho, deposito, qr, qtd = args
            self.adicionar_carrinho(carrinho, deposito, int(qr), int(qtd))

        elif op == "VENDER":
            carrinho, qr, qtd = args
            self.processar_venda(carrinho, int(qr), int(qtd))

        elif op == "ATRIBUIR_VARIAVEL":
            nome, valor = args
            self.atribuir_variavel(nome, float(valor))

        elif op == "CONSULTAR_VARIAVEL":
            nome, = args
            self.consultar_variavel(nome)

        elif op == "CONSULTAR_PRODUTO":
            qr, = args
            self.consultar_produto(int(qr))

        elif op == "CONSULTAR_CARRINHO":
            nome, = args
            self.consultar_carrinho(nome)

        elif op == "CONSULTAR_ITEM_CARRINHO":
            carrinho, qr = args
            self.consultar_item_carrinho(carrinho, int(qr))

        elif op == "CONSULTAR_DEPOSITO":
            deposito, qr = args
            self.consultar_deposito(deposito, int(qr))

        elif op == "PAGAMENTO":
            metodo, status = args
            self.processar_pagamento(metodo, status)

        elif op == "MENSAGEM":
            mensagem, = args
            self.mostrar_mensagem(mensagem)

        elif op == "FINALIZAR":
            self.finalizar_execucao()

        else:
            raise Exception(f"[Semântico] Instrução desconhecida: {op}")

    # --------------------------------------------------------
    #  OPERAÇÕES DO SISTEMA
    # --------------------------------------------------------

    def definir_produto(self, nome: str, qr: int, preco: float):
        """Define um novo produto"""
        self.produtos[qr] = Produto(nome, qr, preco)
        self.produtos_cadastrados += 1

    def criar_deposito(self, nome: str):
        """Cria um novo depósito"""
        self.depositos[nome] = Deposito(nome)
        self.depositos_criados += 1

    def adicionar_estoque(self, deposito_nome: str, qr: int, quantidade: int):
        """Adiciona estoque a um depósito"""
        if deposito_nome not in self.depositos:
            raise Exception(f"[Semântico] [Semântico] Depósito '{deposito_nome}' não existe")
        if qr not in self.produtos:
            raise Exception(f"[Semântico] [Semântico] Produto QR={qr} não definido")
            
        dep = self.depositos[deposito_nome]
        dep.estoque[qr] = dep.estoque.get(qr, 0) + quantidade
        produto = self.produtos[qr]

    def guardar_variavel(self, nome: str, valor: float):
        """Guarda uma variável com valor"""
        self.variaveis[nome] = valor

    def declarar_variavel(self, nome: str):
        """Declara uma variável com valor 0"""
        self.variaveis[nome] = 0.0

    def criar_carrinho(self, nome: str):
        """Cria um novo carrinho"""
        self.carrinhos[nome] = Carrinho(nome)
        self.carrinhos_criados += 1

    def adicionar_carrinho(self, carrinho_nome: str, deposito_nome: str, qr: int, quantidade: int):
        """Adiciona item ao carrinho"""
        if carrinho_nome not in self.carrinhos:
            raise Exception(f"[Semântico] [Semântico] Carrinho '{carrinho_nome}' não existe")
        if deposito_nome not in self.depositos:
            raise Exception(f"[Semântico] [Semântico] Depósito '{deposito_nome}' não existe")
        if qr not in self.produtos:
            raise Exception(f"[Semântico] [Semântico] Produto QR={qr} não definido")
            
        car = self.carrinhos[carrinho_nome]
        dep = self.depositos[deposito_nome]
        
        estoque_disponivel = dep.estoque.get(qr, 0)
        if estoque_disponivel < quantidade:
            raise Exception(f"[Semântico] [Semântico] Estoque insuficiente: {estoque_disponivel} disponíveis, {quantidade} solicitados")
            
        dep.estoque[qr] -= quantidade
        car.itens[qr] = car.itens.get(qr, 0) + quantidade
        
        produto = self.produtos[qr]
        valor = produto.preco * quantidade
        self.registers["TOTAL"] += valor
        

    def processar_venda(self, carrinho_nome: str, qr: int, quantidade: int):
        """Processa venda de itens do carrinho"""
        if carrinho_nome not in self.carrinhos:
            raise Exception(f"[Semântico] [Semântico] Carrinho '{carrinho_nome}' não existe")
            
        car = self.carrinhos[carrinho_nome]
        if qr not in car.itens or car.itens[qr] < quantidade:
            raise Exception(f"[Semântico] [Semântico] Quantidade insuficiente no carrinho: {car.itens.get(qr, 0)} disponíveis")
            
        car.itens[qr] -= quantidade
        car.vendidos[qr] = car.vendidos.get(qr, 0) + quantidade
        self.transacoes_count += 1
        
        produto = self.produtos[qr]
        valor_venda = produto.preco * quantidade

    def atribuir_variavel(self, nome: str, valor: float):
        """Atribui valor a uma variável existente"""
        if nome not in self.variaveis:
            raise Exception(f"[Semântico] Variável '{nome}' não declarada")
        self.variaveis[nome] = valor

    def consultar_variavel(self, nome: str):
        """Consulta valor de uma variável"""
        if nome not in self.variaveis:
            raise Exception(f"[Semântico] Variável '{nome}' não existe")
        valor = self.variaveis[nome]
        print(f"{nome} = {valor:.2f}")

    def consultar_produto(self, qr: int):
        """Consulta preço de um produto"""
        if qr not in self.produtos:
            print(f"❌ Produto QR={qr} não encontrado")
        else:
            produto = self.produtos[qr]
            print(f"Produto {produto.nome} (QR: {qr}): R$ {produto.preco:.2f}")

    def consultar_carrinho(self, carrinho_nome: str):
        """Consulta itens no carrinho"""
        if carrinho_nome not in self.carrinhos:
            raise Exception(f"[Semântico] Carrinho '{carrinho_nome}' não existe")
            
        car = self.carrinhos[carrinho_nome]
        print(f"=== CARRINHO {carrinho_nome.upper()} ===")
        total_carrinho = 0.0
        
        for qr, qtd in car.itens.items():
            if qtd > 0:
                produto = self.produtos[qr]
                valor = produto.preco * qtd
                total_carrinho += valor
                print(f"  {produto.nome}: {qtd} unidades (R$ {valor:.2f})")
                
        print(f"  TOTAL: R$ {total_carrinho:.2f}")

    def consultar_item_carrinho(self, carrinho_nome: str, qr: int):
        """Consulta item específico no carrinho"""
        if carrinho_nome not in self.carrinhos:
            raise Exception(f"[Semântico] Carrinho '{carrinho_nome}' não existe")
            
        car = self.carrinhos[carrinho_nome]
        qtd = car.itens.get(qr, 0)
        if qr in self.produtos:
            produto = self.produtos[qr]
            print(f"Carrinho {carrinho_nome}[{produto.nome}]: {qtd} unidades")
        else:
            print(f"Carrinho {carrinho_nome}[{qr}]: {qtd} unidades")

    def consultar_deposito(self, deposito_nome: str, qr: int):
        """Consulta estoque no depósito"""
        if deposito_nome not in self.depositos:
            raise Exception(f"[Semântico] Depósito '{deposito_nome}' não existe")
            
        dep = self.depositos[deposito_nome]
        qtd = dep.estoque.get(qr, 0)
        if qr in self.produtos:
            produto = self.produtos[qr]
            print(f"Depósito {deposito_nome}[{produto.nome}]: {qtd} unidades")
        else:
            print(f"Depósito {deposito_nome}[{qr}]: {qtd} unidades")

    def processar_pagamento(self, metodo: str, status: str):
        """Processa pagamento"""
        print(f"\nPagamento: {metodo}")
        print(f"Status: {status}")
        if status == "APROVADO":
            for carrinho in self.carrinhos.values():
                # Transferir itens do carrinho para vendidos
                for qr, quantidade in list(carrinho.itens.items()):
                    if quantidade > 0:
                        carrinho.vendidos[qr] = carrinho.vendidos.get(qr, 0) + quantidade
                        carrinho.itens[qr] = 0  # Limpar o carrinho
            self.registers["TOTAL"] = 0.0
        else:
            print("Transação recusada!")

        self.pagamento_status = status

    def mostrar_mensagem(self, mensagem: str):
        """Mostra mensagem do sistema"""
        print(f"{mensagem}")

    def finalizar_execucao(self):
        """Finaliza a execução e mostra nota fiscal"""

        if self.pagamento_status == "APROVADO":
            notinha = input("\nImprimir nota fiscal(s/n)? ")

            if notinha == "s":
                print("\n=== NOTA FISCAL ===")
                print("Produtos vendidos:")
                
                total_venda = 0.0
                produtos_vendidos = False
                
                # Percorrer todos os carrinhos
                for carrinho_nome, carrinho in self.carrinhos.items():
                    for qr, quantidade_vendida in carrinho.vendidos.items():
                        if quantidade_vendida > 0 and qr in self.produtos:
                            produto = self.produtos[qr]
                            subtotal = produto.preco * quantidade_vendida
                            total_venda += subtotal
                            print(f"- {quantidade_vendida}x {produto.nome} (carrinho {carrinho_nome}): R$ {subtotal:.2f}")
                            produtos_vendidos = True
                
                if not produtos_vendidos:
                    print("- Nenhum produto foi adicionado")
                
                print(f"Total da venda: R$ {total_venda:.2f}")
                print("===================")

            print("\nCompra finalizada\n")
        self.halted = True

    def run(self, max_steps: Optional[int] = 1000):
        """Executa o programa até o fim"""
        while not self.halted:
            if max_steps is not None and self.steps >= max_steps:
                print("[AVISO] Limite de steps atingido")
                break
            self.step()

    def reset(self):
        """Reseta a VM para estado inicial"""
        self.__init__()

    def state(self) -> Dict[str, Any]:
        """Retorna estado completo da VM"""
        return {
            "passos_executados": self.steps,
            "transacoes": self.transacoes_count,
            "total_vendas": self.registers["TOTAL"],
            "produtos_cadastrados": self.produtos_cadastrados,
            "depositos_ativos": self.depositos_criados,
            "carrinhos_ativos": self.carrinhos_criados
        }

# ===================================================================
#  EXECUÇÃO PRINCIPAL
# ===================================================================

if __name__ == "__main__":
    import sys
    
    vm = VirtualMachine()

    if len(sys.argv) > 1:
        # Carregar programa de arquivo
        try:
            with open(sys.argv[1], 'r', encoding='utf-8') as f:
                program_content = f.read()
            vm.load_program(program_content)
            vm.run()
        except FileNotFoundError:
            print(f"✗ Arquivo não encontrado: {sys.argv[1]}")
        except Exception as e:
            print(f"✗ Erro: {e}")
    else:
        print("Uso: python3 caixa_autoVM.py <arquivo.asm>")
        print("Exemplo: python3 caixa_autoVM.py mercado.asm")