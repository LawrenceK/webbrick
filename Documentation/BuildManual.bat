rem
rem  Build the main manual
rem
DEL newprimer.pdf
DEL webrick.pdf
DEL webbrick6_00.pdf
texify --language=LaTeX --pdf --clean webrick.tex
REN webrick.pdf webbrick6_00.pdf
