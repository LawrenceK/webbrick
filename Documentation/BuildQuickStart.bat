rem
rem  build the quick start guide
rem
DEL quickstart.pdf
texify --language=LaTeX --pdf --clean WebBrickQuickStart.tex
REN WebBrickQuickStart.pdf QuickStart.pdf
