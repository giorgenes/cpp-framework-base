#!/bin/perl
#

%nodes = {};

print "digraph TestGraph {\n";
while(<STDIN>) {
	chomp $_;
	@l = split(/ /, $_);
	
	$label = @l[0];
	foreach(@l) {
		$_ =~ s/\./_/g;
		$_ = "n" . $_;
	}
	print "node [shape=ellipse fontsize=10 fontname=\"Courier\"]; $l[0];\n";
	print "\"$l[0]\" [label=\"$label\"];\n";
	for($i=1; $i<@l; $i++) {
		print "\"@l[0]\" -> \"@l[$i]\";\n";
	}
}

print "}\n";
