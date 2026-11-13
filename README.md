# Psyche Assembler
Projeto de Software Básico, UnB 2026/2.

Desenvolvido por \
**Giovanni Daldegan, 232002520**

## Utilização

Para compilar o projeto utilizando o compilador GCC, vá para o diretório raiz e execute:

```sh
gcc src/montador.c -o montador
gcc src/ligador.c -o ligador
```

Ainda na raiz, para montar um código fonte, execute um dos dois:

```sh
# Montagem normal
./montador codigo_fonte.asm

# Montagem com comentários de debug
./montador codigo_fonte.asm -d
```

E para ligar **(não implementado)** (fonte pode ser de tipo `.asm` ou `.obj`):

```sh
./ligador [fonte1] [fonte2] [fonte3]
```

Para verificar o histórico de commits, garanta que tem o git instalado e execute
um dos seguintes comandos no diretório raiz:

```sh
# histórico no terminal
git log

# histórico com UI do gitk
gitk
```

## Montador

O programa "montador" implementa um montador simplificado para a linguagem
*assembly* inventada apresentada em aula.

Deve ser fornecido arquivo de extensão `.asm` para montagem; o resultado será um
arquivo de texto `.obj` de mesmo nome.

A chamada do montador deve ser como abaixo:
```
./montador <código-fonte>.asm
```

Executar `./montador -h` apresenta um resumo destas informações de uso.

Caso o programa apresente erros léxicos, sintáticos ou semânticos, o montador
cancela a execução e indica o tipo do erro e, se possível, sua causa.

## Ligador

O programa "ligador" recebe até 3 arquivos fonte e os liga sequencialmente. O
primeiro arquivo deve conter a rotina "*main*" no seu início,

Todos os arquivos devem ser de tipo `.obj` ou `.asm`, de forma que os arquivos
não montados são primeiro montados por uma chamada ao montador e, então,
ligados.

Sua saída é o arquivo de texto `.LIG`, um executável com todos os módulos
ligados.

## Linguagem *assembly* inventada

Algumas regras da linguagem:
- A linguagem não é *case-sensitive*, todos caracteres são tratados como
  maiúsculos;
- Os tokens devem ser separados por espaços;
- Só pode ter uma instrução ou diretiva por linha, de forma que **uma linha deve
  ter uma instrução/diretiva e seus argumentos adequados para ser válida** (não
  é aceita instrução/diretiva com argumentos em linhas diferentes);
- Só pode ter um rótulo por linha;
- Instruções apenas podem receber rótulos como argumento, não constantes
  numéricas;
- Os comentários são iniciados com ponto e vírgula (`;`), desde que haja espaço
  antes do comentário (`STOP; fim do programa` é um erro léxico);
- Os únicos caracteres especiais válidos são: `_`, `+` e `:`.

O programa deve ser dividido em seção de dados (`SECTION DATA`) e seção de
programa (`SECTION TEXT`).

Uma descrição simplificada da gramática da linguagem (`EOL` é *end-of-line*):

```bnf
<linha>                ::= <rótulo>":" <comando> <EOL> | "SECTION" <seção> <EOL>
<seção>                ::= "TEXT" | "DATA"
<comando>              ::= <instrução> <expressão> | "COPY" <expressão> <expressão> | <diretiva-alocação> | <diretiva-declarativa>
<expressão>            ::= <rótulo> | <rótulo> "+" <constante>
<diretiva-alocação>    ::= "CONST" <constante> | "SPACE" | "SPACE" <constante>
<diretiva-declarativa> ::= <nome> "EQU" <constante> | "EXTERN" <rótulo> | "PUBLIC"
```

Abaixo há um exexmplo de código fonte.

```asm
SECTION DATA
N1:     SPACE         ; comentário 1
N2:     CONST   -48
N4:     SPACE

; fim da seção de dados
; iníncio da seção de programa
SECTION TEXT
ROT:    INPUT   N1
        COPY    N1 N4 ;comentário final
        COPY    N2 N3
        COPY    N3 N3
        OUTPUT  N3
        STOP
```

> [!warning]
> Atenção, **"TEXT" e "DATA" são palavras reservadas**, não são símbolos/rótulos
> aceitos.

A seguir, são apresentadas as diretivas implementadas e as instruções da
linguagem estudada.

### Diretivas

Legenda: \
**$\text{ARG[i]}$**: $i$-ésimo argumento fornecido 

| Diretiva | Nº args    | Descrição                                                                       |
| -------- | ---------- | ------------------------------------------------------------------------------- |
| SPACE    | 0 \| 1     | reserva 0 ou $\text{ARG[1]}$ palavras com valor 0                               |
| CONST    | 1          | reserva 1 palavra com o valor de $\text{ARG[1]}$                                |
| EQU      | 2 (infixo) | define o símbolo $\text{ARG[1]}$ como sinônimo para a constante $\text{ARG[2]}$ |
| IF       | 1          | caso $\text{ARG[1]}$ == 0, ignora a linha seguinte na montagem                  |
| BEGIN    | 0          | inicia módulo                                                                   |
| END      | 0          | termina módulo                                                                  |
| PUBLIC   | 1          | indica símbolo definido no módulo                                               |
| EXTERN   | 0          | indica $\text{ARG[1]}$ como símbolo definido em módulo externo                  |
| +        | 2 (infixo) | soma o valor de um símbolo ($\text{ARG[1]}$) a um número ($\text{ARG[2]}$)      |
| SECTION  | 1          | inicia seção de nome $\text{ARG[1]}$ (TEXT ou DATA)                             |

> [!note]
> A diretiva EQU deve ser utilizada apenas antes das seções TEXT e DATA. \
> A diretiva IF deve ser utilizada exclusivamente na seção TEXT.
>
> As diretivas ORG, MACRO e ENDMACRO não são implementadas pelo montador.

### Instruções

O projeto do processador conta com 2 registradores:
- **ACC**: acumulador, utilizado para todas operações
- **PC**: contador de programa, indica a próxima instrução para ser executada

Legenda:\
**mem(PTR)**: valor guardado em memória no endereço PTR \
**in()**: retorna o valor lido da entrada \
**out(PTR)**: imprime o valor guardado em PTR na saída \
**$\text{ARG[i]}$**: i-ésimo argumento fornecido 

| Instrução | opcode | tamanho | Nº args | descrição                                    |
| --------- | ------ | ------- | ------- | -------------------------------------------- |
| ADD       | 1      | 2       | 1       | ACC <- ACC + mem($\text{ARG[1]}$)            |
| SUB       | 2      | 2       | 1       | ACC <- ACC - mem($\text{ARG[1]}$)            |
| MULT      | 3      | 2       | 1       | ACC <- ACC * mem($\text{ARG[1]}$)            |
| DIV       | 4      | 2       | 1       | ACC <- ACC / mem($\text{ARG[1]}$) (inteira)  |
| JMP       | 5      | 2       | 1       | PC <- $\text{ARG[1]}$                        |
| JMPN      | 6      | 2       | 1       | PC <- $\text{ARG[1]}$ se ACC < 0             |
| JMPP      | 7      | 2       | 1       | PC <- $\text{ARG[1]}$ se ACC > 0             |
| JMPZ      | 8      | 2       | 1       | PC <- $\text{ARG[1]}$ se ACC == 0            |
| COPY      | 9      | 3       | 2       | mem($\text{ARG[2]}$) <- mem($\text{ARG[2]}$) |
| LOAD      | 10     | 2       | 1       | ACC <- mem($\text{ARG[1]}$)                  |
| STORE     | 11     | 2       | 1       | mem($\text{ARG[1]}$) <- ACC                  |
| INPUT     | 12     | 2       | 1       | mem($\text{ARG[1]}$) <- in()                 |
| OUTPUT    | 13     | 2       | 1       | out(mem($\text{ARG[1]}$))                    |
| STOP      | 14     | 1       | 0       | termina o programa                           |
