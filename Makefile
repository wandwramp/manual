GLOBALSTY =  $(wildcard globals/*.sty)
FIG = global/graphics/src/*.fig
EPS = $(patsubst global/graphics/src/%, global/graphics/%, \
	$(patsubst %.fig, %.eps, $(wildcard global/graphics/src/*.fig)))
PDFIMG = $(patsubst global/graphics/src/%, global/graphics/%, \
	$(patsubst %.fig, %.pdf, $(wildcard global/graphics/src/*.fig)))
DVI = $(patsubst %.tex, %.dvi, $(patsubst guide/standalone/%, \
		build/guides/%, $(wildcard guide/standalone/*_a.tex)))
STANDALONE = $(patsubst %.tex, %.ps, $(patsubst guide/standalone/%, \
		output/guides/ps/%, $(wildcard guide/standalone/*_a.tex))) \
		$(patsubst %.tex, %.pdf, $(patsubst guide/standalone/%, \
		output/guides/pdf/%, $(wildcard guide/standalone/*_a.tex)))

GUIDESRC = $(wildcard guide/*.tex)
BOOKSRC  = book/book.tex

INSTR = $(patsubst %.tex, %.ps, $(patsubst guide/instr/%, \
	output/instr/%, $(wildcard guide/instr/*.tex))) \
	$(patsubst %.tex, %.pdf, $(patsubst guide/instr/%, \
	output/instr/%, $(wildcard guide/instr/*.tex))) 
BOOK = output/book/book.ps output/book/book.pdf
EXERCISES = output/exercises/ps/comms.ps output/exercises/pdf/comms.pdf \
	output/exercises/ps/cwramp.ps output/exercises/pdf/cwramp.pdf \
	output/exercises/ps/data.ps output/exercises/pdf/data.pdf \
	output/exercises/ps/inter.ps output/exercises/pdf/inter.pdf \
	output/exercises/ps/io.ps output/exercises/pdf/io.pdf \
	output/exercises/ps/intro.ps output/exercises/pdf/intro.pdf \
	output/exercises/ps/kernel.ps output/exercises/pdf/kernel.pdf \
	output/exercises/ps/mtk-sim.ps output/exercises/pdf/mtk-sim.pdf \
	output/exercises/ps/rtlsim.ps output/exercises/pdf/rtlsim.pdf
EXDVI = build/ex/comms.dvi \
	build/ex/cwramp.dvi \
	build/ex/data.dvi \
	build/ex/inter.dvi \
	build/ex/io.dvi \
	build/ex/intro.dvi \
	build/ex/kernel.dvi \
	build/ex/mtk-sim.dvi \
	build/ex/rtlsim.dvi

BASEPATH=$(HOME)/comp201
export TEXINPUTS=:$(BASEPATH)/global:$(BASEPATH)/global/graphics:$(BASEPATH)/guide

#cancel implicit .tex rule
%.dvi : %.tex

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # 
#               BUILDing everything
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # 

# Make all
.PHONY : all
all :	eps dvi standalone allex  instr

# ----------------------------------------------------------
#                    EPS files

#EPS - 'make eps' makes all eps's using the second/third rules
.PHONY : eps
eps :	$(EPS)	

global/graphics/%.eps : global/graphics/src/%.fig
	fig2dev -L eps $< $@



# ----------------------------------------------------------
#                    Standalone Guides

#STANDALONE.dvi - 'make dvi' makes all dvis using the 2nd/3rd rules
.PHONY : dvi
dvi :   $(DVI)
build/guides/%.dvi : guide/standalone/%.tex \
#	      $(subst _a., ., guide/%.tex)\
	      $(GLOBALSTY)
	cd build/guides && latex ../../$<

#GUIDES STANDALONE.ps .pdf - 'make standalone'
.PHONY : standalone
standalone: $(STANDALONE)
output/guides/pdf/%.pdf output/guides/ps/%.ps : build/guides/%.dvi 
	dvips  -o $@ $<
	dvipdf $< $@



# ----------------------------------------------------------
#                     Instructions
.PHONY : instr
instr: $(INSTR)
output/instr/%.ps output/instr/%.pdf :  build/%.dvi
	dvips -o $@ $<
	dvipdf $< $@
build/%.dvi : guide/instr/%.tex $(GLOBALSTY)
	cd build && latex ../$<


# ----------------------------------------------------------
#                     Book
.PHONY : book
book: $(GUIDESRC) $(BOOKSRC)
	cd build && latex ../book/book.tex
	dvips -o output/book/ps/book.ps build/book.dvi
	dvipdf  build/book.dvi  output/book/ps/book.pdf


# ----------------------------------------------------------
#                     Exercises

.PHONY : allex
allex : $(EXERCISES) 
output/exercises/ps/%.ps output/exercises/pdf/%.pdf : build/ex/%.dvi
	dvips -o $@ $<
	dvipdf $< $@

.PHONY : exdvi
exdvi :  $(EXDVI)

#------ Have to do them individually
build/ex/intro.dvi : exercises/intro/intro.tex
	cd build/ex && latex ../../$<
build/ex/data.dvi : exercises/data/data.tex
	cd build/ex && latex ../../$<
build/ex/cwramp.dvi : exercises/cwramp/cwramp.tex
	cd build/ex && latex ../../$<
build/ex/rtlsim.dvi : exercises/rtlsim/rtlsim.tex
	cd build/ex && latex ../../$<
build/ex/io.dvi : exercises/io/io.tex
	cd build/ex && latex ../../$<
build/ex/inter.dvi : exercises/inter/inter.tex
	cd build/ex && latex ../../$<
build/ex/mtk-sim.dvi : exercises/mtk-sim/mtk-sim.tex
	cd build/ex && latex ../../$<
build/ex/kernel.dvi : exercises/kernel/kernel.tex
	cd build/ex && latex ../../$<
build/ex/comms.dvi : exercises/comms/comms.tex
	cd build/ex && latex ../../$<


# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # 
#                clean ups
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # 

.PHONY : cleanout # Just removes the final output
cleanout :
	echo CLEANOUT
	-rm $(wildcard build/*.dvi )
	-rm $(wildcard build/guide/*.dvi )
	-rm $(wildcard build/ex/*.dvi )
	-rm $(wildcard output/guides/ps/* )
	-rm $(wildcard output/guides/pdf/* )
	-rm $(wildcard output/exercises/ps/* )
	-rm $(wildcard output/exercises/pdf/* )

.PHONY : clean # Removes everything
clean:
	echo ALLCLEAN
	-rm $(wildcard build/* )
	-rm $(wildcard build/ex/* )
	-rm $(wildcard build/guides/* )
	-rm $(wildcard output/guides/ps/* )
	-rm $(wildcard output/guides/pdf/* )
	-rm $(wildcard output/exercises/ps/* )
	-rm $(wildcard output/exercises/pdf/* )
	-rm $(wildcard global/graphics/*.eps)





