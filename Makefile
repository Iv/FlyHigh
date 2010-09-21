TARGET_DIR=./build
BIN=flyhigh

all:
	[ -x  $(TARGET_DIR) ] || mkdir $(TARGET_DIR)
	cd $(TARGET_DIR); cmake -DCMAKE_BUILD_TYPE=$(BUILD_TYPE) ..; make

clean:
	rm -rf $(TARGET_DIR)/*

# Workaround, because KDevelop doesn't change to build directory
%.o:
	@for file in "`find ./src/ -name $(@:%.o=%.cpp)`"; \
	do \
		cd $(TARGET_DIR)/`dirname $$file`; make VERBOSE=1 $@; \
	done
