#!/bin/bash

rm /tmp/componentsrvdb/ -fr
mkdir /tmp/componentsrvdb
> /tmp/componentsrvdb/test.default.txt
rm foo -fr

component-srv-cli-cmd create foo
component-srv-cli-cmd checkout foo 0
component-srv-cli-cmd commit foo
component-srv-cli-cmd commit foo
rm foo -r
component-srv-cli-cmd checkout foo 0.1
component-srv-cli-cmd commit foo
component-srv-cli-cmd listrev foo

