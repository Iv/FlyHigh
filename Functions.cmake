# wget -N http://nodejs.org/dist/node-latest.tar.gz
# tar xzvf node-latest.tar.gz
# cd node-latest
# ./configure
# make install
# npm install -g jshint
set(JSLINT /usr/local/bin/jshint)

function(lint_jsfiles target)
	if(EXISTS ${JSLINT})
		set(jsfiles ${ARGN})
		add_custom_target(${target} ALL
											COMMAND ${JSLINT} ${jsfiles}
											WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
											DEPENDS ${jsfiles})
	else()
		message(STATUS "${JSLINT} not installed: no lint!")
	endif()
endfunction(lint_jsfiles)
