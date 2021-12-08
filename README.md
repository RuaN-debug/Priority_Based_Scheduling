# Priority Based Scheduling

Tabalho realizado para composição da nota 2 de Sistemas Operacionais (ECOS02), do curso de Engenharia da Computação na Universidade Federal de Itajubá.
Discentes: 
  - Ariadne Pereira da Silva Vieira - 2019004106
  - Giuseppe Baruffaldi Scassiotti - 2019004330
  - Ruan Marcos Balbino de Alcântara Pereira - 2019014766

Doscente:
  - Otávio de Souza Martins Gomes

# Como rodar

É preciso baixar todos os arquivos, incluindo o arquivo de teste input.txt ou clonar o repositório com:

```sh
git clone git@github.com:RuaN-debug/Priority_Based_Scheduling.git
```

Para compilar:

```sh
gcc priorityBasedScheduling.c -o priorityBasedScheduling.exe
```

Para rodar no Windows:

```sh
priorityBasedScheduling.exe
```

Para rodar no Linux:

```sh
./priorityBasedScheduling.exe
```

# Arquivo de entrada e saída

Com o seguinte arquivo de entrada:
```sh
0 5 2
0 2 3
1 4 1
3 3 4
```
Foi obtido o seguinte arquivo de saída:

```sh
Tempo		P1	P2	P3	P4
  - 1		##			--		
 1- 2		##			--		
 2- 3		##			--	--
 3- 4		##			--	--
 4- 5		##			--	--
 5- 6						--	##
 6- 7						--	##
 7- 8						--	##
 8- 9						##		
 9-10						##		
10-11						##		
11-12						##		

Tempo medio de vida = 6.00
Tempo medio de espera = 2.00
Numero de trocas de contexto = 3
```
