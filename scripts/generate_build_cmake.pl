#!/usr/bin/perl

require "./dsfx.pl";
use File::Path qw(remove_tree);
use Cwd 'abs_path';

dsfx::logln("CMake build file generation started...");

my $os = dsfx::util::get_os();

if (!-e "../$os-build") {	
	dsfx::logln("CMake build files are missing. Generating new build files...");	
	my $exit_code = system("cmake ../ -B ../$os-build");
	if ($exit_code != 0) {
		dsfx::lerrln("CMake build files generation failed! Aborting...");
		exit -1;	
	}
	my $abs_path = abs_path("../$os-build");
	dsfx::logln("Wrote cmake build files at '$abs_path'");
}

dsfx::logln("Starting CMake build...");
my $exit_code = system("cmake --build ../$os-build");
if ($exit_code != 0) {
	dsfx::lerrln("CMake build failed! Aborting...");
	exit -1;
}

my $abs_path = abs_path("../$os-build");
dsfx::logln("CMake build finished.");
dsfx::logln("Built files are at: '$os'");
