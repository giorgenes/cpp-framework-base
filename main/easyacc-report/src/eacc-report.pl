#!/usr/bin/perl

use strict;

my $TMPFILE = "";

sub paste
{
	my ($out, $header, @files) = @_;
	my $params = "";
	foreach(@files) {
		$params .= "$_ ";
	}
	`echo '$header' > $out`;
	`paste -d ';' $params >> $out`;
}

sub process_year
{
	my ($year, $root, $yroot, $dirroot) = @_;
	my $rmprev=0;
	my $prevyear=$year-1;
	my @months = ();
	my $header = "accounts;";
	if(! -e "$root/$prevyear.xml") {
		`cp $TMPFILE/accounts.xml $root/$prevyear.xml`;
		$rmprev=1;
	}

	`cp $root/$prevyear.xml $root/$year-0.xml`;
	`cp $root/$prevyear.xml $root/$year-0.closed.xml`;
	
	my $m;
	for($m=1; $m<=12; $m++) {
		my $prev=$m-1;
		print "processing $yroot/$m.txt\n";
		if (-e "$yroot/$m.txt") {
			push (@months, $m);
			$header .= "$m;";
			eacc("-p", "xml", 
				"$root/$year-$prev.xml", 
				"$yroot/$m.txt", 
				"$root/$year-$m.xml");

			eacc("-p", "xml", 
				"$root/$year-$prev.closed.xml",
				"$yroot/$m.txt",
				"$root/$year-$m.m.xml");

			eacc("-c", "xml",
				"$root/$year-$m.m.xml",
				"",
				"$root/$year-$m.closed.xml");
		} 
		else {
			`cp $root/$year-$prev.xml $root/$year-$m.xml`;
			`cp $root/$year-$prev.closed.xml $root/$year-$m.closed.xml`;
			`cp $root/$year-$prev.closed.xml $root/$year-$m.m.xml`;
		}
		eacc("", "csv", "$root/$year-$m.m.xml", "", "$root/$year-$m.csv");
		unlink "$root/$year-$m.m.xml";
	}

	eacc("", "csv", "$root/$year-12.xml", "", "$root/$year.csv");
	eacc("-c", "xml", "$root/$year-12.xml", "", "$root/$year.xml");
	`rm -f $root/$year-*.xml`;
	if($rmprev == 1) {
		unlink "$root/$prevyear.xml";
	}
	`cut -d ';' -f 1 $root/$year.csv > $root/accounts.lst`;
	
	$header .= "$year";
	my @lst = ();
	push(@lst, "$root/accounts.lst");
	foreach(@months) {
		`cut -d ';' -f 3 $root/$year-$_.csv > $root/$year-$_.lst`;
		push(@lst, "$root/$year-$_.lst");
	}
	`cut -d ';' -f 3 $root/$year.csv > $root/$year.lst`;
	push(@lst, "$root/$year.lst");
	paste("$dirroot/$year-full.txt", $header, @lst);
	`rm $root/*.lst`;
	`rm $root/*.csv`;
}

sub proc_set
{
	my ($root, $which) = @_;
	
	print "criando $TMPFILE/$which\n";;
	mkdir("$TMPFILE/$which");
	
	opendir(DIR, $root) || die "can't opendir $root: $!";
	my @years = grep { 
		/[0-9]+/ && -d "$root/$_" 
	}readdir(DIR);
	closedir DIR;

	foreach(@years) {
		process_year($_, "$TMPFILE/$which", "$root/$_", $root);
	}	
	`find $TMPFILE/$which -name '*.xml' -exec rm {} \\;`;
}

sub merge_year
{
	my ($year) = @_;
}

sub merge_years
{
	my $root = "$TMPFILE/cur";
	my @years_cur = ();
	my @years_fore = ();
	my $dir;

	opendir(DIR, $root) || die "can't opendir: $root $!";
	while($dir = readdir(DIR)) {
		if($dir =~ /([0-9]+)-full.txt/) {
			push(@years_cur, $1);
		}	
	}
	closedir DIR;

	$root = "$TMPFILE/forecast";
	opendir(DIR, $root) || die "can't opendir: $root $!";
	while($dir = readdir(DIR)) {
		if($dir =~ /([0-9]+)-full.txt/) {
			push(@years_fore, $1);
		}	
	}
	closedir DIR;

	foreach(@years_fore) {
		push(@years_cur, $_);
	}

	sort @years_cur;
	my $prev = 0;
	foreach(@years_cur) {
		if($_ != $prev) {
			$prev = $_;
			merge_year($_);
		}
	}
}

sub eacc
{
	my ($param, $format, $acc, $trn, $out) = @_;

	#print "eacc $param -f $format -a $acc $trn -o $out\n";
	`eacc $param -f $format -a $acc $trn -o $out`;
}

sub main {
	if(@ARGV < 1) {
		print "eacc-report.sh path\n";
		exit(-1);
	}

	my $root = $ARGV[0];
	$TMPFILE=`mktemp /tmp/eaccreport.XXXXXX` || exit 1;
	chomp $TMPFILE;
	unlink $TMPFILE;
	mkdir $TMPFILE;

	if(-e "$root/init.txt") {
		eacc("-p", "xml",
			"$root/accounts.xml", 
			"$root/init.txt", 
			"$TMPFILE/accounts.xml.tmp");

		eacc("-c", "xml", 
			"$TMPFILE/accounts.xml.tmp", 
			"", 
			"$TMPFILE/accounts.xml");
		unlink "$TMPFILE/accounts.xml.tmp";
	} 
	else {
		`cp $root/accounts.xml $TMPFILE/`;
	}

	proc_set("$root/cur", "cur");
	proc_set("$root/forecast", "forecast");

	merge_years();

	#system("find $TMPFILE");
	#system("tar -czf result.tar.gz $TMPFILE");
	system("rm -fr $TMPFILE");
}

main();
