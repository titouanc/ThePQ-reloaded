FIGURES = $(shell grep '{.*.eps}' *.tex | cut -d '{' -f 2 | tr -d '}')
TARGET = main.pdf

GLOSSARY_FILES = *.idx *.ist *.alg *.glg *.glo *.gls *.acn *.acr *.ilg *.ind
TEX_FILES = *.aux *.log *.out *.toc *.hist
TEX = pdflatex


all: ${TARGET}

fig: ${FIGURES}

clean:
	rm -f ${GLOSSARY_FILES} ${TEX_FILES}

mrproper: clean
	rm -f ${TARGET} ${FIGURES}

%.pdf: %.tex *.tex ${FIGURES} %.glo2
	${TEX} $<

%.glo: %.tex
	${TEX} $<
	makeglossaries $(subst .tex,,$<)

%.glo2: %.glo
	${TEX} $(subst .glo,,$<).tex
	makeglossaries $(subst .glo,,$<)
	touch $@

%.idx: %.tex
	${TEX} $<
	makeidxssaries $(subst .tex,,$<)

%.idx2: %.idx
	${TEX} $(subst .idx,,$<).tex
	makeidxssaries $(subst .idx,,$<)
	touch $@

figures/%.eps: ../UML/%.dia figures
	dia -e $@ $<

figures:
	mkdir -p figures

.PHONY: all clean mrproper
