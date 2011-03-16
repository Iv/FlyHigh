TARGET_DIR=./build
BIN=flyhigh
DIRS = \
	usr/ \
	usr/share/ \
	usr/share/applications/ \
	usr/share/doc/ \
	usr/share/doc/flyhigh \
	usr/share/flyhigh/ \
	usr/share/man/ \
	usr/bin/

all: Release

Debug:
	[ -x  $(TARGET_DIR) ] || mkdir $(TARGET_DIR)
	cd $(TARGET_DIR); cmake -DCMAKE_BUILD_TYPE=Debug ..; make

Release:
	[ -x  $(TARGET_DIR) ] || mkdir $(TARGET_DIR)
	cd $(TARGET_DIR); cmake -DCMAKE_BUILD_TYPE=Release ..; make

clean:
	rm -rf $(TARGET_DIR)/*

install:
	@for directory in $(DIRS) ; \
	do \
		[ -x $(prefix)/$$directory ] || mkdir $(prefix)/$$directory ; \
	done

	cp $(TARGET_DIR)/src/$(BIN) $(prefix)/usr/bin/
	strip $(prefix)/usr/bin/$(BIN)
	cp ./src/flyhigh.desktop $(prefix)/usr/share/applications/
	cp ./src/images/flyhigh.png $(prefix)/usr/share/flyhigh/
	cp ./doc/sql/flyhigh.sql $(prefix)/usr/share/flyhigh/
	cp ./doc/flyhigh $(prefix)/usr/share/doc/flyhigh/

# Workaround, because KDevelop doesn't change to build directory
%.o:
	@for file in "`find ./src/ -name $(@:%.o=%.cpp)`"; \
	do \
		cd $(TARGET_DIR)/`dirname $$file`; make VERBOSE=1 $@; \
	done
