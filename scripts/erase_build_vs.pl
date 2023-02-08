#!/usr/bin/perl

require "./dsfx.pl";

use Cwd qw(cwd);
use File::Path qw(remove_tree);

$cwd = cwd;

dsfx::logln("Erasing Visual Studio build files except the projects...");
dsfx::logln("Current working directory: '$cwd'");

my @files_to_erase = (
	"../RSE.sln",
	"../ZERO_CHECK.vcxproj",
	"../ZERO_CHECK.vcxproj.filters",
	"../ALL_BUILD.vcxproj",
	"../ALL_BUILD.vcxproj.filters",
	"../cmake_install.cmake",
	"../CMakeCache.txt",
	"../INSTALL.vcxproj",
	"../INSTALL.vcxproj.filters",
	"../RSE/INSTALL.vcxproj",
	"../RSE/INSTALL.vcxproj.filters"
);

my @dirs_to_erase = (
	"../fmt",
	"../SDL",
	"../SDL_image",
	"../yaml-cpp",
	"../x64",
	"../CMakeFiles",
	"../build",
	"../linux-build",
	"../out"
);

foreach my $f (@files_to_erase) {
	if (!unlink $f) {
		if (-e $f) { 
			dsfx::lerrln("Failed to erase file '$f'.");
		}	
	}
}

foreach my $d (@dirs_to_erase) {
	if (!remove_tree($d)) {
		if (-e $d) {
			dsfx::lerrln("Failed to erase directory '$d'.");
		}
	}
}

dsfx::logln("Done.");

1;
