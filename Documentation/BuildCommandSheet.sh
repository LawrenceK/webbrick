#
#  Build Command Sheet for Linux
#
rm CommandSheet6_00.pdf
#texify --language=LaTeX --pdf --clean CommandSheet.tex
pdflatex CommandSheet.tex
mv CommandSheet.pdf CommandSheet6_00.pdf
