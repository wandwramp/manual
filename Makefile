GLOBALSTY =  $(wildcard globals/*.sty)
FIG = global/graphics/src/*.fig
EPS = $(patsubst global/graphics/src/%, global/graphics/%, \
	$(patsubst %.fig, %.eps, $(wildcard global/graphics/src/*.fig)))
PDFIMG = $(patsubst global/graphics/src/%, global/graphics/%, \
	$(patsubst %.fig, %.pdf, $(wildcard global/graphics/src/*.fig)))
DVI = $(patsubst %.tex, %.dvi, $(patsubst guide/standalone/%, \
		build/guides/%, $(wildcard guide/standalone/*_a.tex)))
STANDALONE = $(patsubst %.tex, %.ps, $(patsubst guide/standalone/%, \
		output/guides/ps/%, $(wildcard guide/standalone/*a.tex))) \
		$(patsubst %.tex, %.pdf, $(patsubst guide/standalone/%, \
		output/guides/pdf/%, $(wildcard guide/standalone/*a.tex)))

GUIDESRC = $(wildcard guide/*.tex)
BOOKSRC  = book/book.tex

INSTR = $(patsubst %.tex, %.ps, $(patsubst guide/instr/%, \
	output/instr/ps/%, $(wildcard guide/instr/*.tex))) \
	$(patsubst %.tex, %.pdf, $(patsubst guide/instr/%, \
	output/instr/pdf/%, $(wildcard guide/instr/*.tex))) 
INSTRDVI = $(patsubst %.tex, %.dvi, $(patsubst guide/instr/%, \
	build/instr/%, $(wildcard guide/instr/*.tex)))

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

BASEPATH:=$(shell pwd)
export TEXINPUTS=:$(BASEPATH)/global:$(BASEPATH)/global/graphics:$(BASEPATH)/guide

#cancel implicit .tex rule
%.dvi : %.tex


#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-# 
#               BUILDing everything			  #
#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-# 

# Make all
.PHONY : all
all :	eps standalone exercise instr book


#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-# 
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
	cd build/guides && \
	latex ../../$< && \
	latex ../../$<



#GUIDES STANDALONE.ps .pdf - 'make standalone'
.PHONY : standalone
standalone: $(STANDALONE)

output/guides/ps/%.ps : build/guides/%.dvi 
	dvips  -o $@ $<

output/guides/pdf/%.pdf : build/guides/%.dvi 
	dvips -Ppdf -G0 $< -o $(patsubst %.pdf, %.ps, $@)
	cd output/guides/pdf && \
	ps2pdf  -sPAPERSIZE=a4 -dMaxSubsetPct=100 -dCompatibilityLevel=1.2 \
		-dSubsetFonts=true -dEmbedAllFonts=true  \
		$(patsubst %.pdf, %.ps, ../../../$@)
	rm $(patsubst %.pdf, %.ps, $@)


# ----------------------------------------------------------
#                     Instructions
.PHONY : instr
instr: $(INSTR)

.PHONY : instrdvi
instrdvi: $(INSTRDVI)

output/instr/ps/%.ps :  build/instr/%.dvi
	dvips -o $@ $<

output/instr/pdf/%.pdf :  build/instr/%.dvi
	dvips -Ppdf -G0 $< -o $(patsubst %.pdf, %.ps, $@)
	cd output/instr/pdf && \
	ps2pdf  -sPAPERSIZE=a4 -dMaxSubsetPct=100 -dCompatibilityLevel=1.2 \
		-dSubsetFonts=true -dEmbedAllFonts=true  \
		$(patsubst %.pdf, %.ps, ../../../$@)
	rm $(patsubst %.pdf, %.ps, $@)

build/instr/%.dvi : guide/instr/%.tex $(GLOBALSTY)
	cd build/instr && \
	latex ../../$< && \
	latex ../../$<

# ----------------------------------------------------------
#                     Book
.PHONY : bookdvi
bookdvi : $(GUIDESRC) $(BOOKSRC)
	cd build/book && \
	latex ../../book/book.tex && \
	latex ../../book/book.tex      # Rererun to get cross references right

.PHONY : book
book: $(GUIDESRC) $(BOOKSRC)
	cd build/book && \
	latex ../../book/book.tex && \
	latex ../../book/book.tex      # Rererun to get cross references right
	dvips -o output/book/ps/book.ps build/book/book.dvi
	dvips    -Ppdf -G0 build/book/book.dvi \
		 -o build/book/book.ps
	cd output/book/pdf && \
	ps2pdf  -sPAPERSIZE=a4 -dMaxSubsetPct=100 -dCompatibilityLevel=1.2 \
		-dSubsetFonts=true -dEmbedAllFonts=true  \
		../../../build/book/book.ps
	rm build/book/book.ps


# ----------------------------------------------------------
#                     Exercises

.PHONY : exercise
exercise : $(EXERCISES)

output/exercises/ps/%.ps : build/ex/%.dvi
	dvips -o $@ $<	

output/exercises/pdf/%.pdf : build/ex/%.dvi
	dvips -Ppdf -G0 $< -o $(patsubst %.pdf, %.ps, $@)
	cd output/exercises/pdf && \
	ps2pdf  -sPAPERSIZE=a4 -dMaxSubsetPct=100 -dCompatibilityLevel=1.2 \
		-dSubsetFonts=true -dEmbedAllFonts=true  \
		$(patsubst %.pdf, %.ps, ../../../$@)
	rm $(patsubst %.pdf, %.ps, $@)


.PHONY : exdvi
exdvi :  $(EXDVI)  

#------ Have to do them individually

# TEX->AUX
build/ex/intro.dvi : exercises/intro/intro.tex
	cd build/ex && \
	latex ../../$< && \
	latex ../../$<
build/ex/data.dvi : exercises/data/data.tex  
	cd build/ex && \
	latex ../../$< && \
	latex ../../$<
build/ex/cwramp.dvi : exercises/cwramp/cwramp.tex  
	cd build/ex && \
	latex ../../$< && \
	latex ../../$<
build/ex/rtlsim.dvi : exercises/rtlsim/rtlsim.tex
	cd build/ex && \
	latex ../../$< && \
	latex ../../$<
build/ex/io.dvi : exercises/io/io.tex 
	cd build/ex && \
	latex ../../$< && \
	latex ../../$<
build/ex/inter.dvi : exercises/inter/inter.tex
	cd build/ex && \
	latex ../../$< && \
	latex ../../$<
build/ex/mtk-sim.dvi : exercises/mtk-sim/mtk-sim.tex
	cd build/ex && \
	latex ../../$< && \
	latex ../../$<
build/ex/kernel.dvi : exercises/kernel/kernel.tex
	cd build/ex && \
	latex ../../$< && \
	latex ../../$<
build/ex/comms.dvi : exercises/comms/comms.tex
	cd build/ex && \
	latex ../../$< && \
	latex ../../$<

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # 
#                clean ups
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # 

.PHONY : clean # Removes everything
clean:
	echo ALLCLEAN
	-rm $(wildcard build/* )
	-rm $(wildcard build/ex/* )
	-rm $(wildcard build/guides/* )
	-rm $(wildcard build/instr/* )
	-rm $(wildcard output/guides/ps/* )
	-rm $(wildcard output/guides/pdf/* )
	-rm $(wildcard output/exercises/ps/* )
	-rm $(wildcard output/exercises/pdf/* )
	-rm $(wildcard output/instr/ps/* )
	-rm $(wildcard output/instr/pdf/* )
	-rm $(wildcard global/graphics/*.eps)

