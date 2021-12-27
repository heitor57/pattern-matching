# Graph Coloring

----
## Compile

    make

## Clean

    make clean

## Program

	pattern-matching -i <entry file> -o <out file> -d(d) (DEBUG) -p <Implementations> -t <Pattern> -e <error [0..Integer]> -f <Pattern file> -n <number of threads> -s(match format to number of matches)
    Possible implementations
    -p h(BMH) s(Serial BMHS) n(Exact Shift-And)
    -p m(Approximated Shift-And) p(Parallel BMHS) a(All)
    -n <number of threads> if (n<=0 or n>numProcessors) then n gets numProcessors
    Examples: pattern-matching -i in.txt -o out.txt -d -p h -t br
    pattern-matching -i in.txt -dd -o out.txt -t "hello world"
    pattern-matching -i in.txt -d -p a -t hello
    pattern-matching -i in.txt -d -p n -f patterns.txt

The most likely options to be used are i, t, f and p.
In case you only want to pass the text file as a parameter and the pattern or pattern file is possible, and this will cause the output to be on the standard output and the implementation to be used will be asked during execution.
To skip the question of which implementation to use, just use the -p parameter with some implementation (h,s,n,m,p) or all (a).

when using the approximate shift and it is also necessary to inform an error by the parameter -e. If not informed, by default, the error will be 0.
When using parallel BHMS it is necessary to specify the number of threads with the -n parameter or else the number of threads by default will be the maximum supported by the computer.

Pass the pattern file to the -f parameter or use -t to pass only a pattern.
Use -d to see additional program information and -dd for even more information.
