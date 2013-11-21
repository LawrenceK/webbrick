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
# and pairs of spaces
s/   //
s/  //

# now insert HTTP/1.0 200 Ok$$ at start of text.
1i\
HTTP/1.0 200 Ok\r\
\r
