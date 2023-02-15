# Projeto de Compiladores 

Repositório utilizado para o projeto da disciplina de Compiladores da Universidade de Brasília. Semestre letivo: 2022/2.

Estudantes:

- Fernanda Macedo de Sousa

- Lucas Vinicius Pinheiro

- Rafael Gonçalves de Paulo

- Thales Lima Menezes

## Estrutura de Pastas e Arquivos

- `/`: No diretório raiz, encontram-se os arquivos `Makefile`, `README.md` e `.gitignore`, além do  arquivo `install-deps.sh` com as dependências que precisam ser instaladas e os arquivos `/run-test.sh` e `/run-tests.sh` para executar os arquivos de testes;
- `/tests`: contém os arquivos de teste;
- `/src`: contém o arquivo `c_lang.l`, correspondente ao analisador léxico e o arquivo `sintatico.y` correspondente ao analisador sintático, além dos arquivos `.c` auxiliares.
- `/lib`: contém os arquivos `.h`
- `/include` e `/obj`: após a execução do makefile essas pastas serão criadas, onde serão organizados os arquivos gerados pelos programas flex e bison.

## Dependências

Para instalar as dependências necessárias ao projeto, basta executar: `./install-deps.sh`

## Compilação/Execução

Faça uso do makefile por meio do comando `make`

*Obs: esse comando irá gerar o executável `parser`, mas não é necessário se preocupar com ele pois ele será utilizado pelo arquivo bash que roda os testes.*

### Como usar os arquivos de teste

Para testar um arquivo especificamente, faça uso do arquivo bash `run-test.sh`, seguido do caminho do arquivo presente na pasta de testes.

Exemplo: `./run-test.sh "tests/if.txt"`

Para executar todos os testes de uma só vez (todos os arquivos presentes na pasta de testes), basta executar o comando `./run-tests.sh`

### Como usar o script `mount.py`

![image](https://user-images.githubusercontent.com/33532177/218939098-5fc27412-9a42-4d93-8efc-b8580c3eb555.png)

É esperado que exista um arquivo chamado `code.txt` contendo código de 3 endereços e será gerado um `result.asm` em formato assembly RISC-V válido.

```bash
python3 mount.py
```

Comandos suportados:
- `read <var>`
- `log <var>`
- `jeqz <registrador> <label>`
- atribuição (=):
  - `<var> = <var | literal>`
  - `<var> = <var | literal> [+-] <var | literal>`
- `exit <literal>`
  - opcional: literal é o exit code com o qual será encerrado o programa

fora os comandos listados acima, que são manipulados pelo script, pode ser injetado código assembly RISCV diretamente; qualquer linha não vazia que não se enquadra nas regras acima, e não é um comentário, é redirecionada para o arquivo de saída (ex.: Labels)

NOTA: limitado a criação de 12 variáveis e literais distintos
