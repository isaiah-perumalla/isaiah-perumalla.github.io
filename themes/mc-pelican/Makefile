VERSION=20150208
DIST=mc-pelican-$(VERSION)
DISTFILES=README.txt LICENSE Makefile static templates

$(DIST).tar.bz2:
	mkdir $(DIST)
	cp -r $(DISTFILES) $(DIST)/
	tar cf $(DIST).tar --exclude .git $(DIST)
	bzip2 -9 $(DIST).tar
	$(RM) -rf $(DIST)

dist: $(DIST).tar.bz2

clean:
	$(RM) -f $(DIST).tar.bz2

