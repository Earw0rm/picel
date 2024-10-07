# .PHONY => target is not a file
# target always by default threated as file. If timestamp between target file and they
# dependencies is different then we build it up. Else we say that they are already up to date
.PHONY:all clean test renderer_run yasyc



all: 
	make -C ./yasyc build
	make -C ./ogl_renderer

debug: 
	make -C ./yasyc build
	make -C ./ogl_renderer debug

render:
	make -C ./ogl_renderer

test:
	make -C ./yasyc build
	make -C ./yasyc test
	make -C ./ogl_renderer test

run: 
	make -C ./ogl_renderer run 

yasyc:
	make -C ./yasyc build 

clean:
	make -C ./ogl_renderer clean 
	make -C ./yasyc clean 