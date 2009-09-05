#!/usr/bin/perl -w
#
# $Id$
#
# Usage
# toggle.pl <start> <end> <string>
#
# Taking a string with '--enable-foo' options and toggle 'enable' to 'disable
# from position <start> to <end>.

use strict;

die "toggle.pl <start> <end> <string>" unless (@ARGV==3);
my $start = $ARGV[0];
my $end = $ARGV[1];
die "<start> < 1" if ($start < 1);
die "<start> > <end>"  if ($start > $end);

my @options = split /[[:blank:]]+/, $ARGV[2];

die "<start> out of bounds" if ($start > @options);
die "<end> out of bounds" if ($end > @options);

for my $i ($start-1 .. $end-1) {
    $options[$i] =~ s/--enable/--disable/;
}

print join(' ',@options) . "\n";
