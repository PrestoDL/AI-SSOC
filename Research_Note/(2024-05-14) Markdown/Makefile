.SUFFIXES:.pptx.pdf
PPTX_FILE=$(wildcard images/[^~]*.pptx)
PDF_FILE=${PPTX_FILE:.pptx=.pdf}
JPG_FILE=${PPTX_FILE:.pptx=.jpg}

all: fig

fig: ${PDF_FILE} ${JPG_FILE}

images/%.pdf: images/%.pptx
	ppt2pdf $< $@

images/%.jpg: images/%.pptx
	ppt2jpg $< $@


	

