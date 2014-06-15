# ~ Copyright: (c) 2014. Turtsevich Alexander
# ~
# ~ Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.html

OS=$(shell uname)

#select makefile depending on OS
ifneq (,$(findstring Darwin,$(OS)))
	#mac os x
	FILE=makefile.mac
else
	#windows
	FILE=makefile.win
endif

include $(FILE)

