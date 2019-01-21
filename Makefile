GRAPHICSPATH=./global/graphics
GRAPHICSFILES=$(wildcard $(GRAPHICSPATH)/*.pdf)
STYPATH=./global
STYFILES=$(wildcard $(STYPATH)/*.sty)

BUILDDIR=./build

BOOKNAME=$(BUILDDIR)/book.pdf
# Always ensure the top LaTeX file comes first in the
# dependency list when adding new targets
BOOKFILES=./book/book.tex \
		  ./book/preface.tex \
		  ./guide/exceptions.tex \
		  ./guide/instr-small.tex \
		  ./guide/instruction.tex \
		  ./guide/intro.tex \
		  ./guide/io-devices.tex \
		  ./guide/stackguide.tex \
		  ./guide/wrampmon.tex \
		  ./guide/wasm-wlink.tex \

INSNNAME=$(BUILDDIR)/insn.pdf
INSNFILES=./standalone/insn.tex \
		  ./guide/instr-small.tex \
		  ./guide/instruction.tex \

.PHONY: all clean book insn

all: book insn

book: $(BOOKNAME)
insn: $(INSNNAME)

$(BOOKNAME): $(BOOKFILES) $(GRAPHICSFILES) $(STYFILES)
$(INSNNAME): $(INSNFILES) $(STYFILES)

$(BOOKNAME) $(INSNNAME):
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

