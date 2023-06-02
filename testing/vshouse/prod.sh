#!/bin/bash

rm /tmp/productsrvdb/ -fr
mkdir /tmp/productsrvdb
> /tmp/productsrvdb/test.default.txt
rm foo -fr

prod-srv-cli-cmd create foo
prod-srv-cli-cmd checkout foo 0
prod-srv-cli-cmd commit foo
prod-srv-cli-cmd commit foo
rm foo -r
prod-srv-cli-cmd checkout foo 0.1
prod-srv-cli-cmd commit foo
prod-srv-cli-cmd listrev foo

