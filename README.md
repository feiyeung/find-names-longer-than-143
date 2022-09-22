# find-names-longer-than-143

Scan filesystem and locate filename or dirname that's more than 143 bytes (not to be confused with 143 characters). Used for making my files compatible with eCryptFS.

Why do I not just use some shell command or python, that might seem more readable and one-line-able? Well I tried few, but i still run into issues with non-ASCII or non-UNICODE characters, such that some tools might not account for their actual number of bytes correctly.

## Build

    gcc -o whatever names_longer_than_ecryptfs.c

## Usage Example

To scan a dir and list only entries whose names are more than 143 bytes, while saving to a file

    ./whatever <path(s) to scan> 2>/dev/null | tee gt143.txt

To scan and list everything (regardless of longer than 143 or not), while saving to a file:

    ./whatever -v <path(s) to scan> 2>&1 | tee everything.txt

To scan a dir and list only entries whose names are more than 136 bytes, while saving to a file

    ./whatever -l 136 <path(s) to scan> 2>/dev/null | tee gt136.txt

(why 136? i had an issue when rsync'ing into a target with eCryptFS, and found that rsync would call mktemp at somepoint and that system of mine defaults to `.XXXXXX` that adds 7 bytes to the file names, so i had to find those with more than 136 bytes filenames and rename them)

## Output format

    <number of bytes for the name> <whitespace> <quote> <fullpath> <quote>

## Test

A empty dir with non-Latin name is under the `test` dir. One can run the software against that dir to see if the filename size is accounted properly.
