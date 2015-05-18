#Soubor: makefile
#Autor:  FILIP JEZOVICA


NAME=proj2

all:
	latex $(NAME).tex
	latex $(NAME).tex # 2x
	dvips -t a4 $(NAME).dvi
	ps2pdf $(NAME).ps

clean:
	-rm -f $(NAME).aux $(NAME).ps $(NAME).dvi $(NAME).log
