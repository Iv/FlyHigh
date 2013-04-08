TARGET_DIR=./build

all: Release

create-dir:
	[ -x  $(TARGET_DIR) ] || mkdir $(TARGET_DIR)

Debug:	create-dir
	cd $(TARGET_DIR); cmake -DCMAKE_BUILD_TYPE=Debug ..; make -j4

Release:	create-dir
	cd $(TARGET_DIR); cmake -DCMAKE_BUILD_TYPE=Release ..; make -j4

clean:
	rm -rf $(TARGET_DIR)/*

install:
	cd $(TARGET_DIR); make install DESTDIR=$(prefix)

# Workaround, because KDevelop doesn't change to build directory
%.o:
	@for file in "`find ./src/ -name $(@:%.o=%.cpp)`"; \
	do \
		cd $(TARGET_DIR)/`dirname $$file`; make VERBOSE=1 $@; \
	done
