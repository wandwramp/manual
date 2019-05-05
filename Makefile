GRAPHICSPATH=./global/graphics
GRAPHICSFILES=$(wildcard $(GRAPHICSPATH)/*.pdf)
STYPATH=./global
STYFILES=$(wildcard $(STYPATH)/*.sty)

BUILDDIR=./build

#                         ==== ADDING NEW TARGETS ====
#
# First, choose an output filename and create a new entry like BOOKNAME.
#
# Next, list the .tex files it depends on. Always ensure the top file
# comes first in the dependency list.
#
# Third, add a new phony target with an easy-to-remember name which can
# be passed as an argument to make. It should only depend on the output filename.
# Add it as a dependency for the `all' target.
#
# Fourth, make the output filename depend on its source files, and the
# required extra graphics files and .sty files. Most new targets will depend
# on $(STYFILES). This allows make to correctly detect when targets need to
# be re-run.
#
# Finally, add your output filename variable to the list of targets that
# execute the code including latexmk (The line directly below the Step 5 label).



#            ==== OUTPUT FILENAMES AND SOURCE FILE DEPENDENCIES ====
## Steps 1 and 2 - Output filename and .tex dependencies are followed here.

BOOKNAME=$(BUILDDIR)/book.pdf
BOOKFILES=./book/book.tex \
		  ./book/preface.tex \
		  ./guide/exceptions.tex \
		  ./guide/instr-small.tex \
		  ./guide/instruction.tex \
		  ./guide/intro.tex \
		  ./guide/io-devices.tex \
		  ./guide/stackguide.tex \
		  ./guide/wrampmon.tex \
		  ./guide/toolchain.tex \
		  ./guide/memory-map.tex \
		  ./LICENSE.tex \

INSNNAME=$(BUILDDIR)/insn.pdf
INSNFILES=./standalone/insn.tex \
		  ./guide/instr-small.tex \
		  ./guide/instruction.tex \

INSNSMALLNAME=$(BUILDDIR)/insn-small.pdf
INSNSMALLFILES=./standalone/insn-small.tex \
		  ./guide/instr-small.tex \

MEMMAPNAME=$(BUILDDIR)/memory-map.pdf
MEMMAPFILES=./standalone/mem-map.tex \
		  ./guide/memory-map.tex \



#                   ==== TARGETS AND EXTRA DEPENDENCIES ====

## Step 3 - Phony target and all dependency
.PHONY: all clean book insn insn-small memmap

all: book insn insn-small memmap

## Step 4 - Phony target tie to output filename and graphics/sty dependencies
book: $(BOOKNAME)
insn: $(INSNNAME)
insn-small: $(INSNSMALLNAME)
memmap: $(MEMMAPNAME)

$(BOOKNAME): $(BOOKFILES) $(GRAPHICSFILES) $(STYFILES)
$(INSNNAME): $(INSNFILES) $(STYFILES)
$(INSNSMALLNAME): $(INSNSMALLFILES) $(STYFILES)
$(MEMMAPNAME): $(MEMMAPFILES) $(STYFILES)

## Step 5 - How to actually build your output file
$(BOOKNAME) $(INSNNAME) $(INSNSMALLNAME) $(MEMMAPNAME):
	@echo -- Copying all source files to build/ folder....
	@mkdir -p $(BUILDDIR)
	@cp $^ $(BUILDDIR)
	@echo -- Building....
	cd $(BUILDDIR) && \
	latexmk \
		-pdf \
		-pdflatex="pdflatex -interaction=nonstopmode --shell-escape" \
		$(notdir $<)
	@echo -- Cleaning up intermediate files....
	@rm -f $(addprefix $(BUILDDIR)/,$(notdir $(GRAPHICSFILES)))
	@rm -f $(BUILDDIR)/*.tex $(BUILDDIR)/*.sty

clean:
	rm -rf $(BUILDDIR)/*

