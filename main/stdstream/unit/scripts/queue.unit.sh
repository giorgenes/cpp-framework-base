#!/bin/bash

sum1=$(dd if=/dev/zero | queue-cat | md5sum)
