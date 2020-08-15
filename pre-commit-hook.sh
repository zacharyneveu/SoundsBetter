#!/bin/bash

cwd=$(pwd)
repo_dir=$(git rev-parse --show-toplevel)
cd "${repo_dir}"

APP_PATH="."
# Files in the first level under entries in FORMAT_DIRS will be formatted
FORMAT_DIRS="${APP_PATH}/Source"
# Only files with these types will be formatted
FORMAT_TYPES="c|cpp|cc|h|hpp"

# Find OS because Darwin commands are ~different~
unameOut=$(uname -s)
case "${unameOut}" in
	Linux*)	 OS=Linux;;
	Darwin*) OS=Mac;;
esac

# Checks if list as first argument contains second argument
contains() {
    [[ $1 =~ (^|[[:space:]])$2($|[[:space:]]) ]] && return 0 || return 1
}

################################################################################
# Format all source files with clang format
# skip formatting external code - takes forever and might look jank
################################################################################

# Formats a single file in place with clang format based on .clang-format file
# Adds the file to git after it has been formatted. 
format_file() {
	input_filename="${1}"
	if [ -f $input_filename ]; then
		echo "Formatting ${input_filename}"
		clang-format -i -style=file ${input_filename}
		git add ${input_filename}
	fi
}

echo "formatting C++ and header files..."

# Check FORMAT_DIRS for immediate children of types in FORMAT_TYPES and add them to a list
# Changed files also in this list will be formatted
if [ "${OS}" = "Mac" ]; then
	ok_to_format=$(find -E "${FORMAT_DIRS}" -maxdepth 1 -type f -regex ".*\.(${FORMAT_TYPES})")
else
	ok_to_format=$(find "${FORMAT_DIRS}" -maxdepth 1 -type f -regextype posix-egrep -regex ".*\.(${FORMAT_TYPES})")
fi
#echo "${ok_to_format}" | tr " " "\n"

for changed_file in `git diff-index --cached --name-only HEAD | grep -iE "\.(${FORMAT_TYPES})"` ; do
	contains "${ok_to_format}" "${changed_file}"
	if [ $? -eq 0 ] 
	then
		format_file "${changed_file}"
	fi
done

################################################################################
# Fix Projucer splash screen
################################################################################


jucer=${APP_PATH}/SoundsBetter.jucer
# check if jucer file in changes and splash screen enabled
contains `git diff-index --cached --name-only HEAD` "${jucer}" && cat "${jucer}" | grep 'displaySplashScreen="1"' > /dev/null
if [ $? -eq 0 ]; then
	echo "Removing Projucer splash screen"
	if [ "${OS}" = "Mac" ]; then
		sed -i '' 's/ displaySplashScreen="1"//g' ${jucer}
	else
		sed -i 's/ displaySplashScreen="1"//g' ${jucer}
	fi

	git add "${jucer}"
fi

cd "${cwd}"

