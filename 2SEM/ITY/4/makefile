#makefile Filip Ježovica

NAME=proj4

all:
	latex $(NAME).tex
	bibtex $(NAME).aux
	latex $(NAME).tex
	latex $(NAME).tex
	dvips $(NAME).dvi
	ps2pdf $(NAME).ps
