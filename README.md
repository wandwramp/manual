# WRAMP Manual

This project contains documentation on the WRAMP architecture.
It produces output in both .ps and .pdf formats, and is written in LaTeX.

With an appropriate LaTeX distribution installed, it should be possible to
simply run `make`. This will build every document available, and place them in
the output/ folder.  
Documents can be built individually with `make standalone`, `make instr`, and
`make book`. `make exercise` creates assignments for COMP201 in 2006.

`make clean` will remove any intermediate and output files.

See [MAKE-README](MAKE-README) for extra details on Makefile targets, but be
aware that only the ALL, OUTPUT, and CLEAN sections are supported.
