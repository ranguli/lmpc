## Process this file with automake to produce Makefile.in
# $Id$
 
SUBDIRS = com

LMPC_JARLIST=\
		com/machinima/lmpc/lmpc.class \
		com/machinima/lmpc/io/node/Node.class \
		com/machinima/lmpc/io/node/IntNode.class \
		com/machinima/lmpc/io/node/FloatNode.class \
		com/machinima/lmpc/io/node/StringNode.class \
		com/machinima/lmpc/io/node/TimeNode.class \
		com/machinima/lmpc/io/node/TokenNode.class \
		com/machinima/lmpc/io/text/LsParse.class \
		com/machinima/lmpc/io/text/LsLex.class \
		com/machinima/lmpc/io/text/TextOut.class

lmpc.jar:
	cp $@.manifest manifest.in
	$(JAR) cvfm $@ manifest.in $(LMPC_JARLIST)
	$(RM) manifest.in

