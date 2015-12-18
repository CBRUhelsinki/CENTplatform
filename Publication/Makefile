.PHONY: draft, clean, cleanup

cleanremove=*.toc *.aux *.ps *.log *.lot *.lof *.bbl *.blg *.dvi *.mtc* *.ptc* *.bmt *.idx *.ind *.ilg .DS_Store .DS_Store.1 *.tex~ *.nav *.out *.snm *.fls *.fdb_latexmk *.nlo

# Definitions
latexfile1=CENTplatform

all:
	TEXINPUTS=.//:;	export TEXINPUTS; latexmk -pdf $(latexfile1)
	@make cleanup

coverletter:
	pdflatex cover_letter
	@make cleanup
# CLEANUP
clean:
	@rm -f *.pdf
	@make cleanup

cleanup:
	@for i in $(cleanremove) ; do find . -name $$i -exec rm -f {} \; ; done

