#!/usr/bin/perl

require "./dsfx.pl";

use File::Path qw(remove_tree);
use File::Copy;
use File::Copy::Recursive qw(dircopy);

$os = dsfx::util::get_os();
$config = "debug";
$arch = dsfx::util::get_arch();
$cxx_compiler = "none";
$nuller = "null";
$vs_dev_env = "cmd /C \"C:/Program Files/Microsoft Visual Studio/2022/Community/Common7/Tools/VsDevCmd.bat\"";

dsfx::logln("Build for Ninja started");

dsfx::logln("Searching for compilers...");
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

# Argument parsing
if (scalar(@ARGV) > 0) {
	for (my $i = 0; $i < scalar(@ARGV); ++$i) {
		if (lc $ARGV[$i] eq "--config") {
			$config = lc $ARGV[++$i];
		} elsif (lc $ARGV[$i] eq "--arch") {
			$arch = lc $ARGV[++$i];
		}
	}	
} else {
	dsfx::warn("Warning! Build was not configured, defaulting to:\n\tCONFIGURATION: $config\n\tARCHITECTURE: $arch");
	#dsfx::warn("Warning! A build configuration was not selected, defulating to Debug. You can select a configuration by passing the --config flag to this script.");
}

if (!-e lc "../builds/$os-$config-$arch") {
	dsfx::logln("Ninja build files are not present. Generating ninja build files...");
	mkdir("../builds");
	if ($cxx_compiler eq "cl") {	
		my $exit_code = system("$vs_dev_env && cmake -G \"Ninja\" ../ -B " . lc "../builds/$os-$config-$arch" . "-DCMAKE_BUILD_TYPE=$config -DCMAKE_BUILD_ARCH=$arch");
		if ($exit_code != 0) {
			dsfx::lerrln("CMake build generation for Ninja failed! Aborting...");
			remove_tree("../builds/$os-$config-$arch");
			exit -1;
		}
		dsfx::logln("Ninja build file generation finished.");
	} else {
		my $exit_code = system("cmake -G \"Ninja\" ../ -B ../builds/$os-$config-$arch -DCMAKE_BUILD_TYPE=$config -DCMAKE_BUILD_ARCH=$arch");
		if ($exit_code != 0) {
			dsfx::lerrln("CMake build generation for Ninja failed! a Aborting...");
			remove_tree("../builds/$os-$config-$arch");
			exit -1;
		}
		dsfx::logln("Ninja build file generation finished.");
	}

	# Copy compile_commands.json for clangd symlinks don't work for some reason
	copy("../builds/$os-$config-$arch/compile_commands.json", "../");
}

dsfx::logln("Building project...");
if ($os eq "mswin32") { 
	my $exit_code = system("$vs_dev_env && cd .. && cmake --build builds/$os-$config-$arch/");
	if ($exit_code != 0) {
		dsfx::lerrln("Build failed! Aborting...");
		exit -1;
	}
} else {
	my $exit_code = system("cmake --build ../builds/$os-$config-$arch");
	if ($exit_code != 0) {
		dsfx::lerrln("Build failed! Aborting...");
		exit -1;
	}
}

# Copy the assets
my @assets = glob("../data");
foreach my $f (@assets) {
	if (-d $f) {
		dircopy($f, "../builds/$os-$config-$arch/TestGame");
	} else {
		copy($f, "../builds/$os-$config-$arch/TestGame");
	}
}

dsfx::logln("Build finished.");

1;
