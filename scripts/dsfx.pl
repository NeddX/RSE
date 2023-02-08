#!/usr/bin/perl

package dsfx::ansi;
%colours = (
	red		=> "\e[31m",
	green	=> "\e[32m",
	yellow	=> "\e[33m",
	blue	=> "\e[34m",
	magenta	=> "\e[35m",
	cyan	=> "\e[36m",
	lblue	=> "\e[117m",
	rst		=> "\e[0m"
);

package dsfx;
sub logln {
	my $str = shift;
	print $dsfx::ansi::colours{'cyan'}, "[~] :: $str\n", $dsfx::ansi::colours{'rst'};
}
sub log {
	my $str = shift;
	print $dsfx::ansi::colours{'cyan'}, "[~] :: $str", $dsfx::ansi::colours{'rst'};
}
sub lerrln {
	my $str = shift;
	print $dsfx::ansi::colours{'red'}, "[!] :: $str\n", $dsfx::ansi::colours{'rst'};
}
sub lerr {
	my $str = shift;
	print $dsfx::ansi::colours{'red'}, "[!] :: $str", $dsfx::ansi::colours{'rst'};
}

package dsfx::util;
use Cwd qw(cwd);
use English qw' -no_match_vars ';

sub get_os {
	return (lc "$OSNAME");
}
sub get_cwd {
	return ($CWD);
}

1;
