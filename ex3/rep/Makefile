SOURCE=
TEXFLAGS=-synctex -interaction=nonstopmode --shell-escape
TEXCC=pdflatex

all:
	$(TEXCC) $(TEXFLAGS) $(SOURCE) > /dev/null

clean:
	rm $(SOURCE).aux $(SOUCE).log $(SOURCE).out $(SOURCE).synctex.gz
