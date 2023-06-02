#!/bin/bash

export REQUEST_METHOD="POST"
export CONTENT_LENGTH="29"

#cgdb ./test.cgi
~/work/bin/auth.cgi < contents.txt
