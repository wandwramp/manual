# WRAMP Manual

This project contains documentation on the WRAMP architecture, specifically
the Basys implementation.
It produces output as .pdf documents, and is written in LaTeX.

With an appropriate LaTeX distribution installed, it should be possible to
simply run `make`. This will build every document available, and place them in
the output/ folder. `latexmk` is used to rerun `pdflatex` and other required
commands enough times, so if your distribution includes it, the build should
go without a hitch.

Documents can be built individually with `make book` and `make insn`.
`insn` creates a small handout detailing the WRAMP instruction-set.
`book` creates the entire reference manual, which includes all other handouts.

`make clean` will remove any intermediate and output files.

## Notes for editing diagrams

Diagrams which are saved in .pdf format have been edited using Inkscape.
When saving .pdf documents, ensure the following settings are correct:

* Convert texts to paths should be off
* PDF+LaTeX should be off

When opening .pdf documents, the following settings should be correct:

* Text handling should be "Import text as text"
* "Replace PDF fonts by closest-named installed fonts" should be off if the
"Courier" font is not installed

Feel free to ignore the warning urging you to save the file as an Inkscape SVG
when closing the document. PDF was chosen over SVG due to better text
compatability with pdftex.
