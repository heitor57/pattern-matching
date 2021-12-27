# Coloração de grafos

----
## Gerar executáveis

    make

## Remover

    make clean

## Utilização do programa

	pattern-matching -i <entry file> -o <out file> -d(d) (DEBUG) -p <Implementations> -t <Pattern> -e <error [0..Integer]> -f <Pattern file> -n <number of threads> -s(match format to number of matches)
    Possible implementations
    -p h(BMH) s(Serial BMHS) n(Exact Shift-And)
    -p m(Approximated Shift-And) p(Parallel BMHS) a(All)
    -n <number of threads> if (n<=0 or n>numProcessors) then n gets numProcessors
    Examples: pattern-matching -i in.txt -o out.txt -d -p h -t br
    pattern-matching -i in.txt -dd -o out.txt -t "hello world"
    pattern-matching -i in.txt -d -p a -t hello
    pattern-matching -i in.txt -d -p n -f patterns.txt

As opções prováveis a serem mais utilizadas são o i, t, f e p
Caso queira passar somente o arquivo de texto como parâmetro e o padrão ou o arquivo de padrão é possível, e isso fará com que a saída seja na saída padrão e a implementação a ser utilizada irá ser perguntada durante a execução.
Para pular a pergunta de qual implementação a ser utilizado basta utilizar o parâmetro -p com alguma implementação (h,s,n,m,p) ou todas (a).

Quando usando o ShiftAnd aproximado também é necessário informar um erro pelo parâmetro -e caso não for informado por padrão o erro será 0
Quando usando o BMHS Paralelo é necessário especificar o número de threads com o parâmetro -n ou então o número de threads por padrão será o máximo suportado pelo computador.

Passe para o parâmetro -f o arquivo de padrões ou use -t para passar somente um padrão
use -d para ver informações adicionais do programa e -dd para mais informações ainda
