#!/usr/bin/perl

require "./dsfx.pl";
use File::Path qw(remove_tree);
use File::Copy;


$os = dsfx::util::get_os();
$cxx_compiler = "none";
$nuller = "null";
$vs_dev_env = "cmd /C \"C:/Program Files/Microsoft Visual Studio/2022/Community/Common7/Tools/VsDevCmd.bat\"";

dsfx::logln("Build for Ninja started");

dsfx::logln("Finding compilers...");
if ($os eq "mswin32") {
	#dsfx::logln("Windows platform detected.");
	$nuller = "> nul 2>&1";
	# Check for MSVC
	if (system("\"C:/Program Files (x86)/Microsoft Visual Studio/Installer/vswhere.exe\" $nuller") == 0) {
		my $vs_install_path = qx("C:/Program Files (x86)/Microsoft Visual Studio/Installer/vswhere.exe" -property installationPath");
		#dsfx::logln("Visual Studio install path: $vs_install_path");
		my $exit_code = system("$vs_dev_env && cl");
		if ($exit_code == 0) {
			$cxx_compiler = "cl";
		}
	}
		
	# Check for alternatives if MSVC was not found
	if ($cxx_compiler ne "cl") {
		if (system("clang --version $nuller") == 0) {
			$cxx_compiler = "clang";
		} elsif (system("g++ --version $nuller") == 0) {
			$cxx_compiler = "g++";
		} 	
	}
} else {
	dsfx::logln("Detected platform: $os");
	$nuller = "> /dev/null 2>&1";
	if (system("clang --version $nuller") == 0) {
		$cxx_compiler = "clang";
	} elsif (system("g++ --version $nuller") == 0) {
		$cxx_compiler = "g++";
	}
}
dsfx::logln("Found compiler: $cxx_compiler");

if (!-e "../$os-build-ninja") {
	dsfx::logln("Ninja build files are not present. Generating ninja build files...");
	mkdir("../$os-build-ninja");
	if ($cxx_compiler eq "cl") {	
		my $exit_code = system("$vs_dev_env && cmake -G \"Ninja\" ../ -B ../$os-build-ninja");
		if ($exit_code != 0) {
			dsfx::lerrln("CMake build generation for Ninja failed! Aborting...");
			remove_tree("../$os-build-ninja");
			exit -1;
		}
		dsfx::logln("Ninja build file generation finished.");
	} else {
		my $exit_code = system("cmake -G \"Ninja\" ../ -B ../$os-build-ninja");
		if ($exit_code != 0) {
			dsfx::lerrln("CMake build generation for Ninja failed! a Aborting...");
			remove_tree("../$os-build-ninja");
			exit -1;
		}
		dsfx::logln("Ninja build file generation finished.");
	}
	
	# Create symbolic links
	if ($os eq "mswin32") {
		# Windwos symlinks don't work with clangd
		copy("../$os-build-ninja/compile_commands.json", "../");
	} else {
		# Make symlink for unix like devices
	}
}

dsfx::logln("Building project...");
if ($os eq "mswin32") { 
	my $exit_code = system("$vs_dev_env && cd .. && cmake --build $os-build-ninja/");
	if ($exit_code != 0) {
		dsfx::lerrln("Build failed! Aborting...");
		exit -1;
	}
} else {
	my $exit_code = system("cmake --build ../$os-build-ninja");
	if ($exit_code != 0) {
		dsfx::lerrln("Build failed! Aborting...");
		exit -1;
	}
}

dsfx::logln("Build finished.");

1;
