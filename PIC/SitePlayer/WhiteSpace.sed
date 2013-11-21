# Remove all whitespace
# Need to watch out for spaces in strings
# replace 4 spaces by single space.
# repeated 5 times so it catches up to 20 spaces.
# For final ship may strip all spaces.
s/    //
s/    //
s/    //
s/    //
s/    //
# and 3 spaces
s/   //
# and pairs of spaces
s/  //
#repeated to handle multiple tabs
s/\t//
s/\t//
s/\t//
# Delete blank lines.
/^$/ d
/^\s*$/ d
# trailing white space
s/\s*$//
